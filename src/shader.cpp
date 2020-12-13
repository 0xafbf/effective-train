
#include "shader.h"
#include "engine.h"
#include "vulkan.h"


const char *shader_ids;
VkShaderModule *shaders;

VkShaderModule *gg::VkGetShader(const char* path) {

	int code_size;
	void *spv = stb_file(path, &code_size);

	VkShaderModuleCreateInfo shader_info {};
	shader_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shader_info.codeSize = code_size;
	shader_info.pCode = (uint32_t*) spv;

	VkShaderModule shader_module;
	gg::Engine *engine = gg::GetEngine();
	VK_CHECK(vkCreateShaderModule(engine->device, &shader_info, nullptr, &shader_module));

	return shader_module;
}		
