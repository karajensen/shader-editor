////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkanbase.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "vulkan.h"
#include <vector>
#include <Windows.h>
#include <boost/noncopyable.hpp>

class VulkanBase : boost::noncopyable
{
public:

    VulkanBase(HINSTANCE hinstance, HWND hwnd);
    ~VulkanBase();
    
    /**
    * Initialises the vulkan data
    */
    bool Initialise();

    /**
    * Releases the vulkan data
    */
    void Release();

    /**
    * @return the vulkan instance
    */
    VkInstance& Instance() const;

    /**
    * @return the vulkan device
    */
    VkInstance& Device() const;

    /**
    * Get memory type for a given memory allocation (flags and bits)
    */
    bool GetMemoryType(uint32_t typeBits, VkFlags properties, uint32_t& typeIndex);

    /**
    * Put an image memory barrier for setting an image layout 
    * on the sub resource into the given command buffer
    */
    void SetImageLayout(VkCommandBuffer cmdbuffer,
                        VkImage image,
                        VkImageAspectFlags aspectMask,
                        VkImageLayout oldImageLayout,
                        VkImageLayout newImageLayout,
                        VkImageSubresourceRange subresourceRange);

    /**
    * Uses a fixed sub resource layout with first mip level and layer
    */
    void SetImageLayout(VkCommandBuffer cmdbuffer,
                        VkImage image,
                        VkImageAspectFlags aspectMask,
                        VkImageLayout oldImageLayout,
                        VkImageLayout newImageLayout);

    /**
    * Vulcan API Function pointers
    */
    PFN_vkGetPhysicalDeviceSurfaceSupportKHR       GetPhysicalDeviceSurfaceSupport      = VK_NULL_HANDLE;
    PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR  GetPhysicalDeviceSurfaceCapabilities = VK_NULL_HANDLE;
    PFN_vkGetPhysicalDeviceSurfaceFormatsKHR       GetPhysicalDeviceSurfaceFormats      = VK_NULL_HANDLE;
    PFN_vkGetPhysicalDeviceSurfacePresentModesKHR  GetPhysicalDeviceSurfacePresentModes = VK_NULL_HANDLE;
    PFN_vkCreateSwapchainKHR                       CreateSwapchain                      = VK_NULL_HANDLE;
    PFN_vkDestroySwapchainKHR                      DestroySwapchain                     = VK_NULL_HANDLE;
    PFN_vkGetSwapchainImagesKHR                    GetSwapchainImages                   = VK_NULL_HANDLE;
    PFN_vkAcquireNextImageKHR                      AcquireNextImage                     = VK_NULL_HANDLE;
    PFN_vkQueuePresentKHR                          QueuePresent                         = VK_NULL_HANDLE;
    PFN_vkCreateDebugReportCallbackEXT             CreateDebugReport                    = VK_NULL_HANDLE;
    PFN_vkDestroyDebugReportCallbackEXT            DestroyDebugReport                   = VK_NULL_HANDLE;
    PFN_vkDebugReportMessageEXT                    DbgBreak                             = VK_NULL_HANDLE;

private:

    /**
    * Initialises parts of the vulkan engine
    */
    bool InitializeFunctionPointers();
    bool InitializeInstance();
    bool InitializeDevice();
    bool InitializeSwapChain();
    bool InitializeDebugging();
    bool InitializeSurface();
    bool InitializeCommands();
    bool InitializeDepthStencil();

    struct SwapChainBuffer
    {
        VkImage Image;
        VkImageView View;
    };

    struct DepthStencil
    {
        VkImage Image;
        VkDeviceMemory Memory;
        VkImageView View;
    };

    HWND m_hwnd = nullptr;                          ///< handle to the window
    HINSTANCE m_hinstance = nullptr;                ///< handle to the current instance of the application

    VkInstance m_instance;
    VkDevice m_device;
    VkPhysicalDevice m_physicalDevice;
    VkSurfaceKHR m_surface;
    VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;
    DepthStencil m_depthStencil;
    uint32_t m_imageCount = 0;
    std::vector<VkImage> m_images;
    std::vector<SwapChainBuffer> m_buffers;
    uint32_t m_queueNodeIndex = UINT32_MAX;
    VkFormat m_colorFormat = VK_FORMAT_UNDEFINED;
    VkFormat m_depthFormat = VK_FORMAT_UNDEFINED;
    VkColorSpaceKHR m_colorSpace;
    VkPhysicalDeviceFeatures m_enabledFeatures = {};
    VkPhysicalDeviceProperties m_deviceProperties;
    VkPhysicalDeviceFeatures m_deviceFeatures;
    VkPhysicalDeviceMemoryProperties m_deviceMemoryProperties;
    VkPipelineStageFlags m_submitPipelineStages = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    VkQueue m_queue;
    bool m_vsync = false;

    VkSemaphore m_presentCompleteSemaphore;
    VkSemaphore m_renderCompleteSemaphore;

    bool m_enableValidation = true;
    bool m_enableWarnings = false;
    int m_validationLayerCount = 1;
    const char* m_validationLayerNames = "VK_LAYER_LUNARG_standard_validation";
    VkDebugReportCallbackEXT m_debugCallback = VK_NULL_HANDLE;

    VkCommandPool m_cmdPool;
    VkCommandBuffer m_setupCmdBuffer = VK_NULL_HANDLE;
    std::vector<VkCommandBuffer> m_drawCmdBuffers;
    std::vector<VkCommandBuffer> m_postPresentCmdBuffers = { VK_NULL_HANDLE };
    std::vector<VkCommandBuffer> m_prePresentCmdBuffers = { VK_NULL_HANDLE };
};