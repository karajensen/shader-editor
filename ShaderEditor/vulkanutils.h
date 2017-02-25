////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkanutils.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "common.h"
#include "renderdata.h"
#include "vulkan.h"

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

    /**
    * Generic Vulkan initualisation error
    */
    static const VkResult VK_ERROR = VK_ERROR_INITIALIZATION_FAILED;

    /**
    * Initialise the Vulkan Engine
    */
    static VkResult InitInstance(VulkanData &info);
    static VkResult InitEnumerateDevice(VulkanData &info);
    static VkResult InitDevice(VulkanData &info);
    static VkResult InitDebugging(VulkanData &info);
    static VkResult InitGlobalLayerProperties(VulkanData& info);
    static VkResult InitExtensionNames(VulkanData &info);
    static VkResult InitSwapchainExtension(VulkanData &info);
    static VkResult InitCommandPool(VulkanData &info);
    static VkResult InitCommandBuffer(VulkanData &info);
    static VkResult InitDeviceQueue(VulkanData &info);
    static VkResult InitSwapChain(VulkanData &info);
    static VkResult InitDepthBuffer(VulkanData &info);
    static VkResult InitDescriptorAndPipelineLayouts(VulkanData &info);
    static VkResult InitRenderpass(VulkanData &info);
    static VkResult InitFramebuffers(VulkanData &info);
    static VkResult InitDescriptorPool(VulkanData& info);
    static VkResult InitDescriptorSet(VulkanData& info);
    static VkResult InitPipeline(VulkanData& info);
    static VkResult InitViewports(VulkanData& info);
    static VkResult InitScissors(VulkanData& info);
    static VkResult InitSemaphores(VulkanData& info);
    static VkResult InitFence(VulkanData& info);

    // TODO: Temporary until reorganising engine
    static VkResult InitUniformBuffer(VulkanData& info);
    static VkResult InitVertexBuffer(VulkanData& info);

    /**
    * Start recording a command buffer
    */
    static void BeginCommandBuffer(VulkanData& info);

    /**
    * End recording a command buffer
    */
    static void EndCommandBuffer(VulkanData& info);

    /**
    * @return whether the given result indicates failure
    */
    static bool Failed(VkResult result);

    /**
    * Logs the given result if a failure
    * @return whether the given result indicates failure
    */
    static bool LogFail(VkResult result, const char* file, int line);

    /**
    * @return a text description of the failure or empty if a success
    */
    static const char* GetFailText(VkResult result);
};

#undef FAILED
#define FAILED(result) VulkanUtils::LogFail(result, __FILE__, __LINE__)