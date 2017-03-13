////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkan_init.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

struct VulkanData;

class VulkanInit
{
public:

    /**
    * Initialise the vulkan instance
    * @param info Data for the Vulkan Engine
    */
    static bool InitInstance(VulkanData &info);

    /**
    * Enumerates the physical devices accessible to a Vulkan instance
    * @param info Data for the Vulkan Engine
    */
    static bool InitEnumerateDevice(VulkanData &info);

    /**
    * Initialise the vulkan device
    * @param info Data for the Vulkan Engine
    */
    static bool InitDevice(VulkanData &info);

    /**
    * Initialise the debug validation layer
    * @param info Data for the Vulkan Engine
    */
    static bool InitDebugging(VulkanData &info);

    /**
    * Initialise the available layers properties
    * @param info Data for the Vulkan Engine
    */
    static bool InitGlobalLayerProperties(VulkanData& info);

    /**
    * Initialises the vulkan swapchain layer
    * @param info Data for the Vulkan Engine
    */
    static bool InitSwapchainExtension(VulkanData &info);

    /**
    * Initialises a command pool to allocate command buffers
    * @param info Data for the Vulkan Engine
    */
    static bool InitCommandPool(VulkanData &info);

    /**
    * Initialises the command buffers to record render information
    * @param info Data for the Vulkan Engine
    */
    static bool InitCommandBuffer(VulkanData &info);

    /**
    * Gets a queue handle from a device
    * @param info Data for the Vulkan Engine
    */
    static bool InitDeviceQueue(VulkanData &info);

    /**
    * Initialises a swapchain- 
    * a queue of images that are waiting to be presented to the screen
    * @param info Data for the Vulkan Engine
    */
    static bool InitSwapChain(VulkanData &info);

    /** 
    * Initialises the depth-stencil buffer
    * @param info Data for the Vulkan Engine
    */
    static bool InitDepthBuffer(VulkanData &info);

    /**
    * Initialises the render pass and framebuffers
    * @param info Data for the Vulkan Engine
    */
    static bool InitRenderPass(VulkanData &info);

    /**
    * Creates the semaphores required for rendering multiple frames
    * @param info Data for the Vulkan Engine
    */
    static bool InitSemaphores(VulkanData& info);

    /**
    * Initialises the render fence. A fence becomes signaled when 
    * it is submitted to a queue with a call to vkQueueSubmit
    * @param info Data for the Vulkan Engine
    */
    static bool InitFence(VulkanData& info);
};