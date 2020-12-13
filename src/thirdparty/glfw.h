

/*
// summary of things we need to do:


// for every platform:
clang -c .\glfw\src\context.c -D_GLFW_WIN32 -D_CRT_SECURE_NO_WARNINGS -o bin/context.o
clang -c .\glfw\src\init.c -D_GLFW_WIN32 -D_CRT_SECURE_NO_WARNINGS -o bin/init.o
clang -c .\glfw\src\input.c -D_GLFW_WIN32 -D_CRT_SECURE_NO_WARNINGS -o bin/input.o
clang -c .\glfw\src\monitor.c -D_GLFW_WIN32 -D_CRT_SECURE_NO_WARNINGS -o bin/monitor.o
clang -c .\glfw\src\vulkan.c -D_GLFW_WIN32 -D_CRT_SECURE_NO_WARNINGS -o bin/vulkan.o
clang -c .\glfw\src\window.c -D_GLFW_WIN32 -D_CRT_SECURE_NO_WARNINGS -o bin/window.o


// for win32:

clang -c .\glfw\src\win32_init.c -D_GLFW_WIN32 -D_CRT_SECURE_NO_WARNINGS -o bin/win32_init.o
clang -c .\glfw\src\win32_joystick.c -D_GLFW_WIN32 -D_CRT_SECURE_NO_WARNINGS -o bin/win32_joystick.o
clang -c .\glfw\src\win32_monitor.c -D_GLFW_WIN32 -D_CRT_SECURE_NO_WARNINGS -o bin/win32_monitor.o
clang -c .\glfw\src\win32_time.c -D_GLFW_WIN32 -D_CRT_SECURE_NO_WARNINGS -o bin/win32_time.o
clang -c .\glfw\src\win32_thread.c -D_GLFW_WIN32 -D_CRT_SECURE_NO_WARNINGS -o bin/win32_thread.o
clang -c .\glfw\src\win32_window.c -D_GLFW_WIN32 -D_CRT_SECURE_NO_WARNINGS -o bin/win32_window.o
clang -c .\glfw\src\wgl_context.c -D_GLFW_WIN32 -D_CRT_SECURE_NO_WARNINGS -o bin/wgl_context.o
clang -c .\glfw\src\egl_context.c -D_GLFW_WIN32 -D_CRT_SECURE_NO_WARNINGS -o bin/egl_context.o
clang -c .\glfw\src\osmesa_context.c -D_GLFW_WIN32 -D_CRT_SECURE_NO_WARNINGS -o bin/osmesa_context.o

// then link:

llvm-ar rc bin/glfw.a bin/context.o bin/init.o bin/input.o bin/monitor.o bin/vulkan.o bin/window.o bin/win32_init.o bin/win32_joystick.o bin/win32_monitor.o bin/win32_time.o bin/win32_thread.o bin/win32_window.o bin/wgl_context.o bin/egl_context.o bin/osmesa_context.o

// additionally, we need to add the glfw/include dir to the registry



*/

#include "builder.h"
#include "stb/stb.h"

void compile_stb(b_Compile_Context *context);
void compile_glfw(b_Compile_Context *context);

#ifdef EXPORT_BUILD_COMMANDS
void compile_stb(b_Compile_Context *context)
{
	char** new_item = stb_arr_add(context->include_directories);
	*new_item = stb_mprintf("%s/stb", context->current_path);

	char** dependency = stb_arr_add(context->libraries);
	*dependency = stb_mprintf("%s", "advapi32");
}

void compile_glfw(b_Compile_Context *context)
{
	char** include_directory = stb_arr_add(context->include_directories);
	*include_directory = stb_mprintf("%s/glfw/include", context->current_path);

	char** dependency = stb_arr_add(context->libraries);
	*dependency = stb_mprintf("%s", "gdi32");
	dependency = stb_arr_add(context->libraries);
	*dependency = stb_mprintf("%s", "user32");
	dependency = stb_arr_add(context->libraries);
	*dependency = stb_mprintf("%s", "shell32");


// here we are missing the actual compiling, TODO later

	char** output = stb_arr_add(context->outputs);
	*output = stb_mprintf("%s/bin/glfw.a", context->current_path);

}

// doesn't actually compile vulkan, but sets up environment for using it
void compile_vulkan(b_Compile_Context *context)
{
	char** include_directory = stb_arr_add(context->include_directories);
	char* vulkan_env = getenv("VULKAN_SDK");
	*include_directory = stb_mprintf("%s/Include", vulkan_env);

	char** dependency = stb_arr_add(context->libraries);
	*dependency = stb_mprintf("%s/Lib/vulkan-1", vulkan_env);
}
#endif


