////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkandata.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "vulkandata.h"
#include "renderdata.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

VulkanData::VulkanData(HINSTANCE hinstance, HWND hwnd)
    : connection(hinstance)
    , window(hwnd)
{
    Reset();

    projection = glm::perspective(FIELD_OF_VIEW,
        RATIO, FRUSTRUM_NEAR, FRUSTRUM_FAR);
}

VulkanData::~VulkanData()
{
    Release();
}

void VulkanData::Reset()
{
    DestroyDebugReportFn = nullptr;
    CreateDebugReportFn = nullptr;

    surface = VK_NULL_HANDLE;
    debug_callback = VK_NULL_HANDLE;
    cmd = VK_NULL_HANDLE;
    device = VK_NULL_HANDLE;
    instance = VK_NULL_HANDLE;
    swap_chain = VK_NULL_HANDLE;

    swapchainImageCount = 0;
    queue_family_count = 0;
    present_queue_family_index = 0;
    graphics_queue_family_index = 0;
    cmd_pool = 0;

    format = VK_FORMAT_UNDEFINED;

    memory_properties = {};
    gpu_props = {};
    depth = {};
    uniform_data = {};

    queue_props.clear();
    instance_layer_properties.clear();
    instance_layer_names.clear();
    instance_extension_names.clear();
    gpus.clear();
    device_extension_names.clear();
    buffers.clear();
}

void VulkanData::Release()
{
    if (uniform_data.buffer != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(device, uniform_data.buffer, NULL);
    }

    if (uniform_data.memory != VK_NULL_HANDLE)
    {
        vkFreeMemory(device, uniform_data.memory, NULL);
    }

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

    Reset();
}