////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkanfunctions.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "vulkan.h"

struct VulkanFunctions
{
    /**
    * Initialises all function pointers
    */
    bool Initialise(VkInstance& instance, VkDevice& device);

    PFN_vkGetPhysicalDeviceSurfaceSupportKHR       GetPhysicalDeviceSurfaceSupport      = VK_NULL_HANDLE;
    PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR  GetPhysicalDeviceSurfaceCapabilities = VK_NULL_HANDLE;
    PFN_vkGetPhysicalDeviceSurfaceFormatsKHR       GetPhysicalDeviceSurfaceFormats      = VK_NULL_HANDLE;
    PFN_vkGetPhysicalDeviceSurfacePresentModesKHR  GetPhysicalDeviceSurfacePresentModes = VK_NULL_HANDLE;
    PFN_vkCreateSwapchainKHR                       CreateSwapchain                      = VK_NULL_HANDLE;
    PFN_vkDestroySwapchainKHR                      DestroySwapchain                     = VK_NULL_HANDLE;
    PFN_vkGetSwapchainImagesKHR                    GetSwapchainImages                   = VK_NULL_HANDLE;
    PFN_vkAcquireNextImageKHR                      AcquireNextImage                     = VK_NULL_HANDLE;
    PFN_vkQueuePresentKHR                          QueuePresent                         = VK_NULL_HANDLE;
    PFN_vkCreateDebugReportCallbackEXT             CreateDebugReport                    = VK_NULL_HANDLE;
    PFN_vkDestroyDebugReportCallbackEXT            DestroyDebugReport                   = VK_NULL_HANDLE;
    PFN_vkDebugReportMessageEXT                    DbgBreak                             = VK_NULL_HANDLE;
};