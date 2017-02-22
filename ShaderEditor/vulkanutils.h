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

class VulkanUtils
{
public:

    /**
    * Number of descriptor sets needs to be the same at alloc,
    * pipeline layout creation, and descriptor set layout creation
    */
    static const int NUM_DESCRIPTOR_SETS = 1;

    /**
    * Number of viewports and number of scissors have to be the same
    * at pipeline creation and in any call to set them dynamically
    * They also have to be the same as each other
    */
    static const int NUM_VIEWPORTS_AND_SCISSORS = 1;

    /**
    * Number of samples needs to be the same at image creation
    * renderpass creation and pipeline creation.      
    */
    static const VkSampleCountFlagBits NUM_SAMPLES = VK_SAMPLE_COUNT_1_BIT;

    /**
    * Amount of time, in nanoseconds, to wait for a command buffer to complete
    */
    static const int FENCE_TIMEOUT = 100000000;

    static VkResult init_global_extension_properties(LayerProperties& layer_props);
    static VkResult init_instance(VulkanData &info);
    static VkResult init_enumerate_device(VulkanData &info, uint32_t gpu_count = 1);
    static VkResult init_queue_family_index(VulkanData &info);
    static VkResult init_device(VulkanData &info);
    static VkResult init_debugging(VulkanData &info);
    static VkResult init_global_layer_properties(VulkanData& info);
    static VkResult init_instance_extension_names(VulkanData &info);
    static VkResult init_device_extension_names(VulkanData &info);
    static VkResult init_swapchain_extension(VulkanData &info);
    static VkResult init_command_pool(VulkanData &info);
    static VkResult init_command_buffer(VulkanData &info);
    static VkResult init_device_queue(VulkanData &info);
    static VkResult init_swap_chain(VulkanData &info);
    static VkResult init_depth_buffer(VulkanData &info);
    static VkResult init_descriptor_and_pipeline_layouts(VulkanData &info);
    static VkResult init_renderpass(VulkanData &info);
    static VkResult init_framebuffers(VulkanData &info);
    static VkResult init_descriptor_pool(VulkanData& info);
    static VkResult init_descriptor_set(VulkanData& info);
    static VkResult init_pipeline_cache(VulkanData& info);
    static VkResult init_pipeline(VulkanData& info);
    static VkResult init_viewports(VulkanData& info);
    static VkResult init_scissors(VulkanData& info);
    static VkResult init_semaphores(VulkanData& info);
    static VkResult init_fence(VulkanData& info);

    static void begin_command_buffer(VulkanData& info);
    static void end_command_buffer(VulkanData& info);
    static bool log_fail(VkResult result, const char* file, int line);

    // TODO: Temporary until reorganising engine
    static VkResult init_uniform_buffer(VulkanData& info);
    static VkResult init_shaders(VulkanData& info);
    static VkResult init_vertex_buffer(VulkanData& info);
};

#undef FAILED
#define FAILED(result) VulkanUtils::log_fail(result, __FILE__, __LINE__)