////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkandata.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "vulkandata.h"

VulkanData::VulkanData(HINSTANCE hinstance, HWND hwnd)
{
    Release();
}

VulkanData::~VulkanData()
{
    Release();
}

void VulkanData::Release()
{
    queue_family_count = 0;
    graphics_queue_family_index = 0;
    cmd_pool = 0;

    memory_properties = {};
    gpu_props = {};

    queue_props.clear();
    instance_layer_properties.clear();
    instance_layer_names.clear();
    instance_extension_names.clear();
    gpus.clear();
    device_extension_names.clear();

    if (cmd != VK_NULL_HANDLE)
    {
        VkCommandBuffer cmd_bufs[1] = { cmd };
        vkFreeCommandBuffers(device, cmd_pool, 1, cmd_bufs);
        vkDestroyCommandPool(device, cmd_pool, 0);
        cmd = VK_NULL_HANDLE;
    }

    if (device != VK_NULL_HANDLE)
    {
        vkDeviceWaitIdle(device);
        vkDestroyDevice(device, NULL);
        device = VK_NULL_HANDLE;
    }

    if (instance != VK_NULL_HANDLE)
    {
        vkDestroyInstance(instance, nullptr);
        instance = VK_NULL_HANDLE;
    }
}