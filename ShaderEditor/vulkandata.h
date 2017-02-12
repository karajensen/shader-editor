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

    std::vector<LayerProperties> instance_layer_properties;
};