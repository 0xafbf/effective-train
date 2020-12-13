#pragma once

struct b_Compile_Context {
	char* project_path;
	char* current_path;
	char** include_directories;
	char** libraries;
	char** outputs;
};
