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

    VkPhysicalDeviceMemoryProperties memory_properties;
    VkPhysicalDeviceProperties gpu_props;
    std::vector<VkQueueFamilyProperties> queue_props;
    std::vector<LayerProperties> instance_layer_properties;
    std::vector<const char*> instance_layer_names;
    std::vector<const char*> instance_extension_names;
    std::vector<VkPhysicalDevice> gpus;
    uint32_t queue_family_count;
};