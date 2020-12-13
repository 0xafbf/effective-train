
#define _CRT_SECURE_NO_WARNINGS
#include "src/microsoft_craziness.h"

#include "src/thirdparty/builder.h"


#define STB_DEFINE
#define STB_NO_REGISTRY
#include "src/thirdparty/stb/stb.h"
#undef STB_DEFINE

#define EXPORT_BUILD_COMMANDS
#include "src/thirdparty/glfw.h"
#undef EXPORT_BUILD_COMMANDS

int arg_count;
char** arg_values;

void compile_stuff() {

	b_Compile_Context compile_context = {};
	char* project_path = stb_splitpathdup(arg_values[0], STB_PATH);
	compile_context.project_path = project_path;
	char current_path[1024];
	stb_snprintf(current_path, 1024, "%ssrc/thirdparty", project_path);
	stb_fixpath(current_path);
	compile_context.current_path = "src/thirdparty";

	compile_stb(&compile_context);
	compile_glfw(&compile_context);
	compile_vulkan(&compile_context);

	int num_includes = stb_arr_len(compile_context.include_directories);
	char include_directives[1024];
	int include_directives_size = 0;
	for (int idx = 0; idx < num_includes; ++idx) {

		char *directive = stb_sprintf("-I%s ", compile_context.include_directories[idx]);
		int max_size = include_directives_size - 1024;
		int additional_size = stb_strscpy(include_directives + include_directives_size, directive, max_size);
		include_directives_size += additional_size - 1;
		stbprint("directive: %s\n", directive);

	}
	stbprint("include directives: %s\n", include_directives);

	char* files[] = {
		"test1.cpp",
		"main.cpp",
		"window.cpp",
		"engine.cpp"
	};
	int num_files = stb_arrcount(files);
	stb_("num files: %d", num_files);

	for (int idx = 0; idx < num_files; ++idx) {
		char* full_path = files[idx];
		char* path_noext = stb_splitpathdup(full_path, STB_PATH | STB_FILE);
		char* ext = stb_splitpathdup(full_path, STB_EXT);
		char* command = stb_sprintf("clang -g -gcodeview \"src/%s\" -c -o  \"bin/%s.o\" %s -Wno-writable-strings", full_path, path_noext, include_directives);
		stbprint("CMD: %s\n", command);
		system(command);


		char** file_output = stb_arr_add(compile_context.outputs);
		*file_output = stb_mprintf("bin/%s.o", path_noext);
	}

	int num_outputs = stb_arr_len(compile_context.outputs);
	char output_command[1024];
	int output_command_size = 0;
	for (int idx = 0; idx < num_outputs; ++idx) {
		char *directive = stb_sprintf("%s ", compile_context.outputs[idx]);
		int max_size = output_command_size - 1024;
		int additional_size = stb_strscpy(output_command + output_command_size, directive, max_size);
		output_command_size += additional_size - 1;
	}

	int num_libs = stb_arr_len(compile_context.libraries);
	for (int idx = 0; idx < num_libs; ++idx) {
		char *directive = stb_sprintf("-l%s ", compile_context.libraries[idx]);
		int max_size = output_command_size - 1024;
		int additional_size = stb_strscpy(output_command + output_command_size, directive, max_size);
		output_command_size += additional_size - 1;
	}

	char* link_command = stb_sprintf("clang -g %s -o bin/main.exe", output_command);
	stbprint("linking with command: %s", link_command);
	system(link_command);

/*
	auto vs_env = find_visual_studio_and_windows_sdk();
	stbprint("exe:%s\n", vs_env.vs_exe_path);
	stbprint("lib:%s\n", vs_env.windows_sdk_root);
*/
}

int main(int argc, char** argv) {
	arg_count = argc;
	arg_values = argv;

	compile_stuff();
}
