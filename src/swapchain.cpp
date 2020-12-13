


#include "swapchain.h"

gg::Swapchain::Swapchain(gg::Window *window) {


	VkSurfaceCapabilitiesKHR capabilities;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(engine->physical_device, surface, &capabilities);
	VkExtent2D* min = &capabilities.minImageExtent;
	VkExtent2D* max = &capabilities.maxImageExtent;
	VkExtent2D surface_extents = {
		stb_clamp(width, min.width, max.width),
		stb_clamp(height, min.height, max.height),
	};

	uint32_t image_count = capabilities.minImageCount + 1;
	if (capabilities.maxImageCount != 0) {
		image_count = stb_min(image_count, capabilities.maxImageCount);
	}

	VkSwapchainCreateInfoKHR swapchain_info {};
	swapchain_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchain_info.surface = window->surface;
	// VkSwapchainCreateFlagsKHR          flags;
	swapchain_info.minImageCount = image_count;
	swapchain_info.imageFormat = desired_format.format;
	swapchain_info.imageColorSpace = desired_format.colorSpace;
	swapchain_info.imageExtent = surface_extents;
	swapchain_info.imageArrayLayers = 1;
	swapchain_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	
	swapchain_info.preTransform = capabilities.currentTransform;
	swapchain_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchain_info.presentMode = present_mode;
	swapchain_info.clipped = VK_TRUE;
	swapchain_info.oldSwapchain = VK_NULL_HANDLE;

	gg::Engine *engine = gg::GetEngine();
	uint32_t queue_indices[] = {
		engine->graphics_queue_family_idx,
		engine->present_queue_family_idx
	};
	if (queue_indices[0] != queue_indices[1]) {
		swapchain_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		swapchain_info.queueFamilyIndexCount = 2;
		swapchain_info.pQueueFamilyIndices = queue_indices;
	} else {
		swapchain_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapchain_info.queueFamilyIndexCount = 0;
		swapchain_info.pQueueFamilyIndices = nullptr;
	}


	VK_CHECK(vkCreateSwapchainKHR(engine->device, &swapchain_info, nullptr, &handle));


	
	uint32_t swapchain_image_count;
	vkGetSwapchainImagesKHR(device, handle, &swapchain_image_count, nullptr);
	images = malloc(sizeof(VkImage) * swapchain_image_count);
	vkGetSwapchainImagesKHR(device, handle, &swapchain_image_count, images);

	image_views = malloc(sizeof(VkImageView) * swapchain_image_count);

	VkImageViewCreateInfo image_view_info {};
	image_view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	image_view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
	image_view_info.format = desired_format.format;
	image_view_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	image_view_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	image_view_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	image_view_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
	image_view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	image_view_info.subresourceRange.baseMipLevel = 0;
	image_view_info.subresourceRange.levelCount = 1;
	image_view_info.subresourceRange.baseArrayLayer = 0;
	image_view_info.subresourceRange.layerCount = 1;

	for (int idx = 0; idx < swapchain_image_count; ++idx) {
		image_view_info.image = images[idx];
		VK_CHECK(vkCreateImageView(device, &image_view_info, nullptr, &image_views[idx]));
	}

	

	
}
