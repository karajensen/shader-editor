////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkan_init.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

struct VulkanData;

class VulkanInit
{
public:
    static bool InitInstance(VulkanData &info);
    static bool InitEnumerateDevice(VulkanData &info);
    static bool InitDevice(VulkanData &info);
    static bool InitDebugging(VulkanData &info);
    static bool InitGlobalLayerProperties(VulkanData& info);
    static bool InitSwapchainExtension(VulkanData &info);
    static bool InitCommandPool(VulkanData &info);
    static bool InitCommandBuffer(VulkanData &info);
    static bool InitDeviceQueue(VulkanData &info);
    static bool InitSwapChain(VulkanData &info);
    static bool InitDepthBuffer(VulkanData &info);
    static bool InitDescriptorAndPipelineLayouts(VulkanData &info);
    static bool InitRenderpass(VulkanData &info);
    static bool InitFramebuffers(VulkanData &info);
    static bool InitDescriptorPool(VulkanData& info);
    static bool InitSemaphores(VulkanData& info);
    static bool InitFence(VulkanData& info);
    static bool InitDescriptorSet(VulkanData& info);
};