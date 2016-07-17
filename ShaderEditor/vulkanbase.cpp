////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkanbase.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "vulkanbase.h"
#include "vulkancommon.h"
#include "logger.h"
#include "renderdata.h"

namespace
{
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

    VkBool32 DebugMessageCallback(VkDebugReportFlagsEXT flags,
                                  VkDebugReportObjectTypeEXT objType,
                                  uint64_t srcObject,
                                  size_t location,
                                  int32_t msgCode,
                                  const char* pLayerPrefix,
                                  const char* pMsg,
                                  void* pUserData)
    {
        bool isError = false;
        std::string text("Vulkan: ");

        if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
        {
            text += "Error:";
            isError = true;
        }
        if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT)
        {
            text += "Warning: ";
        }
        if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)
        {
            text += "Performance: ";
        }
        if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT)
        {
            text += "Info: ";
        }
        if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT)
        {
            text += "Debug: ";
        }

        text += pLayerPrefix;
        text += pMsg;
        text += " [" + std::to_string(msgCode) + "]";

        if (isError)
        {
            Logger::LogError(text);
        }
        else
        {
            Logger::LogInfo(text);
        }
        return VK_FALSE; // True will exit on error
    }
}

VulkanBase::VulkanBase(HINSTANCE hinstance, HWND hwnd) :
    m_hinstance(hinstance),
    m_hwnd(hwnd)
{
}

VulkanBase::~VulkanBase()
{
    Release();
}

void VulkanBase::Release()
{
    for (size_t i = 0; i < m_buffers.size(); ++i)
    {
        vkDestroyImageView(m_device, m_buffers[i].View, nullptr);
    }
    DestroySwapchain(m_device, m_swapChain, nullptr);
    vkDestroySurfaceKHR(m_instance, m_surface, nullptr);

    vkDestroySemaphore(m_device, m_presentCompleteSemaphore, nullptr);
    vkDestroySemaphore(m_device, m_renderCompleteSemaphore, nullptr);

    if (m_debugCallback != VK_NULL_HANDLE)
    {
        DestroyDebugReport(m_instance, m_debugCallback, nullptr);
    }

    vkDestroyDevice(m_device, nullptr);
    vkDestroyInstance(m_instance, nullptr);
}

bool VulkanBase::Initialise()
{
    if (!InitializeInstance())
    {
        Logger::LogError("Vulkan: InitializeInstance failed");
        return false;
    }
    if (!InitializeDevice())
    {
        Logger::LogError("Vulkan: InitializeDevice failed");
        return false;
    }
    if (!InitializeDebugging())
    {
        Logger::LogError("Vulkan: InitializeDebugging failed");
        return false;
    }
    if (!InitializeSurface())
    {
        Logger::LogError("Vulkan: InitializeSurface failed");
        return false;
    }
    if (!InitializeSwapChain())
    {
        Logger::LogError("Vulkan: InitializeSwapChain failed");
        return false;
    }
    return true;
}

bool VulkanBase::InitializeInstance()
{
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "";
    appInfo.pEngineName = "";
    appInfo.apiVersion = VK_API_VERSION_1_0;

    std::vector<const char*> enabledExtensions = { VK_KHR_SURFACE_EXTENSION_NAME };
    enabledExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);

    VkInstanceCreateInfo instanceCreateInfo = {};
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pNext = NULL;
    instanceCreateInfo.pApplicationInfo = &appInfo;
    if (enabledExtensions.size() > 0)
    {
        if (m_enableValidation)
        {
            enabledExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
        }
        instanceCreateInfo.enabledExtensionCount = (uint32_t)enabledExtensions.size();
        instanceCreateInfo.ppEnabledExtensionNames = enabledExtensions.data();
    }
    if (m_enableValidation)
    {
        instanceCreateInfo.enabledLayerCount = m_validationLayerCount;
        instanceCreateInfo.ppEnabledLayerNames = &m_validationLayerNames;
    }

    if (FAIL(vkCreateInstance(&instanceCreateInfo, nullptr, &m_instance)))
    {
        Logger::LogError("Vulkan: vkCreateInstance failed");
        return false;
    }
    return true;
}

bool VulkanBase::InitializeDevice()
{
    uint32_t gpuCount = 0;

    // Get number of available physical devices
    if (FAIL(vkEnumeratePhysicalDevices(m_instance, &gpuCount, nullptr)))
    {
        Logger::LogError("Vulkan: vkEnumeratePhysicalDevices failed");
        return false;
    }
    assert(gpuCount > 0);

    // Enumerate devices
    std::vector<VkPhysicalDevice> physicalDevices(gpuCount);
    if (FAIL(vkEnumeratePhysicalDevices(m_instance, &gpuCount, physicalDevices.data())))
    {
        Logger::LogError("Vulkan: Could not enumerate phyiscal devices");
        return false;
    }

    // use the first physical device reported, change the vector index if you have 
    // multiple Vulkan devices installed and want to use another one
    m_physicalDevice = physicalDevices[0];

    // Find a queue that supports graphics operations
    uint32_t graphicsQueueIndex = 0;
    uint32_t queueCount;
    vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueCount, NULL);
    assert(queueCount >= 1);

    std::vector<VkQueueFamilyProperties> queueProps;
    queueProps.resize(queueCount);
    vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueCount, queueProps.data());

    for (graphicsQueueIndex = 0; graphicsQueueIndex < queueCount; graphicsQueueIndex++)
    {
        if (queueProps[graphicsQueueIndex].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            break;
        }
    }
    assert(graphicsQueueIndex < queueCount);

    // Create the Vulkan device
    std::array<float, 1> queuePriorities = { 0.0f };
    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = graphicsQueueIndex;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = queuePriorities.data();

    std::vector<const char*> enabledExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pNext = NULL;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
    deviceCreateInfo.pEnabledFeatures = &m_enabledFeatures;

    if (enabledExtensions.size() > 0)
    {
        deviceCreateInfo.enabledExtensionCount = (uint32_t)enabledExtensions.size();
        deviceCreateInfo.ppEnabledExtensionNames = enabledExtensions.data();
    }

    if (FAIL(vkCreateDevice(m_physicalDevice, &deviceCreateInfo, nullptr, &m_device)))
    {
        Logger::LogError("Vulkan: vkCreateDevice failed");
        return false;
    }

    // Store properties (including limits) and features of the phyiscal device
    // So examples can check against them and see if a feature is actually supported
    vkGetPhysicalDeviceProperties(m_physicalDevice, &m_deviceProperties);
    vkGetPhysicalDeviceFeatures(m_physicalDevice, &m_deviceFeatures);

    // Gather physical device memory properties
    vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &m_deviceMemoryProperties);

    // Get the graphics queue
    vkGetDeviceQueue(m_device, graphicsQueueIndex, 0, &m_queue);

    // Find a suitable depth format
    // Start with the highest precision packed format
    std::vector<VkFormat> depthFormats = {
        VK_FORMAT_D32_SFLOAT_S8_UINT,
        VK_FORMAT_D32_SFLOAT,
        VK_FORMAT_D24_UNORM_S8_UINT,
        VK_FORMAT_D16_UNORM_S8_UINT,
        VK_FORMAT_D16_UNORM
    };

    for (auto& format : depthFormats)
    {
        VkFormatProperties formatProps;
        vkGetPhysicalDeviceFormatProperties(m_physicalDevice, format, &formatProps);

        // Format must support depth stencil attachment for optimal tiling
        if (formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
        {
            m_depthFormat = format;
            break;
        }
    }

    if (m_depthFormat == VK_FORMAT_UNDEFINED)
    {
        Logger::LogError("Vulkan: Could not find valid depth format");
        return false;
    }

    // Get all required function pointers
    if (!InitializeFunctionPointers())
    {
        Logger::LogError("Vulkan: InitializeFunctionPointers failed");
        return false;
    }

    // Create synchronization objects
    VkSemaphoreCreateInfo semaphoreCreateInfo = {};
    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreCreateInfo.pNext = NULL;
    semaphoreCreateInfo.flags = 0;

    // Create a semaphore used to synchronize image presentation
    // Ensures that the image is displayed before we start submitting new commands to the queu
    if (FAIL(vkCreateSemaphore(m_device, &semaphoreCreateInfo, nullptr, &m_presentCompleteSemaphore)))
    {
        Logger::LogError("Vulkan: vkCreateSemaphore failed");
        return false;
    }

    // Create a semaphore used to synchronize command submission
    // Ensures that the image is not presented until all commands have been sumbitted and executed
    if (FAIL(vkCreateSemaphore(m_device, &semaphoreCreateInfo, nullptr, &m_renderCompleteSemaphore)))
    {
        Logger::LogError("Vulkan: vkCreateSemaphore failed");
        return false;
    }

    // Set up submit info structure
    // Semaphores will stay the same during application lifetime
    // Command buffer submission info is set by each example
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = NULL;
    submitInfo.pWaitDstStageMask = &m_submitPipelineStages;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &m_presentCompleteSemaphore;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &m_renderCompleteSemaphore;

    return true;
}

bool VulkanBase::InitializeSwapChain()
{
    VkSwapchainKHR oldSwapchain = m_swapChain;

    // Get physical device surface properties and formats
    VkSurfaceCapabilitiesKHR surfCaps;
    if (FAIL(GetPhysicalDeviceSurfaceCapabilities(m_physicalDevice, m_surface, &surfCaps)))
    {
        Logger::LogError("Vulkan: GetPhysicalDeviceSurfaceCapabilities failed");
        return false;
    }

    // Get available present modes
    uint32_t presentModeCount;
    if (FAIL(GetPhysicalDeviceSurfacePresentModes(m_physicalDevice, m_surface, &presentModeCount, NULL)))
    {
        Logger::LogError("Vulkan: GetPhysicalDeviceSurfacePresentModes failed");
        return false;
    }
    assert(presentModeCount > 0);

    std::vector<VkPresentModeKHR> presentModes(presentModeCount);
    if (FAIL(GetPhysicalDeviceSurfacePresentModes(m_physicalDevice, m_surface, &presentModeCount, presentModes.data())))
    {
        Logger::LogError("Vulkan: GetPhysicalDeviceSurfacePresentModes failed");
        return false;
    }

    VkExtent2D swapchainExtent = {};
    swapchainExtent.width = WINDOW_WIDTH;
    swapchainExtent.height = WINDOW_HEIGHT;

    // Select a present mode for the swapchain
    // The VK_PRESENT_MODE_FIFO_KHR mode must always be present as per spec
    // This mode waits for the vertical blank ("v-sync")
    VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;

    // If v-sync is not requested, try to find a mailbox mode if present
    // It's the lowest latency non-tearing present mode available
    if (!m_vsync)
    {
        for (size_t i = 0; i < presentModeCount; i++)
        {
            if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                swapchainPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
                break;
            }
            if ((swapchainPresentMode != VK_PRESENT_MODE_MAILBOX_KHR) && (presentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR))
            {
                swapchainPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
            }
        }
    }

    // Determine the number of images
    uint32_t desiredNumberOfSwapchainImages = surfCaps.minImageCount + 1;
    if ((surfCaps.maxImageCount > 0) && (desiredNumberOfSwapchainImages > surfCaps.maxImageCount))
    {
        desiredNumberOfSwapchainImages = surfCaps.maxImageCount;
    }

    VkSurfaceTransformFlagsKHR preTransform;
    if (surfCaps.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
    {
        preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    }
    else
    {
        preTransform = surfCaps.currentTransform;
    }

    VkSwapchainCreateInfoKHR swapchainCI = {};
    swapchainCI.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainCI.pNext = NULL;
    swapchainCI.surface = m_surface;
    swapchainCI.minImageCount = desiredNumberOfSwapchainImages;
    swapchainCI.imageFormat = m_colorFormat;
    swapchainCI.imageColorSpace = m_colorSpace;
    swapchainCI.imageExtent = { swapchainExtent.width, swapchainExtent.height };
    swapchainCI.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchainCI.preTransform = (VkSurfaceTransformFlagBitsKHR)preTransform;
    swapchainCI.imageArrayLayers = 1;
    swapchainCI.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapchainCI.queueFamilyIndexCount = 0;
    swapchainCI.pQueueFamilyIndices = NULL;
    swapchainCI.presentMode = swapchainPresentMode;
    swapchainCI.oldSwapchain = oldSwapchain;
    swapchainCI.clipped = true;
    swapchainCI.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    if (FAIL(CreateSwapchain(m_device, &swapchainCI, nullptr, &m_swapChain)))
    {
        Logger::LogError("Vulkan: CreateSwapchain failed");
        return false;
    }

    // If an existing sawp chain is re-created, destroy the old swap chain
    // This also cleans up all the presentable images
    if (oldSwapchain != VK_NULL_HANDLE)
    {
        for (uint32_t i = 0; i < m_imageCount; i++)
        {
            vkDestroyImageView(m_device, m_buffers[i].View, nullptr);
        }
        DestroySwapchain(m_device, oldSwapchain, nullptr);
    }

    if (FAIL(GetSwapchainImages(m_device, m_swapChain, &m_imageCount, 0)))
    {
        Logger::LogError("Vulkan: GetSwapchainImages failed");
        return false;
    }

    // Get the swap chain images
    m_images.resize(m_imageCount);
    if(FAIL(GetSwapchainImages(m_device, m_swapChain, &m_imageCount, m_images.data())))
    {
        Logger::LogError("Vulkan: GetSwapchainImages failed");
        return false;
    }

    // Get the swap chain buffers containing the image and imageview
    m_buffers.resize(m_imageCount);
    for (uint32_t i = 0; i < m_imageCount; i++)
    {
        VkImageViewCreateInfo colorAttachmentView = {};
        colorAttachmentView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        colorAttachmentView.pNext = 0;
        colorAttachmentView.format = m_colorFormat;
        colorAttachmentView.components = 
        {
            VK_COMPONENT_SWIZZLE_R,
            VK_COMPONENT_SWIZZLE_G,
            VK_COMPONENT_SWIZZLE_B,
            VK_COMPONENT_SWIZZLE_A
        };
        colorAttachmentView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        colorAttachmentView.subresourceRange.baseMipLevel = 0;
        colorAttachmentView.subresourceRange.levelCount = 1;
        colorAttachmentView.subresourceRange.baseArrayLayer = 0;
        colorAttachmentView.subresourceRange.layerCount = 1;
        colorAttachmentView.viewType = VK_IMAGE_VIEW_TYPE_2D;
        colorAttachmentView.flags = 0;

        m_buffers[i].Image = m_images[i];
        colorAttachmentView.image = m_buffers[i].Image;

        if (FAIL(vkCreateImageView(m_device, &colorAttachmentView, nullptr, &m_buffers[i].View)))
        {
            Logger::LogError("Vulkan: vkCreateImageView failed");
            return false;
        }
    }
    return true;
}

bool VulkanBase::InitializeSurface()
{
    VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {};
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    surfaceCreateInfo.hinstance = m_hinstance;
    surfaceCreateInfo.hwnd = m_hwnd;
    if (FAIL(vkCreateWin32SurfaceKHR(m_instance, &surfaceCreateInfo, nullptr, &m_surface)))
    {
        Logger::LogError("Vulkan: vkCreateWin32SurfaceKHR failed");
        return false;
    }

    uint32_t queueCount;
    vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueCount, 0);
    assert(queueCount >= 1);

    std::vector<VkQueueFamilyProperties> queueProps(queueCount);
    vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueCount, queueProps.data());

    // Iterate over each queue to learn whether it supports presenting
    // Will be used to present the swap chain images to the windowing system
    std::vector<VkBool32> supportsPresent(queueCount);
    for (uint32_t i = 0; i < queueCount; i++)
    {
        if (FAIL(vkGetPhysicalDeviceSurfaceSupportKHR(m_physicalDevice, i, m_surface, &supportsPresent[i])))
        {
            Logger::LogError("Vulkan: getPhysicalDeviceSurfaceSupportKHR failed for " + std::to_string(i));
            return false;
        }
    }

    // Search for a graphics and a present queue in the array 
    // of queue families, try to find one that supports both
    uint32_t graphicsQueueNodeIndex = UINT32_MAX;
    uint32_t presentQueueNodeIndex = UINT32_MAX;
    for (uint32_t i = 0; i < queueCount; ++i)
    {
        if ((queueProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0)
        {
            if (graphicsQueueNodeIndex == UINT32_MAX)
            {
                graphicsQueueNodeIndex = i;
            }

            if (supportsPresent[i] == VK_TRUE)
            {
                graphicsQueueNodeIndex = i;
                presentQueueNodeIndex = i;
                break;
            }
        }
    }

    if (presentQueueNodeIndex == UINT32_MAX)
    {
        // If there's no queue that supports both present and graphics
        // try to find a separate present queue
        for (uint32_t i = 0; i < queueCount; ++i)
        {
            if (supportsPresent[i] == VK_TRUE)
            {
                presentQueueNodeIndex = i;
                break;
            }
        }
    }

    // Exit if either a graphics or a presenting queue hasn't been found
    if (graphicsQueueNodeIndex == UINT32_MAX || presentQueueNodeIndex == UINT32_MAX)
    {
        Logger::LogError("Vulkan: Could not find a graphics and/or presenting queue");
        return false;
    }
    if (graphicsQueueNodeIndex != presentQueueNodeIndex)
    {
        Logger::LogError("Vulkan: Separate graphics and presenting queues are not supported yet");
        return false;
    }

    m_queueNodeIndex = graphicsQueueNodeIndex;

    // Get list of supported surface formats
    uint32_t formatCount;
    if (FAIL(vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_surface, &formatCount, 0)))
    {
        Logger::LogError("Vulkan: getPhysicalDeviceSurfaceFormatsKHR failed");
        return false;
    }
    assert(formatCount > 0);

    std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
    if (FAIL(vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_surface, &formatCount, surfaceFormats.data())))
    {
        Logger::LogError("Vulkan: getPhysicalDeviceSurfaceFormatsKHR failed");
        return false;
    }

    // If the surface format list only includes one entry with VK_FORMAT_UNDEFINED,
    // there is no preferered format, so we assume VK_FORMAT_B8G8R8A8_UNORM
    if ((formatCount == 1) && (surfaceFormats[0].format == VK_FORMAT_UNDEFINED))
    {
        m_colorFormat = VK_FORMAT_B8G8R8A8_UNORM;
    }
    else
    {
        // Always select the first available color format
        // If you need a specific format (e.g. SRGB) you'd need to
        // iterate over the list of available surface format and check for it's presence
        m_colorFormat = surfaceFormats[0].format;
    }
    m_colorSpace = surfaceFormats[0].colorSpace;

    return true;
}

bool VulkanBase::InitializeFunctionPointers()
{
    return GetProcAddress(m_instance, "vkCreateDebugReportCallbackEXT", CreateDebugReport) &&
           GetProcAddress(m_instance, "vkDestroyDebugReportCallbackEXT", DestroyDebugReport) &&
           GetProcAddress(m_instance, "vkDebugReportMessageEXT", DbgBreak) &&
           GetProcAddress(m_instance, "vkGetPhysicalDeviceSurfaceSupportKHR", GetPhysicalDeviceSurfaceSupport) &&
           GetProcAddress(m_instance, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR", GetPhysicalDeviceSurfaceCapabilities) &&
           GetProcAddress(m_instance, "vkGetPhysicalDeviceSurfaceFormatsKHR", GetPhysicalDeviceSurfaceFormats) &&
           GetProcAddress(m_instance, "vkGetPhysicalDeviceSurfacePresentModesKHR", GetPhysicalDeviceSurfacePresentModes) &&
           GetProcAddress(m_device, "vkCreateSwapchainKHR", CreateSwapchain) &&
           GetProcAddress(m_device, "vkDestroySwapchainKHR", DestroySwapchain) &&
           GetProcAddress(m_device, "vkGetSwapchainImagesKHR", GetSwapchainImages) &&
           GetProcAddress(m_device, "vkAcquireNextImageKHR", AcquireNextImage) &&
           GetProcAddress(m_device, "vkQueuePresentKHR", QueuePresent);
}

bool VulkanBase::InitializeDebugging()
{
    VkDebugReportCallbackCreateInfoEXT dbgCreateInfo = {};
    dbgCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
    dbgCreateInfo.pfnCallback = (PFN_vkDebugReportCallbackEXT)DebugMessageCallback;

    dbgCreateInfo.flags = m_enableWarnings ?
        VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT :
        VK_DEBUG_REPORT_ERROR_BIT_EXT;

    if (FAIL(CreateDebugReport(m_instance, &dbgCreateInfo, nullptr, &m_debugCallback)))
    {
        Logger::LogError("Vulkan: CreateDebugReport failed");
        return false;
    }
    return true;
}