

#define STB_DEFINE
#include "stb.h"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include "test1.h"
#include "gg.h"


uint32_t WIDTH = 800;
uint32_t HEIGHT = 600;

int main(int argc, char** argv) {
	gg::Init();
	auto engine = gg::GetEngine();
	auto instance = engine->instance;

	gg::Window* w = new gg::Window(WIDTH, HEIGHT, "My Window");
	auto window = w->glfw_window;
	VkSurfaceKHR surface = w->surface;
	

	VkQueue graphics_queue;
	vkGetDeviceQueue(engine->device, engine->graphics_queue_family_idx, 0, &graphics_queue);

	VkQueue present_queue;
	vkGetDeviceQueue(engine->device, engine->present_queue_family_idx, 0, &present_queue);


	VkPipelineShaderStageCreateInfo vertex_stage_info {};
	vertex_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertex_stage_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertex_stage_info.module = gg::VkGetShader("content/vert.spv");
	vertex_stage_info.pName = "main";

	VkPipelineShaderStageCreateInfo fragment_stage_info {};
	fragment_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragment_stage_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragment_stage_info.module = gg::VkGetShader("content/frag.spv");
	fragment_stage_info.pName = "main";

	VkPipelineShaderStageCreateInfo shader_stages[] = { vertex_stage_info, fragment_stage_info };

	VkPipelineVertexInputStateCreateInfo vertex_info {};
	vertex_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertex_info.vertexBindingDescriptionCount = 0;
	vertex_info.pVertexBindingDescriptions = nullptr;
	vertex_info.vertexAttributeDescriptionCount = 0;
	vertex_info.pVertexAttributeDescriptions = nullptr;

	VkPipelineInputAssemblyStateCreateInfo assembly_info {};
	assembly_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	assembly_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	assembly_info.primitiveRestartEnable = VK_FALSE;


	VkViewport viewport {};
	viewport.x = 0;
	viewport.y = 0;
	viewport.width = surface_extents.width;
	viewport.height = surface_extents.height;
	viewport.minDepth = 0;
	viewport.maxDepth = 1;

	VkRect2D scissor {};
	scissor.extent = surface_extents;

	VkPipelineViewportStateCreateInfo viewport_state {};
	viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewport_state.viewportCount = 1;
	viewport_state.pViewports = &viewport;
	viewport_state.scissorCount = 1;
	viewport_state.pScissors = &scissor;

	VkPipelineRasterizationStateCreateInfo rasterization_state {};
	rasterization_state.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterization_state.depthClampEnable = VK_FALSE;
	rasterization_state.rasterizerDiscardEnable = VK_FALSE;
	rasterization_state.polygonMode = VK_POLYGON_MODE_FILL;
	rasterization_state.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterization_state.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterization_state.depthBiasEnable = VK_FALSE;
	// rasterization_state.depthBiasConstantFactor;
	// rasterization_state.depthBiasClamp;
	// rasterization_state.depthBiasSlopeFactor;
	rasterization_state.lineWidth = 1;

	VkPipelineMultisampleStateCreateInfo multisample_info {};
	multisample_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisample_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisample_info.sampleShadingEnable = VK_FALSE;
	// float                    minSampleShading;
	// const  VkSampleMask *     pSampleMask;
	// multisample_info.alphaToCoverageEnable;
	// multisample_info.alphaToOneEnable;

	VkPipelineColorBlendAttachmentState color_blend_attachment {};
	color_blend_attachment.blendEnable = VK_FALSE;
	// VkBlendFactor            srcColorBlendFactor;
	// VkBlendFactor            dstColorBlendFactor;
	// VkBlendOp                colorBlendOp;
	// VkBlendFactor            srcAlphaBlendFactor;
	// VkBlendFactor            dstAlphaBlendFactor;
	// VkBlendOp                alphaBlendOp;
	color_blend_attachment.colorWriteMask = (
		VK_COLOR_COMPONENT_R_BIT |
		VK_COLOR_COMPONENT_G_BIT |
		VK_COLOR_COMPONENT_B_BIT |
		VK_COLOR_COMPONENT_A_BIT
	);

	VkPipelineColorBlendStateCreateInfo color_blend_info {};
	color_blend_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	color_blend_info.logicOpEnable = VK_FALSE;
	// VkLogicOp                logicOp;
	color_blend_info.attachmentCount = 1;
	color_blend_info.pAttachments = &color_blend_attachment;
	// float                    blendConstants [4];

	VkDynamicState dynamic_states[] = {
	    VK_DYNAMIC_STATE_VIEWPORT,
	    VK_DYNAMIC_STATE_SCISSOR
	};
	VkPipelineDynamicStateCreateInfo dynamic_state_info {};
	dynamic_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
 	dynamic_state_info.dynamicStateCount = stb_arrcount(dynamic_states);
	dynamic_state_info.pDynamicStates = dynamic_states;

	VkPipelineLayoutCreateInfo pipeline_layout_info {};
	pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipeline_layout_info.setLayoutCount = 0;
	pipeline_layout_info.pSetLayouts = nullptr;
	pipeline_layout_info.pushConstantRangeCount = 0;
	pipeline_layout_info.pPushConstantRanges = nullptr;
	VkPipelineLayout pipeline_layout;
	VK_CHECK(vkCreatePipelineLayout(device, &pipeline_layout_info, nullptr, &pipeline_layout));

	VkAttachmentDescription attachment_description {};
	attachment_description.format = desired_format.format;
	attachment_description.samples = VK_SAMPLE_COUNT_1_BIT;
	attachment_description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachment_description.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachment_description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachment_description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachment_description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachment_description.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;


	VkAttachmentReference attachment_ref {};
	attachment_ref.attachment = 0;
	attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &attachment_ref;

	VkSubpassDependency subpass_dependency {};
	subpass_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	subpass_dependency.dstSubpass = 0;
	subpass_dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subpass_dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subpass_dependency.srcAccessMask = 0;
	subpass_dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	// subpass_dependency.dependencyFlags;

	VkRenderPassCreateInfo render_pass_info {};
	render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	render_pass_info.attachmentCount = 1;
	render_pass_info.pAttachments = &attachment_description;
	render_pass_info.subpassCount = 1;
	render_pass_info.pSubpasses = &subpass;
	render_pass_info.dependencyCount = 1;
	render_pass_info.pDependencies = &subpass_dependency;

	VkRenderPass render_pass;
	VK_CHECK(vkCreateRenderPass(device, &render_pass_info, nullptr, &render_pass));

	VkGraphicsPipelineCreateInfo graphics_pipeline_info {};
	graphics_pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	graphics_pipeline_info.stageCount = 2;
	graphics_pipeline_info.pStages = shader_stages;
	graphics_pipeline_info.pVertexInputState = &vertex_info;
	graphics_pipeline_info.pInputAssemblyState = &assembly_info;
	// const  VkPipelineTessellationStateCreateInfo *  pTessellationState;
	graphics_pipeline_info.pViewportState = &viewport_state;
	graphics_pipeline_info.pRasterizationState = &rasterization_state;
	graphics_pipeline_info.pMultisampleState = &multisample_info;
	// const  VkPipelineDepthStencilStateCreateInfo *  pDepthStencilState;
	graphics_pipeline_info.pColorBlendState = &color_blend_info;
	graphics_pipeline_info.pDynamicState = &dynamic_state_info;
	graphics_pipeline_info.layout = pipeline_layout;
	graphics_pipeline_info.renderPass = render_pass;
	graphics_pipeline_info.subpass = 0;
	graphics_pipeline_info.basePipelineHandle = VK_NULL_HANDLE;
	graphics_pipeline_info.basePipelineIndex = -1;

	VkPipelineCacheCreateInfo pipeline_cache_info;
	pipeline_cache_info.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
	pipeline_cache_info.initialDataSize = 0;
	pipeline_cache_info.pInitialData = NULL;

	VkPipelineCache pipeline_cache;
	VK_CHECK(vkCreatePipelineCache(device, &pipeline_cache_info, nullptr, &pipeline_cache));

	VkPipeline pipeline;
	VK_CHECK(vkCreateGraphicsPipelines(device, pipeline_cache, 1, &graphics_pipeline_info, nullptr, &pipeline));




	VkFramebuffer framebuffers[10];

	VkFramebufferCreateInfo framebuffer_info {};
	framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	framebuffer_info.renderPass = render_pass;
	framebuffer_info.attachmentCount = 1;
	framebuffer_info.layers = 1;
	for (int idx = 0; idx < swapchain_image_count; ++idx) {
		framebuffer_info.pAttachments = &swapchain_image_views[idx];
		framebuffer_info.width = surface_extents.width;
		framebuffer_info.height = surface_extents.height;
		VK_CHECK(vkCreateFramebuffer(device, &framebuffer_info, nullptr, &framebuffers[idx]));
	}


	VkCommandPoolCreateInfo graphics_command_pool_info {};
	graphics_command_pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	graphics_command_pool_info.queueFamilyIndex = graphics_queue_family_idx;
	graphics_command_pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	VkCommandPool graphics_command_pool;
	vkCreateCommandPool(device, &graphics_command_pool_info, nullptr, &graphics_command_pool);

	// VkCommandPoolCreateInfo present_command_pool_info {};
	// present_command_pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	// present_command_pool_info.queueFamilyIndex = present_queue_family_idx;
	// VkCommandPool present_command_pool;
	// vkCreateCommandPool(device, &present_command_pool_info, nullptr, &present_command_pool);

	VkCommandBufferAllocateInfo command_buffer_info {};
	command_buffer_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	command_buffer_info.commandPool = graphics_command_pool;
	command_buffer_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	command_buffer_info.commandBufferCount = 10;

	VkCommandBuffer command_buffers[10];

	vkAllocateCommandBuffers(device, &command_buffer_info, command_buffers);


	VkCommandBufferBeginInfo command_buffer_begin_info {};
	command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	VkRenderPassBeginInfo render_pass_begin_info {};
	render_pass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	render_pass_begin_info.renderPass = render_pass;
	VkClearValue clear_color {0.0f, 0.0f, 0.0f, 1.0f};
	render_pass_begin_info.clearValueCount = 1;
	render_pass_begin_info.pClearValues = &clear_color;

	for (int idx = 0; idx < swapchain_image_count; ++idx) {
		VkCommandBuffer command_buffer = command_buffers[idx];

		VK_CHECK(vkBeginCommandBuffer(command_buffer, &command_buffer_begin_info));

		render_pass_begin_info.framebuffer = framebuffers[idx];
		render_pass_begin_info.renderArea.extent = surface_extents;

		vkCmdBeginRenderPass(command_buffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdSetViewport(command_buffer, 0, 1, &viewport);
		vkCmdSetScissor(command_buffer, 0, 1, &scissor);
		vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
		vkCmdDraw(command_buffer, 3, 1, 0, 0);

		vkCmdEndRenderPass(command_buffer);
		VK_CHECK(vkEndCommandBuffer(command_buffer));
	}

	const int MAX_FRAMES_IN_FLIGHT = 2;

	VkSemaphore image_available_semaphore[MAX_FRAMES_IN_FLIGHT];
	VkSemaphore render_finished_semaphore[MAX_FRAMES_IN_FLIGHT];
	VkFence in_flight_fences[MAX_FRAMES_IN_FLIGHT];

	VkFence image_fences[10] { VK_NULL_HANDLE };

	VkSemaphoreCreateInfo semaphore_info;
	semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fence_info;
	fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (int idx = 0; idx < MAX_FRAMES_IN_FLIGHT; ++idx) {
		VK_CHECK(vkCreateSemaphore(device, &semaphore_info, nullptr, &image_available_semaphore[idx]));
		VK_CHECK(vkCreateSemaphore(device, &semaphore_info, nullptr, &render_finished_semaphore[idx]));

		VK_CHECK(vkCreateFence(device, &fence_info, nullptr, &in_flight_fences[idx]));
	}

	int current_frame = 0;

	while(!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		vkWaitForFences(device, 1, &in_flight_fences[current_frame], VK_TRUE, UINT64_MAX);

		uint32_t image_index;
		vkAcquireNextImageKHR(device, swapchain, UINT64_MAX, image_available_semaphore[current_frame], VK_NULL_HANDLE, &image_index);


		if (image_fences[image_index] != VK_NULL_HANDLE) {
			vkWaitForFences(device, 1, &image_fences[image_index], VK_TRUE, UINT64_MAX);
		}
		image_fences[image_index] = in_flight_fences[current_frame];

		VkPipelineStageFlags wait_stages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		VkSubmitInfo submit_info;
		submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submit_info.waitSemaphoreCount = 1;
		submit_info.pWaitSemaphores = &image_available_semaphore[current_frame];
		submit_info.pWaitDstStageMask = &wait_stages;
		submit_info.commandBufferCount = 1;
		submit_info.pCommandBuffers = &command_buffers[image_index];
		submit_info.signalSemaphoreCount = 1;
		submit_info.pSignalSemaphores = &render_finished_semaphore[current_frame];

		vkResetFences(device, 1, &in_flight_fences[current_frame]);
		VK_CHECK(vkQueueSubmit(graphics_queue, 1, &submit_info, in_flight_fences[current_frame]));

		VkPresentInfoKHR present_info {};
		present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		present_info.waitSemaphoreCount = 1;
		present_info.pWaitSemaphores = &render_finished_semaphore[current_frame];
		present_info.swapchainCount = 1;
		present_info.pSwapchains = &swapchain;
		present_info.pImageIndices = &image_index;
		present_info.pResults = nullptr;

		VkResult present_result = vkQueuePresentKHR(present_queue, &present_info);

		if (present_result != VK_SUCCESS) {
			stbprint("present result is:%d\n", present_result);

			vkDeviceWaitIdle(device);


			// recreate_swapchain();

			int width, height;
			glfwGetFramebufferSize(window, &width, &height);
			// glfwWaitEvents();
			// vkFreeCommandBuffers(device, graphics_command_pool, swapchain_image_count, command_buffers);

			for (int idx = 0; idx < swapchain_image_count; ++idx) {
				vkDestroyFramebuffer(device, framebuffers[idx], nullptr);
				vkDestroyImageView(device, swapchain_image_views[idx], nullptr);
			}

			vkDestroySwapchainKHR(device, swapchain, nullptr);

			surface_extents.width = width;
			surface_extents.height = height;
			swapchain_info.imageExtent = surface_extents;
			VK_CHECK(vkCreateSwapchainKHR(device, &swapchain_info, nullptr, &swapchain));

			vkGetSwapchainImagesKHR(device, swapchain, &swapchain_image_count, nullptr);
			vkGetSwapchainImagesKHR(device, swapchain, &swapchain_image_count, swapchain_images);

			for (int idx = 0; idx < swapchain_image_count; ++idx) {
				image_view_info.image = swapchain_images[idx];
				VK_CHECK(vkCreateImageView(device, &image_view_info, nullptr, &swapchain_image_views[idx]));
			}

			for (int idx = 0; idx < swapchain_image_count; ++idx) {
				framebuffer_info.pAttachments = &swapchain_image_views[idx];
				framebuffer_info.width = surface_extents.width;
				framebuffer_info.height = surface_extents.height;
				VK_CHECK(vkCreateFramebuffer(device, &framebuffer_info, nullptr, &framebuffers[idx]));
			}

			// vkAllocateCommandBuffers(device, &command_buffer_info, command_buffers);

			for (int idx = 0; idx < swapchain_image_count; ++idx) {
				VkCommandBuffer command_buffer = command_buffers[idx];

				VK_CHECK(vkBeginCommandBuffer(command_buffer, &command_buffer_begin_info));

				render_pass_begin_info.framebuffer = framebuffers[idx];
				render_pass_begin_info.renderArea.extent = surface_extents;

				vkCmdBeginRenderPass(command_buffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

				viewport.width = surface_extents.width;
				viewport.height = surface_extents.height;
				scissor.extent = surface_extents;

				vkCmdSetViewport(command_buffer, 0, 1, &viewport);
				vkCmdSetScissor(command_buffer, 0, 1, &scissor);
				vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

				// test if setting viewpor here is different

				vkCmdDraw(command_buffer, 3, 1, 0, 0);

				vkCmdEndRenderPass(command_buffer);
				VK_CHECK(vkEndCommandBuffer(command_buffer));
			}

		}

		current_frame = (current_frame + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	for (int idx = 0; idx < MAX_FRAMES_IN_FLIGHT; ++idx) {
		vkDestroySemaphore(device, image_available_semaphore[idx], nullptr);
		vkDestroySemaphore(device, render_finished_semaphore[idx], nullptr);
		vkDestroyFence(device, in_flight_fences[idx], nullptr);
	}

	vkDestroyCommandPool(device, graphics_command_pool, nullptr);

	vkDestroyShaderModule(device, vertex_shader_module, nullptr);
	vkDestroyShaderModule(device, fragment_shader_module, nullptr);

	for (int idx = 0; idx < swapchain_image_count; ++idx) {
		vkDestroyFramebuffer(device, framebuffers[idx], nullptr);
		vkDestroyImageView(device, swapchain_image_views[idx], nullptr);
	}
	vkDestroyRenderPass(device, render_pass, nullptr);

	// vkDestroyDevice(device);
	//auto func2 = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	//func2(instance, debug_messenger, nullptr);

	vkDestroySwapchainKHR(device, swapchain, nullptr);
	vkDestroySurfaceKHR(instance, surface, nullptr);
	vkDestroyInstance(instance, nullptr);


	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
