////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkanutils.cpp
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "common.h"
#include "renderdata.h"
#include "vulkan.h"
#include "glslang/SPIRV/GlslangToSpv.h"

struct VulkanData;
struct LayerProperties;

VkResult init_global_extension_properties(LayerProperties& layer_props);
VkResult init_instance(VulkanData &info);
VkResult init_enumerate_device(VulkanData &info, uint32_t gpu_count = 1);
VkResult init_queue_family_index(VulkanData &info);
VkResult init_device(VulkanData &info);
VkResult init_debugging(VulkanData &info);
VkResult init_global_layer_properties(VulkanData& info);
VkResult init_instance_extension_names(VulkanData &info);
VkResult init_device_extension_names(VulkanData &info);
VkResult init_swapchain_extension(VulkanData &info);
VkResult init_command_pool(VulkanData &info);
VkResult init_command_buffer(VulkanData &info);
VkResult init_device_queue(VulkanData &info);
VkResult init_swap_chain(VulkanData &info);
VkResult init_depth_buffer(VulkanData &info);
VkResult init_descriptor_and_pipeline_layouts(VulkanData &info);
VkResult init_renderpass(VulkanData &info);
VkResult init_framebuffers(VulkanData &info);
VkResult init_descriptor_pool(VulkanData& info);
VkResult init_descriptor_set(VulkanData& info);
VkResult init_pipeline_cache(VulkanData& info);
VkResult init_pipeline(VulkanData& info);
VkResult init_viewports(VulkanData& info);
VkResult init_scissors(VulkanData& info);

VkResult execute_begin_command_buffer(VulkanData &info);
void init_shader_resources(TBuiltInResource& resources);
EShLanguage find_language(const VkShaderStageFlagBits shader_type);

bool memory_type_from_properties(VulkanData &info,
                                 uint32_t typeBits,
                                 VkFlags requirements_mask,
                                 uint32_t* typeIndex);

bool glsl_to_spb(const VkShaderStageFlagBits shader_type, 
                 const char* pshader, 
                 std::vector<unsigned int>& spirv);

bool log_fail(VkResult result, const char* file, int line);
#undef FAILED
#define FAILED(result) log_fail(result, __FILE__, __LINE__)

// TODO: Temporary until reorganising engine
VkResult init_uniform_buffer(VulkanData& info);
VkResult init_shaders(VulkanData& info);
VkResult init_vertex_buffer(VulkanData& info);
