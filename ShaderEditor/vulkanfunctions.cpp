////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkanfunctions.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "vulkanfunctions.h"
#include "logger.h"

template<typename T> bool GetProcAddress(VkInstance& instance, std::string name, T& fn)
{
    void* address = vkGetInstanceProcAddr(instance, name.c_str());
    if (address == nullptr)
    {
        Logger::LogError("Vulkan: Could not get instance address for " + name);
        return false;
    }
    fn = static_cast<T>(address);
    return true;
}

template<typename T> bool GetProcAddress(VkDevice& device, std::string name, T& fn)
{
    void* address = vkGetDeviceProcAddr(device, name.c_str());
    if (address == nullptr)
    {
        Logger::LogError("Vulkan: Could not get device address for " + name);
        return false;
    }
    fn = static_cast<T>(address);
    return true;
}

bool VulkanFunctions::Initialise(VkInstance& instance, VkDevice& device)
{
    return GetProcAddress(instance, "vkCreateDebugReportCallbackEXT", CreateDebugReport) &&
           GetProcAddress(instance, "vkDestroyDebugReportCallbackEXT", DestroyDebugReport) &&
           GetProcAddress(instance, "vkDebugReportMessageEXT", DbgBreak) &&
           GetProcAddress(instance, "vkGetPhysicalDeviceSurfaceSupportKHR", GetPhysicalDeviceSurfaceSupport) &&
           GetProcAddress(instance, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR", GetPhysicalDeviceSurfaceCapabilities) &&
           GetProcAddress(instance, "vkGetPhysicalDeviceSurfaceFormatsKHR", GetPhysicalDeviceSurfaceFormats) &&
           GetProcAddress(instance, "vkGetPhysicalDeviceSurfacePresentModesKHR", GetPhysicalDeviceSurfacePresentModes) &&
           GetProcAddress(device, "vkCreateSwapchainKHR", CreateSwapchain) &&
           GetProcAddress(device, "vkDestroySwapchainKHR", DestroySwapchain) &&
           GetProcAddress(device, "vkGetSwapchainImagesKHR", GetSwapchainImages) &&
           GetProcAddress(device, "vkAcquireNextImageKHR", AcquireNextImage) &&
           GetProcAddress(device, "vkQueuePresentKHR", QueuePresent);
}