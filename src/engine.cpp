
#include "engine.h"
#include "vulkan.h"


gg::Engine *engine = nullptr;
gg::Engine *gg::GetEngine() {
	return engine;
}

VkQueueFamilyProperties queue_family_props[10];
uint queue_family_count;
	

VkBool32 debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity, VkDebugUtilsMessageTypeFlagsEXT type, const VkDebugUtilsMessengerCallbackDataEXT* data, void* userData) {

	stbprint("validation layer error: %s\n", data->pMessage);
	return VK_FALSE;
}


void gg::Init() {
	ASSERT(engine == nullptr);
	engine = new gg::Engine();
	stb_("CALLING INIT");
	
	glfwInit();

	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Granada Graphics Application";
	appInfo.applicationVersion = VK_MAKE_VERSION(1,0,0);
	appInfo.pEngineName = "Granada Graphics";
	appInfo.engineVersion = VK_MAKE_VERSION(1,0,0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo instance_info = {};
	instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instance_info.pApplicationInfo = &appInfo;

	const char *all_extensions[20] = {};
	uint32_t extension_count;
	const char **glfw_extensions = glfwGetRequiredInstanceExtensions(&extension_count);
	memcpy(all_extensions, glfw_extensions, sizeof(char*) * extension_count);

	for (int idx = 0; idx < extension_count; ++idx) {
		stbprint("layer: %s\n", all_extensions[idx]);
	}

	all_extensions[extension_count++] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;

	instance_info.enabledExtensionCount = extension_count;
	instance_info.ppEnabledExtensionNames = all_extensions;

	static const char *validation_layers[] = {
		"VK_LAYER_KHRONOS_validation"
	};
	instance_info.enabledLayerCount = stb_arrcount(validation_layers);
	instance_info.ppEnabledLayerNames = validation_layers;

	VkDebugUtilsMessengerCreateInfoEXT messenger_info = {};
	messenger_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	messenger_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
	messenger_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |  VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |  VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	messenger_info.pfnUserCallback = debugCallback;

	instance_info.pNext = &messenger_info;


	VK_CHECK(vkCreateInstance(&instance_info, nullptr, &engine->instance));
	
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	ASSERT(func != nullptr);
	VkDebugUtilsMessengerEXT debug_messenger;
	func(engine->instance, &messenger_info, nullptr, &debug_messenger);



	VkPhysicalDevice physical_devices[4];
	uint device_count = 4;
	vkEnumeratePhysicalDevices(instance, &device_count, physical_devices);
	stbprint("Found %d devices:\n", device_count);

	int device_to_use = -1;
	VkPhysicalDeviceFeatures device_features {};
	for (int idx = 0; idx < device_count; ++idx) {
		vkGetPhysicalDeviceFeatures(physical_devices[idx], &device_features);
		if (!device_features.geometryShader) continue;

		VkPhysicalDeviceProperties device_properties;
		vkGetPhysicalDeviceProperties(physical_devices[idx], &device_properties);
		if (device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
			device_to_use = idx;
		}
		stbprint("%d. %s\n", idx, device_properties.deviceName);
	}

	VkPhysicalDevice physical_device = physical_devices[device_to_use];
	engine->physical_device = physical_device;
	vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queue_family_props);

	
	engine->graphics_queue_family_idx = -1;
	engine->present_queue_family_idx = -1;

	for (int idx = 0; idx < queue_family_count; ++idx) {
		if (queue_family_props[idx].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			graphics_queue_family_idx = idx;
			break;
		}
	}
	ASSERT(engine->graphics_queue_family_idx != -1);

	
	VkDeviceQueueCreateInfo graphics_queue_info{};
	graphics_queue_info->sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	graphics_queue_info->queueFamilyIndex = graphics_queue_family_idx;
	graphics_queue_info->queueCount = 1;
	float graphics_queue_priority = 1.0f;
	graphics_queue_info->pQueuePriorities = &graphics_queue_priority;



	
	uint32_t num_extension_properties = 0;
	vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &num_extension_properties, nullptr);
	VkExtensionProperties extension_properties = malloc(sizeof(VkExtensionProperties) * num_extension_properties);
	vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &num_extension_properties, extension_properties);

	const char *device_extensions[] = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};
	bool all_extensions_found = true;
	int num_required_extensions = stb_arrcount(device_extensions);
	for (int idx = 0; idx < num_required_extensions; ++idx) {
		bool extension_found = false;
		const char* device_extension = device_extensions[idx];
		for (int jdx = 0; jdx < num_extension_properties; jdx++) {
			char* available_extension = extension_properties[jdx].extensionName;
			if (strcmp(available_extension, device_extension) == 0) {
				extension_found = true;
				break;
			}
		}
		if (!extension_found) {
			all_extensions_found = false;
			break;
		}
	}
	assert(all_extensions_found);



	
	VkDeviceCreateInfo device_info {};
	device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	device_info.pQueueCreateInfos = queue_infos;
	device_info.queueCreateInfoCount = stb_arrcount(queue_infos);

	device_info.pEnabledFeatures = &device_features;
	device_info.enabledExtensionCount = stb_arrcount(device_extensions);
	device_info.ppEnabledExtensionNames = device_extensions;


	VK_CHECK(vkCreateDevice(physical_device, &device_info, nullptr, &engine->device));

	
}


void gg::Engine::ensure_present_queue(VkSurfaceKHR surface) {
	for (int idx = 0; idx < queue_family_count; ++idx) {
		VkBool32 present_support = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, idx, surface, &present_support);
		if (present_support) {
			present_queue_family_idx = idx;
			break;
		}
	}
	ASSERT(present_queue_family_idx != -1);

	
	VkDeviceQueueCreateInfo present_queue_info {};
	present_queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	present_queue_info.queueFamilyIndex = present_queue_family_idx;
	present_queue_info.queueCount = 1;
	float present_queue_priority = 1.0f;
	present_queue_info.pQueuePriorities = &present_queue_priority;

}


VkSurfaceFormatKHR gg::Engine::get_surface_format(VkSurfaceKHR surface) {

	VkSurfaceFormatKHR formats[100];
	uint32_t format_count = 100;
	vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, formats);
	assert(format_count > 0);

	VkSurfaceFormatKHR desired_format {};
	desired_format.format = VK_FORMAT_B8G8R8A8_SRGB;
	desired_format.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
	
	bool format_available = false;
	for (int idx = 0; idx < format_count; ++idx) {
		VkSurfaceFormatKHR format = formats[idx];
		if (format.format == desired_format.format
			&& format.colorSpace == desired_format.colorSpace
		) {
			format_available = true;
			break;
		}
	}
	assert(format_available);

	return desired_format;
}

VkPresentModeKHR *present_modes;

VkPresentModeKHR gg::Engine::get_present_mode(VkSurfaceKHR surface) {

	if (present_modes == null) {
		uint32_t present_modes_count;
		vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_modes_count, present_modes);
		assert(present_modes_count > 0);
		present_modes = malloc(sizeof(VkPresentModeKHR) * present_modes_count);

		vkGetPhysicalDeviceSurfacePresentModesKHR(
			engine->physical_device,
			surface,
			&present_modes_count,
			present_modes
		);

	}
	
	VkPresentModeKHR present_mode = VK_PRESENT_MODE_FIFO_KHR;
	for (int idx = 0; idx < present_modes_count; ++idx) {
		if (present_modes[idx] == VK_PRESENT_MODE_MAILBOX_KHR) {
			present_mode = VK_PRESENT_MODE_MAILBOX_KHR;
		}
	}

	return present_mode;

}
