////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkandata.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "vulkandata.h"

VulkanData::VulkanData(HINSTANCE hinstance, HWND hwnd)
{

}

VulkanData::~VulkanData()
{
    Release();
}

void VulkanData::Release()
{
    instance_layer_properties.clear();
    instance_layer_names.clear();
    instance_extension_names.clear();
    gpus.clear();

    if (instance != VK_NULL_HANDLE)
    {
        vkDestroyInstance(instance, nullptr);
        instance = VK_NULL_HANDLE;
    }
}