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
    graphics_queue = nullptr;
    present_queue = nullptr;

    imageAcquiredSemaphore = VK_NULL_HANDLE;
    surface = VK_NULL_HANDLE;
    debug_callback = VK_NULL_HANDLE;
    cmd = VK_NULL_HANDLE;
    device = VK_NULL_HANDLE;
    instance = VK_NULL_HANDLE;
    swap_chain = VK_NULL_HANDLE;
    imageAcquiredSemaphore = VK_NULL_HANDLE;
    pipeline = VK_NULL_HANDLE;
    pipelineCache = VK_NULL_HANDLE;
    desc_pool = VK_NULL_HANDLE;
    render_pass = VK_NULL_HANDLE;
    pipeline_layout = VK_NULL_HANDLE;

    current_buffer = 0;
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
    vertex_buffer = {};
    vi_binding = {};
    viewport = {};
    scissor = {};

    queue_props.clear();
    instance_layer_properties.clear();
    instance_layer_names.clear();
    instance_extension_names.clear();
    gpus.clear();
    device_extension_names.clear();
    buffers.clear();
    desc_layout.clear();
    framebuffers.clear();
    desc_set.clear();

    vi_attribs.clear();
    vi_attribs.resize(2);

    shaderStages.clear();
    shaderStages.resize(2);

    clear_values.clear();
    clear_values.resize(2);
}

void VulkanData::Release()
{
    if (imageAcquiredSemaphore != VK_NULL_HANDLE)
    {
        vkDestroySemaphore(device, imageAcquiredSemaphore, NULL);
    }

    if (pipeline != VK_NULL_HANDLE)
    {
        vkDestroyPipeline(device, pipeline, NULL);
    }

    if (pipelineCache != VK_NULL_HANDLE)
    {
        vkDestroyPipelineCache(device, pipelineCache, NULL);
    }

    if (desc_pool != VK_NULL_HANDLE)
    {
        vkDestroyDescriptorPool(device, desc_pool, NULL);
    }

    if (vertex_buffer.buffer != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(device, vertex_buffer.buffer, NULL);
    }

    if (vertex_buffer.memory != VK_NULL_HANDLE)
    {
        vkFreeMemory(device, vertex_buffer.memory, NULL);
    }

    for (int i = 0; i < (int)framebuffers.size(); i++)
    {
        vkDestroyFramebuffer(device, framebuffers[i], NULL);
    }

    for (int i = 0; i < (int)shaderStages.size(); i++)
    {
        if (shaderStages[i].module != VK_NULL_HANDLE)
        {
            vkDestroyShaderModule(device, shaderStages[i].module, NULL);
        }
    }

    if (render_pass != VK_NULL_HANDLE)
    {
        vkDestroyRenderPass(device, render_pass, NULL);
    }

    for (int i = 0; i < (int)desc_layout.size(); i++)
    {
        vkDestroyDescriptorSetLayout(device, desc_layout[i], NULL);
    }

    if (pipeline_layout != VK_NULL_HANDLE)
    {
        vkDestroyPipelineLayout(device, pipeline_layout, NULL);
    }

    if (uniform_data.buffer != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(device, uniform_data.buffer, NULL);
    }

    if (uniform_data.memory != VK_NULL_HANDLE)
    {
        vkFreeMemory(device, uniform_data.memory, NULL);
    }

    if (depth.view != VK_NULL_HANDLE)
    {
        vkDestroyImageView(device, depth.view, NULL);
    }

    if (depth.image != VK_NULL_HANDLE)
    {
        vkDestroyImage(device, depth.image, NULL);
    }

    if (depth.memory != VK_NULL_HANDLE)
    {
        vkFreeMemory(device, depth.memory, NULL);
    }

    for (int i = 0; i < (int)buffers.size(); i++)
    {
        vkDestroyImageView(device, buffers[i].view, NULL);
    }

    if (swap_chain != VK_NULL_HANDLE)
    {
        vkDestroySwapchainKHR(device, swap_chain, NULL);
    }

    if (cmd != VK_NULL_HANDLE)
    {
        VkCommandBuffer cmd_bufs[1] = { cmd };
        vkFreeCommandBuffers(device, cmd_pool, 1, cmd_bufs);
        vkDestroyCommandPool(device, cmd_pool, 0);
    }

    if (debug_callback != VK_NULL_HANDLE)
    {
        if (DestroyDebugReportFn != nullptr)
        {
            DestroyDebugReportFn(instance, debug_callback, nullptr);
        }
    }

    if (device != VK_NULL_HANDLE)
    {
        vkDeviceWaitIdle(device);
        vkDestroyDevice(device, NULL);
    }

    if (surface != VK_NULL_HANDLE)
    {
        vkDestroySurfaceKHR(instance, surface, NULL);
    }

    if (instance != VK_NULL_HANDLE)
    {
        vkDestroyInstance(instance, nullptr);
    }

    Reset();
}