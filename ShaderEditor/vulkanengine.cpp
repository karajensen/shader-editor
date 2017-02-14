////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkanengine.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "vulkanengine.h"
#include "vulkandata.h"
#include "vulkanutils.h"
#include "sceneInterface.h"
#include "mesh.h"
#include "water.h"
#include "shader.h"
#include "texture.h"
#include "textureProcedural.h"
#include "postprocessing.h"
#include "emitter.h"
#include "terrain.h"
#include "light.h"
#include <array>
#include <fstream>

VulkanEngine::VulkanEngine(HWND hwnd, HINSTANCE hinstance) :
    m_data(new VulkanData(hinstance, hwnd))
{
}

VulkanEngine::~VulkanEngine()
{
    Release();
}

void VulkanEngine::Release()
{
    m_data->Release();
}

bool VulkanEngine::Initialize()
{
    auto& info = *m_data;

    if (FAILED(init_global_layer_properties(info)) ||
        FAILED(init_instance_extension_names(info)) ||
        FAILED(init_device_extension_names(info)) ||
        FAILED(init_instance(info)) ||
        FAILED(init_debugging(info)) ||
        FAILED(init_enumerate_device(info)) ||
        FAILED(init_window_size(info)))
    {
        return false;
    }

    VkWin32SurfaceCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.pNext = NULL;
    createInfo.hinstance = info.connection;
    createInfo.hwnd = info.window;
    VkResult res = vkCreateWin32SurfaceKHR(info.instance, &createInfo, NULL, &info.surface);

    assert(res == VK_SUCCESS);

    // Iterate over each queue to learn whether it supports presenting:
    VkBool32 *pSupportsPresent = (VkBool32 *)malloc(info.queue_family_count * sizeof(VkBool32));
    for (uint32_t i = 0; i < info.queue_family_count; i++) {
        vkGetPhysicalDeviceSurfaceSupportKHR(info.gpus[0], i, info.surface, &pSupportsPresent[i]);
    }

    // Search for a graphics and a present queue in the array of queue
    // families, try to find one that supports both
    info.graphics_queue_family_index = UINT32_MAX;
    info.present_queue_family_index = UINT32_MAX;
    for (uint32_t i = 0; i < info.queue_family_count; ++i) {
        if ((info.queue_props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) {
            if (info.graphics_queue_family_index == UINT32_MAX) info.graphics_queue_family_index = i;

            if (pSupportsPresent[i] == VK_TRUE) {
                info.graphics_queue_family_index = i;
                info.present_queue_family_index = i;
                break;
            }
        }
    }

    if (info.present_queue_family_index == UINT32_MAX) {
        // If didn't find a queue that supports both graphics and present, then
        // find a separate present queue.
        for (size_t i = 0; i < info.queue_family_count; ++i)
            if (pSupportsPresent[i] == VK_TRUE) {
                info.present_queue_family_index = i;
                break;
            }
    }
    free(pSupportsPresent);

    // Generate error if could not find queues that support graphics
    // and present
    if (info.graphics_queue_family_index == UINT32_MAX || info.present_queue_family_index == UINT32_MAX) 
    {
        Logger::LogError("Could not find a queues for graphics and present");
        return false;
    }

    if (FAILED(init_device(info)))
    {
        return false;
    }

    // Get the list of VkFormats that are supported:
    uint32_t formatCount;
    res = vkGetPhysicalDeviceSurfaceFormatsKHR(info.gpus[0], info.surface, &formatCount, NULL);
    assert(res == VK_SUCCESS);
    VkSurfaceFormatKHR *surfFormats = (VkSurfaceFormatKHR *)malloc(formatCount * sizeof(VkSurfaceFormatKHR));
    res = vkGetPhysicalDeviceSurfaceFormatsKHR(info.gpus[0], info.surface, &formatCount, surfFormats);
    assert(res == VK_SUCCESS);
    // If the format list includes just one entry of VK_FORMAT_UNDEFINED,
    // the surface has no preferred format.  Otherwise, at least one
    // supported format will be returned.
    if (formatCount == 1 && surfFormats[0].format == VK_FORMAT_UNDEFINED) {
        info.format = VK_FORMAT_B8G8R8A8_UNORM;
    }
    else {
        assert(formatCount >= 1);
        info.format = surfFormats[0].format;
    }
    free(surfFormats);

    VkSurfaceCapabilitiesKHR surfCapabilities;

    res = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(info.gpus[0], info.surface, &surfCapabilities);
    assert(res == VK_SUCCESS);

    uint32_t presentModeCount;
    res = vkGetPhysicalDeviceSurfacePresentModesKHR(info.gpus[0], info.surface, &presentModeCount, NULL);
    assert(res == VK_SUCCESS);
    VkPresentModeKHR *presentModes = (VkPresentModeKHR *)malloc(presentModeCount * sizeof(VkPresentModeKHR));

    res = vkGetPhysicalDeviceSurfacePresentModesKHR(info.gpus[0], info.surface, &presentModeCount, presentModes);
    assert(res == VK_SUCCESS);

    VkExtent2D swapchainExtent;
    // width and height are either both 0xFFFFFFFF, or both not 0xFFFFFFFF.
    if (surfCapabilities.currentExtent.width == 0xFFFFFFFF) {
        // If the surface size is undefined, the size is set to
        // the size of the images requested.
        swapchainExtent.width = info.width;
        swapchainExtent.height = info.height;
        if (swapchainExtent.width < surfCapabilities.minImageExtent.width) {
            swapchainExtent.width = surfCapabilities.minImageExtent.width;
        }
        else if (swapchainExtent.width > surfCapabilities.maxImageExtent.width) {
            swapchainExtent.width = surfCapabilities.maxImageExtent.width;
        }

        if (swapchainExtent.height < surfCapabilities.minImageExtent.height) {
            swapchainExtent.height = surfCapabilities.minImageExtent.height;
        }
        else if (swapchainExtent.height > surfCapabilities.maxImageExtent.height) {
            swapchainExtent.height = surfCapabilities.maxImageExtent.height;
        }
    }
    else {
        // If the surface size is defined, the swap chain size must match
        swapchainExtent = surfCapabilities.currentExtent;
    }

    // The FIFO present mode is guaranteed by the spec to be supported
    VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;

    // Determine the number of VkImage's to use in the swap chain.
    // We need to acquire only 1 presentable image at at time.
    // Asking for minImageCount images ensures that we can acquire
    // 1 presentable image as long as we present it before attempting
    // to acquire another.
    uint32_t desiredNumberOfSwapChainImages = surfCapabilities.minImageCount;

    VkSurfaceTransformFlagBitsKHR preTransform;
    if (surfCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
        preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    }
    else {
        preTransform = surfCapabilities.currentTransform;
    }

    VkSwapchainCreateInfoKHR swapchain_ci = {};
    swapchain_ci.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchain_ci.pNext = NULL;
    swapchain_ci.surface = info.surface;
    swapchain_ci.minImageCount = desiredNumberOfSwapChainImages;
    swapchain_ci.imageFormat = info.format;
    swapchain_ci.imageExtent.width = swapchainExtent.width;
    swapchain_ci.imageExtent.height = swapchainExtent.height;
    swapchain_ci.preTransform = preTransform;
    swapchain_ci.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchain_ci.imageArrayLayers = 1;
    swapchain_ci.presentMode = swapchainPresentMode;
    swapchain_ci.oldSwapchain = VK_NULL_HANDLE;
    swapchain_ci.clipped = true;
    swapchain_ci.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
    swapchain_ci.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchain_ci.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapchain_ci.queueFamilyIndexCount = 0;
    swapchain_ci.pQueueFamilyIndices = NULL;
    uint32_t queueFamilyIndices[2] = { (uint32_t)info.graphics_queue_family_index, (uint32_t)info.present_queue_family_index };
    if (info.graphics_queue_family_index != info.present_queue_family_index) {
        // If the graphics and present queues are from different queue families,
        // we either have to explicitly transfer ownership of images between
        // the queues, or we have to create the swapchain with imageSharingMode
        // as VK_SHARING_MODE_CONCURRENT
        swapchain_ci.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapchain_ci.queueFamilyIndexCount = 2;
        swapchain_ci.pQueueFamilyIndices = queueFamilyIndices;
    }

    res = vkCreateSwapchainKHR(info.device, &swapchain_ci, NULL, &info.swap_chain);
    assert(res == VK_SUCCESS);

    res = vkGetSwapchainImagesKHR(info.device, info.swap_chain, &info.swapchainImageCount, NULL);
    assert(res == VK_SUCCESS);

    VkImage *swapchainImages = (VkImage *)malloc(info.swapchainImageCount * sizeof(VkImage));
    assert(swapchainImages);
    res = vkGetSwapchainImagesKHR(info.device, info.swap_chain, &info.swapchainImageCount, swapchainImages);
    assert(res == VK_SUCCESS);

    info.buffers.resize(info.swapchainImageCount);
    for (uint32_t i = 0; i < info.swapchainImageCount; i++) {
        info.buffers[i].image = swapchainImages[i];
    }
    free(swapchainImages);

    for (uint32_t i = 0; i < info.swapchainImageCount; i++) {
        VkImageViewCreateInfo color_image_view = {};
        color_image_view.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        color_image_view.pNext = NULL;
        color_image_view.flags = 0;
        color_image_view.image = info.buffers[i].image;
        color_image_view.viewType = VK_IMAGE_VIEW_TYPE_2D;
        color_image_view.format = info.format;
        color_image_view.components.r = VK_COMPONENT_SWIZZLE_R;
        color_image_view.components.g = VK_COMPONENT_SWIZZLE_G;
        color_image_view.components.b = VK_COMPONENT_SWIZZLE_B;
        color_image_view.components.a = VK_COMPONENT_SWIZZLE_A;
        color_image_view.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        color_image_view.subresourceRange.baseMipLevel = 0;
        color_image_view.subresourceRange.levelCount = 1;
        color_image_view.subresourceRange.baseArrayLayer = 0;
        color_image_view.subresourceRange.layerCount = 1;

        res = vkCreateImageView(info.device, &color_image_view, NULL, &info.buffers[i].view);
        assert(res == VK_SUCCESS);
    }

    return true;
}

std::string VulkanEngine::CompileShader(int index)
{
    return "";
}

bool VulkanEngine::InitialiseScene(const IScene& scene)
{
    return ReInitialiseScene();
}

bool VulkanEngine::ReInitialiseScene()
{
    Logger::LogInfo("Vulkan: Re-Initialised");
    return true;
}

bool VulkanEngine::FadeView(bool in, float amount)
{
    return true;
}

void VulkanEngine::Render(const IScene& scene, float timer)
{
}

void VulkanEngine::ToggleWireframe()
{

}

std::string VulkanEngine::GetName() const
{
    return "Vulkan";
}

void VulkanEngine::UpdateView(const Matrix& world)
{
}

std::string VulkanEngine::GetShaderText(int index) const
{
    return "";
}

std::string VulkanEngine::GetShaderAssembly(int index)
{
    return "";
}

void VulkanEngine::SetFade(float value)
{
    
}

void VulkanEngine::ReloadTerrain(int index)
{
}

void VulkanEngine::ReloadTexture(int index)
{
}

void VulkanEngine::WriteToShader(const Shader& shader,
                                 const std::string& text)
{
}