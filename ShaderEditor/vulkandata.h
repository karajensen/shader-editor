////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkandata.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "vulkan.h"
#include <vector>

/*
* A layer can expose extensions, keep track of those extensions here
*/
struct LayerProperties
{
    VkLayerProperties properties;
    std::vector<VkExtensionProperties> extensions;
};

/*
* Keep each of our swap chain buffers' image, command buffer and view in one spot
*/
struct SwapChainBuffer 
{
    VkImage image;
    VkImageView view;
};

/**
* Internal data for the vulkan rendering engine
*/
struct VulkanData
{
    /**
    * Constructor
    */
    VulkanData(HINSTANCE hinstance, HWND hwnd);

    /**
    * Destructor
    */
    ~VulkanData();

    /**
    * Releases the device
    */
    void Release();

    HINSTANCE connection = nullptr;
    HWND window = nullptr;

    VkInstance instance = VK_NULL_HANDLE;
    VkDevice device = VK_NULL_HANDLE;
    VkCommandBuffer cmd = VK_NULL_HANDLE;
    VkDebugReportCallbackEXT debug_callback = VK_NULL_HANDLE;
    VkSurfaceKHR surface = VK_NULL_HANDLE;
    VkSwapchainKHR swap_chain = VK_NULL_HANDLE;

    PFN_vkCreateDebugReportCallbackEXT CreateDebugReportFn = nullptr;
    PFN_vkDestroyDebugReportCallbackEXT DestroyDebugReportFn = nullptr;

    VkPhysicalDeviceMemoryProperties memory_properties = {};
    VkPhysicalDeviceProperties gpu_props = {};

    std::vector<SwapChainBuffer> buffers;
    std::vector<VkQueueFamilyProperties> queue_props;
    std::vector<LayerProperties> instance_layer_properties;
    std::vector<const char*> instance_layer_names;
    std::vector<const char*> instance_extension_names;
    std::vector<VkPhysicalDevice> gpus;
    std::vector<const char*> device_extension_names;

    int width, height = 0;
    uint32_t queue_family_count = 0;
    uint32_t graphics_queue_family_index = 0;
    uint32_t present_queue_family_index = 0;
    uint32_t swapchainImageCount = 0;
    VkCommandPool cmd_pool = 0;
    VkFormat format = VK_FORMAT_UNDEFINED;
};