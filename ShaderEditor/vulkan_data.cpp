////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkan_data.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "vulkan_data.h"
#include "vulkan_mesh.h"
#include "vulkan_shader.h"
#include "render_data.h"
#include "common.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

VulkanData::VulkanData(HINSTANCE hinstance, HWND hwnd)
    : connection(hinstance)
    , window(hwnd)
{
    Reset();

    projection = glm::perspective(FIELD_OF_VIEW,
        RATIO, FRUSTRUM_NEAR, FRUSTRUM_FAR);

    clearValues[0].color = { { 0.0f, 0.0f, 1.0f, 1.0f } };
    clearValues[1].depthStencil = { 1.0f, 0 };
}

VulkanData::~VulkanData()
{
    Release();
}

void VulkanData::Reset()
{
    destroyDebugReportFn = nullptr;
    createDebugReportFn = nullptr;
    setDebugNameFn = nullptr;
    graphicsQueue = nullptr;
    presentQueue = nullptr;

    presentCompleteSemaphore = VK_NULL_HANDLE;
    renderCompleteSemaphore = VK_NULL_HANDLE;
    surface = VK_NULL_HANDLE;
    debugCallback = VK_NULL_HANDLE;
    device = VK_NULL_HANDLE;
    instance = VK_NULL_HANDLE;
    swapChain = VK_NULL_HANDLE;
    renderPass = VK_NULL_HANDLE;
    cmdPool = VK_NULL_HANDLE;

    currentBuffer = 0;
    swapchainImageCount = 0;
    queueFamilyCount = 0;
    presentQueueFamilyIndex = 0;
    graphicsQueueFamilyIndex = 0;
    format = VK_FORMAT_UNDEFINED;
    pipeStageFlags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    selectedShader = NO_INDEX;

    memoryProperties = {};
    gpuProps = {};
    depth = {};

    queueProps.clear();
    instanceLayerProperties.clear();
    instanceLayerNames.clear();
    instanceExtensionNames.clear();
    gpus.clear();
    deviceExtensionNames.clear();
    buffers.clear();
    framebuffers.clear();
    cmd.clear();
    fences.clear();
}

void VulkanData::Release()
{
    for (auto& shader : shaders)
    {
        shader->Release();
    }

    for (auto& mesh : meshes)
    {
        mesh->Release();
    }

    for (int i = 0; i < (int)fences.size(); i++)
    {
        vkDestroyFence(device, fences[i], NULL);
    }

    if (presentCompleteSemaphore != VK_NULL_HANDLE)
    {
        vkDestroySemaphore(device, presentCompleteSemaphore, NULL);
    }

    if (renderCompleteSemaphore != VK_NULL_HANDLE)
    {
        vkDestroySemaphore(device, renderCompleteSemaphore, NULL);
    }

    for (int i = 0; i < (int)framebuffers.size(); i++)
    {
        vkDestroyFramebuffer(device, framebuffers[i], NULL);
    }

    if (renderPass != VK_NULL_HANDLE)
    {
        vkDestroyRenderPass(device, renderPass, NULL);
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

    if (cmdPool != VK_NULL_HANDLE)
    {
        if (!cmd.empty())
        {
            vkFreeCommandBuffers(device, cmdPool, cmd.size(), &cmd[0]);
        }
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

VkShader& VulkanData::SelectedShader()
{
    return *shaders.at(selectedShader);
}

VkCommandBuffer& VulkanData::SelectedCmd()
{
    return cmd.at(currentBuffer);
}