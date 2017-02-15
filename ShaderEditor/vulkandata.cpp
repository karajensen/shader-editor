////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkandata.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "vulkandata.h"

VulkanData::VulkanData(HINSTANCE hinstance, HWND hwnd)
    : connection(hinstance)
    , window(hwnd)
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
    present_queue_family_index = 0;
    graphics_queue_family_index = 0;
    cmd_pool = 0;
    height = 0;
    width = 0;
    format = VK_FORMAT_UNDEFINED;

    memory_properties = {};
    gpu_props = {};
    depth = {};

    queue_props.clear();
    instance_layer_properties.clear();
    instance_layer_names.clear();
    instance_extension_names.clear();
    gpus.clear();
    device_extension_names.clear();

    if (debug_callback != VK_NULL_HANDLE)
    {
        if (DestroyDebugReportFn != nullptr)
        {
            DestroyDebugReportFn(instance, debug_callback, nullptr);
        }
    }

    if (cmd != VK_NULL_HANDLE)
    {
        VkCommandBuffer cmd_bufs[1] = { cmd };
        vkFreeCommandBuffers(device, cmd_pool, 1, cmd_bufs);
        vkDestroyCommandPool(device, cmd_pool, 0);
    }

    for (uint32_t i = 0; i < swapchainImageCount; i++) 
    {
        vkDestroyImageView(device, buffers[i].view, NULL);
    }
    swapchainImageCount = 0;
    buffers.clear();

    if (swap_chain != VK_NULL_HANDLE)
    {
        vkDestroySwapchainKHR(device, swap_chain, NULL);
    }

    if (device != VK_NULL_HANDLE)
    {
        vkDeviceWaitIdle(device);
        vkDestroyDevice(device, NULL);
    }

    if (instance != VK_NULL_HANDLE)
    {
        vkDestroyInstance(instance, nullptr);
    }

    DestroyDebugReportFn = nullptr;
    CreateDebugReportFn = nullptr;
    surface = VK_NULL_HANDLE;
    debug_callback = VK_NULL_HANDLE;
    cmd = VK_NULL_HANDLE;
    device = VK_NULL_HANDLE;
    instance = VK_NULL_HANDLE;
    swap_chain = VK_NULL_HANDLE;
}