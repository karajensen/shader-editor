////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkaninit.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "vulkan.h"

struct VulkanData;

class VulkanInit
{
public:
    static VkResult InitInstance(VulkanData &info);
    static VkResult InitEnumerateDevice(VulkanData &info);
    static VkResult InitDevice(VulkanData &info);
    static VkResult InitDebugging(VulkanData &info);
    static VkResult InitGlobalLayerProperties(VulkanData& info);
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
    static VkResult InitSemaphores(VulkanData& info);
    static VkResult InitFence(VulkanData& info);
};