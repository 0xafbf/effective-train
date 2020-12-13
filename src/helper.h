
#pragma once

#include "stb.h"
#define BREAK() {								\
		int* p = nullptr;						\
		*p = 1;									\
}

#define ASSERT(cond){													\
		if (!(cond)) {													\
			stbprint("Assertion failed in %s:%d\n", __FILE__, __LINE__); \
			BREAK();													\
		}																\
	}
