
#pragma once

#include "vulkan.h"

namespace gg {

	struct Window {
		int width;
		int height;
		const char *name;
		GLFWwindow* glfw_window;
		VkSurfaceKHR surface;

		class Swapchain *swapchain;
		
		Window(int in_width, int in_height, const char *in_name);
	};
}
