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

    VkInstance instance = VK_NULL_HANDLE;
    VkDevice device = VK_NULL_HANDLE;
    VkCommandBuffer cmd = VK_NULL_HANDLE;
    VkDebugReportCallbackEXT debug_callback = VK_NULL_HANDLE;

    VkPhysicalDeviceMemoryProperties memory_properties;
    VkPhysicalDeviceProperties gpu_props;
    std::vector<VkQueueFamilyProperties> queue_props;
    std::vector<LayerProperties> instance_layer_properties;
    std::vector<const char*> instance_layer_names;
    std::vector<const char*> instance_extension_names;
    std::vector<VkPhysicalDevice> gpus;
    std::vector<const char*> device_extension_names;

    PFN_vkCreateDebugReportCallbackEXT CreateDebugReportFn;
    PFN_vkDestroyDebugReportCallbackEXT DestroyDebugReportFn;

    uint32_t queue_family_count;
    uint32_t graphics_queue_family_index;
    VkCommandPool cmd_pool;
};