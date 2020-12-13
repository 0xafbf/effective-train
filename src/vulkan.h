
#pragma once

#include "helper.h"
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"


#define VK_CHECK(f)										\
	{													\
		VkResult res = (f);								\
		if (res != VK_SUCCESS) {						\
			stbprint(									\
				"Fatal : VkResult is '%d' in %s:%d\n",	\
				res, __FILE__, __LINE__);				\
			BREAK();									\
		}												\
	}
