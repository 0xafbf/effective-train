
#pragma once

#include "vulkan.h"

namespace gg {

	struct Engine {
		
		VkInstance instance;
		VkPhysicalDevice physical_device;
		VkDevice device;
		uint32_t graphics_queue_family_idx;
		uint32_t present_queue_family_idx;

		void ensure_present_queue(VkSurfaceKHR surface);

		VkSurfaceFormatKHR get_surface_format(VkSurfaceKHR surface);
	};
		
	/** Creates a new engine, sets it as default, and returns it */
	Engine* Init();
	Engine* GetEngine();
	
}
