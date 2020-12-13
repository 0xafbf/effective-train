

namespace gg {
	struct Swapchain {
		VkSwapchain handle;
		VkImage *images;
		VkImageView *image_views;
		Swapchain(class Window *window);
	}
}
