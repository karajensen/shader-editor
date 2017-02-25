////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkandata.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "vulkandata.h"
#include "vulkanshader.h"
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
    destroyDebugReportFn = nullptr;
    createDebugReportFn = nullptr;
    graphicsQueue = nullptr;
    presentQueue = nullptr;

    imageAcquiredSemaphore = VK_NULL_HANDLE;
    surface = VK_NULL_HANDLE;
    debugCallback = VK_NULL_HANDLE;
    cmd = VK_NULL_HANDLE;
    device = VK_NULL_HANDLE;
    instance = VK_NULL_HANDLE;
    swapChain = VK_NULL_HANDLE;
    imageAcquiredSemaphore = VK_NULL_HANDLE;
    pipeline = VK_NULL_HANDLE;
    pipelineCache = VK_NULL_HANDLE;
    descPool = VK_NULL_HANDLE;
    renderPass = VK_NULL_HANDLE;
    pipelineLayout = VK_NULL_HANDLE;

    currentBuffer = 0;
    swapchainImageCount = 0;
    queueFamilyCount = 0;
    presentQueueFamilyIndex = 0;
    graphicsQueueFamilyIndex = 0;
    cmdPool = 0;
    format = VK_FORMAT_UNDEFINED;

    memoryProperties = {};
    gpuProps = {};
    depth = {};
    uniformData = {};
    vertexBuffer = {};
    viBinding = {};
    viewport = {};
    scissor = {};

    queueProps.clear();
    instanceLayerProperties.clear();
    instanceLayerNames.clear();
    instanceExtensionNames.clear();
    gpus.clear();
    deviceExtensionNames.clear();
    buffers.clear();
    descLayout.clear();
    framebuffers.clear();
    descSet.clear();

    viAttribs.clear();
    viAttribs.resize(2);

    shaderStages.clear();
    shaderStages.resize(2);

    clearValues.clear();
    clearValues.resize(2);
}

void VulkanData::Release()
{
    for (auto& shader : shaders)
    {
        shader->Release();
    }

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

    if (descPool != VK_NULL_HANDLE)
    {
        vkDestroyDescriptorPool(device, descPool, NULL);
    }

    if (vertexBuffer.buffer != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(device, vertexBuffer.buffer, NULL);
    }

    if (vertexBuffer.memory != VK_NULL_HANDLE)
    {
        vkFreeMemory(device, vertexBuffer.memory, NULL);
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

    if (renderPass != VK_NULL_HANDLE)
    {
        vkDestroyRenderPass(device, renderPass, NULL);
    }

    for (int i = 0; i < (int)descLayout.size(); i++)
    {
        vkDestroyDescriptorSetLayout(device, descLayout[i], NULL);
    }

    if (pipelineLayout != VK_NULL_HANDLE)
    {
        vkDestroyPipelineLayout(device, pipelineLayout, NULL);
    }

    if (uniformData.buffer != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(device, uniformData.buffer, NULL);
    }

    if (uniformData.memory != VK_NULL_HANDLE)
    {
        vkFreeMemory(device, uniformData.memory, NULL);
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

    if (swapChain != VK_NULL_HANDLE)
    {
        vkDestroySwapchainKHR(device, swapChain, NULL);
    }

    if (cmd != VK_NULL_HANDLE)
    {
        VkCommandBuffer cmd_bufs[1] = { cmd };
        vkFreeCommandBuffers(device, cmdPool, 1, cmd_bufs);
        vkDestroyCommandPool(device, cmdPool, 0);
    }

    if (debugCallback != VK_NULL_HANDLE)
    {
        if (destroyDebugReportFn != nullptr)
        {
            destroyDebugReportFn(instance, debugCallback, nullptr);
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