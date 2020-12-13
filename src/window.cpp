

#include "window.h"
#include "engine.h"
#include "swapchain.h"


gg::Window::Window(int in_width, int in_height, const char *in_name):
	width(in_width),
	height(in_height),
	name(in_name)
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	glfw_window = glfwCreateWindow(width, height, name, nullptr, nullptr);

	VK_CHECK(glfwCreateWindowSurface(gg::Instance(), glfw_window, nullptr, &surface));

	swapchain = new gg::Swapchain(this);
}
