////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkanbase.h
// Vulkan code Reference: https://github.com/SaschaWillems/Vulkan
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
    * Renders the scene
    */
    void Render();

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

private:

    /**
    * Reset all values to their defaults
    */
    void Reset();

    /**
    * Vulcan API Function pointers
    */
    PFN_vkGetPhysicalDeviceSurfaceSupportKHR GetPhysicalDeviceSurfaceSupport;
    PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR GetPhysicalDeviceSurfaceCapabilities;
    PFN_vkGetPhysicalDeviceSurfaceFormatsKHR GetPhysicalDeviceSurfaceFormats;
    PFN_vkGetPhysicalDeviceSurfacePresentModesKHR GetPhysicalDeviceSurfacePresentModes;
    PFN_vkCreateSwapchainKHR CreateSwapchain;
    PFN_vkDestroySwapchainKHR DestroySwapchain;
    PFN_vkGetSwapchainImagesKHR GetSwapchainImages;
    PFN_vkAcquireNextImageKHR AcquireNextImage;
    PFN_vkQueuePresentKHR QueuePresent;
    PFN_vkCreateDebugReportCallbackEXT CreateDebugReport;
    PFN_vkDestroyDebugReportCallbackEXT DestroyDebugReport;
    PFN_vkDebugReportMessageEXT DbgBreak;

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
    bool InitializeRenderPass();
    bool InitializePipelineCache();
    bool InitializeFrameBuffers();
    bool InitializeDescriptorSetLayout();
    bool InitializeDescriptorPool();
    bool BuildCommandBuffers();

    struct SwapChainBuffer
    {
        VkImage Image;
        VkImageView View;
    };

    HWND m_hwnd = nullptr;            ///< handle to the window
    HINSTANCE m_hinstance = nullptr;  ///< handle to the current instance of the application

    VkInstance m_instance;
    VkSurfaceKHR m_surface;
    VkRenderPass m_renderPass;
    uint32_t m_queueNodeIndex;
    VkQueue m_queue;

    VkDevice m_device;
    VkPhysicalDevice m_physicalDevice;
    VkPhysicalDeviceProperties m_deviceProperties;
    VkPhysicalDeviceFeatures m_deviceFeatures;
    VkPhysicalDeviceFeatures m_deviceCreateFeatures;
    VkPhysicalDeviceMemoryProperties m_deviceMemoryProperties;

    VkSwapchainKHR m_swapChain;
    uint32_t m_swapChainImageCount;
    std::vector<VkImage> m_swapChainImages;
    std::vector<SwapChainBuffer> m_swapChainBuffers;

    VkImage m_depthStencilImage;
    VkDeviceMemory m_depthStencilMemory;
    VkImageView m_depthStencilView;

    std::vector<VkFramebuffer> m_frameBuffers;
    uint32_t m_currentBuffer;
    VkFormat m_colorFormat;
    VkFormat m_depthFormat;
    VkColorSpaceKHR m_colorSpace;
    bool m_vsync;

    VkSemaphore m_presentCompleteSemaphore;
    VkSemaphore m_renderCompleteSemaphore;

    VkDescriptorSet m_descriptorSet;
    VkDescriptorSetLayout m_descriptorSetLayout;
    VkDescriptorPool m_descriptorPool;

    VkPipeline m_pipeline;
    VkPipelineLayout m_pipelineLayout;
    VkPipelineCache m_pipelineCache;
    VkPipelineStageFlags m_submitPipelineStages;

    bool m_enableValidation;
    bool m_enableWarnings;
    int m_validationLayerCount;
    const char* m_validationLayerNames;
    VkDebugReportCallbackEXT m_debugCallback;

    VkCommandPool m_cmdPool;
    VkCommandBuffer m_setupCmdBuffer;
    std::vector<VkCommandBuffer> m_drawCmdBuffers;
    std::vector<VkCommandBuffer> m_postPresentCmdBuffers;
    std::vector<VkCommandBuffer> m_prePresentCmdBuffers;
};