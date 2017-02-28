////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkaninit.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "vulkaninit.h"
#include "vulkanutils.h"
#include "vulkandata.h"
#include "logger.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

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
        if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
        {
            Logger::LogError("Vulkan: Error %s - %s - %d", pLayerPrefix, pMsg, msgCode);
        }
        else if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT)
        {
            Logger::LogInfo("Vulkan: Warning %s - %s - %d", pLayerPrefix, pMsg, msgCode);
        }
        else if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)
        {
            Logger::LogInfo("Vulkan: Performance %s - %s - %d", pLayerPrefix, pMsg, msgCode);
        }
        else if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT)
        {
            Logger::LogInfo("Vulkan: Info %s - %s - %d", pLayerPrefix, pMsg, msgCode);
        }
        else if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT)
        {
            Logger::LogInfo("Vulkan: Debug %s - %s - %d", pLayerPrefix, pMsg, msgCode);
        }
        return VK_FALSE; // True will exit on error
    }

    VkResult InitGlobalExtensionProperties(LayerProperties& layerProps)
    {
        VkExtensionProperties* instanceExtensions = nullptr;
        uint32_t instanceExtensionCount;
        VkResult result;
        char* layerName = nullptr;
        layerName = layerProps.properties.layerName;

        do
        {
            result = vkEnumerateInstanceExtensionProperties(layerName, &instanceExtensionCount, nullptr);
            if (result)
            {
                return result;
            }

            if (instanceExtensionCount == 0)
            {
                return VK_SUCCESS;
            }

            layerProps.extensions.resize(instanceExtensionCount);
            instanceExtensions = layerProps.extensions.data();
            result = vkEnumerateInstanceExtensionProperties(layerName, &instanceExtensionCount, instanceExtensions);

        } while (result == VK_INCOMPLETE);

        return result;
    }
}

VkResult VulkanInit::InitEnumerateDevice(VulkanData &info)
{
    uint32_t gpuCount = 0;
    const uint32_t reqCount = 1;
    VkResult result = vkEnumeratePhysicalDevices(info.instance, &gpuCount, NULL);
    if (VulkanUtils::Failed(result))
    {
        return result;
    }
    if (gpuCount < 1)
    {
        return VulkanUtils::VK_ERROR;
    }
    info.gpus.resize(gpuCount);

    result = vkEnumeratePhysicalDevices(info.instance, &gpuCount, info.gpus.data());
    if (VulkanUtils::Failed(result))
    {
        return result;
    }

    if (gpuCount < reqCount)
    {
        return VulkanUtils::VK_ERROR;
    }

    vkGetPhysicalDeviceQueueFamilyProperties(info.gpus[0], &info.queueFamilyCount, NULL);
    if (info.queueFamilyCount < 1)
    {
        return VulkanUtils::VK_ERROR;
    }

    info.queueProps.resize(info.queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(info.gpus[0], &info.queueFamilyCount, info.queueProps.data());
    if (info.queueFamilyCount < 1)
    {
        return VulkanUtils::VK_ERROR;
    }

    vkGetPhysicalDeviceMemoryProperties(info.gpus[0], &info.memoryProperties);
    vkGetPhysicalDeviceProperties(info.gpus[0], &info.gpuProps);

    return result;
}

VkResult VulkanInit::InitInstance(VulkanData &info)
{
    info.instanceExtensionNames =
    {
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
        VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
    };

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext = NULL;
    appInfo.pApplicationName = "";
    appInfo.applicationVersion = 1;
    appInfo.pEngineName = "";
    appInfo.engineVersion = 1;
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo instInfo = {};
    instInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instInfo.pNext = NULL;
    instInfo.flags = 0;
    instInfo.pApplicationInfo = &appInfo;
    instInfo.enabledLayerCount = info.instanceLayerNames.size();
    instInfo.ppEnabledLayerNames = info.instanceLayerNames.size() ? info.instanceLayerNames.data() : NULL;
    instInfo.enabledExtensionCount = info.instanceExtensionNames.size();
    instInfo.ppEnabledExtensionNames = info.instanceExtensionNames.data();

    return vkCreateInstance(&instInfo, NULL, &info.instance);
}

VkResult VulkanInit::InitDevice(VulkanData &info)
{
    info.deviceExtensionNames =
    {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    VkDeviceQueueCreateInfo queueInfo = {};

    float queuePriorities[1] = { 0.0 };
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.pNext = NULL;
    queueInfo.queueCount = 1;
    queueInfo.pQueuePriorities = queuePriorities;
    queueInfo.queueFamilyIndex = info.graphicsQueueFamilyIndex;

    VkDeviceCreateInfo deviceInfo = {};
    deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceInfo.pNext = NULL;
    deviceInfo.queueCreateInfoCount = 1;
    deviceInfo.pQueueCreateInfos = &queueInfo;
    deviceInfo.enabledExtensionCount = info.deviceExtensionNames.size();
    deviceInfo.ppEnabledExtensionNames = deviceInfo.enabledExtensionCount ? info.deviceExtensionNames.data() : NULL;
    deviceInfo.pEnabledFeatures = NULL;

    return vkCreateDevice(info.gpus[0], &deviceInfo, NULL, &info.device);
}

VkResult VulkanInit::InitDebugging(VulkanData &info)
{
    if (!GetProcAddress(info.instance, "vkCreateDebugReportCallbackEXT", info.createDebugReportFn) ||
        !GetProcAddress(info.instance, "vkDestroyDebugReportCallbackEXT", info.destroyDebugReportFn) ||
        !GetProcAddress(info.device, "vkDebugMarkerSetObjectNameEXT", info.setDebugNameFn))
    {
        return VulkanUtils::VK_ERROR;
    }

    VkDebugReportCallbackCreateInfoEXT dbgCreateInfo = {};
    dbgCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
    dbgCreateInfo.pfnCallback = (PFN_vkDebugReportCallbackEXT)DebugMessageCallback;
    dbgCreateInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
    return info.createDebugReportFn(info.instance, &dbgCreateInfo, nullptr, &info.debugCallback);
}

VkResult VulkanInit::InitGlobalLayerProperties(VulkanData& info)
{
    uint32_t instanceLayerCount;
    VkLayerProperties *vkProps = NULL;
    VkResult result;

    // It's possible, though very rare, that the number of
    // instance layers could change. For example, installing something
    // could include new layers that the loader would pick up
    // between the initial query for the count and the
    // request for VkLayerProperties. The loader indicates that
    // by returning a VK_INCOMPLETE status and will update the
    // the count parameter.
    // The count parameter will be updated with the number of
    // entries loaded into the data pointer - in case the number
    // of layers went down or is smaller than the size given.
    do {
        result = vkEnumerateInstanceLayerProperties(&instanceLayerCount, NULL);
        if (result)
        {
            return result;
        }

        if (instanceLayerCount == 0) 
        {
            return VK_SUCCESS;
        }

        vkProps = (VkLayerProperties *)realloc(vkProps, instanceLayerCount * sizeof(VkLayerProperties));

        result = vkEnumerateInstanceLayerProperties(&instanceLayerCount, vkProps);
    } while (result == VK_INCOMPLETE);

    // Now gather the extension list for each instance layer.
    for (uint32_t i = 0; i < instanceLayerCount; i++) 
    {
        LayerProperties layerProps;
        layerProps.properties = vkProps[i];
        result = InitGlobalExtensionProperties(layerProps);
        if (result)
        {
            return result;
        }
        info.instanceLayerProperties.push_back(layerProps);
    }

    free(vkProps);
    return result;
}

VkResult VulkanInit::InitSwapchainExtension(VulkanData &info)
{
    VkWin32SurfaceCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.pNext = NULL;
    createInfo.hinstance = info.connection;
    createInfo.hwnd = info.window;
    VkResult res = vkCreateWin32SurfaceKHR(info.instance, &createInfo, NULL, &info.surface);
    if (VulkanUtils::Failed(res))
    {
        return res;
    }

    // Iterate over each queue to learn whether it supports presenting:
    VkBool32 *pSupportsPresent = (VkBool32 *)malloc(info.queueFamilyCount * sizeof(VkBool32));
    for (uint32_t i = 0; i < info.queueFamilyCount; i++) 
    {
        res = vkGetPhysicalDeviceSurfaceSupportKHR(info.gpus[0], i, info.surface, &pSupportsPresent[i]);
        if (VulkanUtils::Failed(res))
        {
            return res;
        }
    }

    // Search for a graphics and a present queue in the array of queue
    // families, try to find one that supports both
    info.graphicsQueueFamilyIndex = UINT32_MAX;
    info.presentQueueFamilyIndex = UINT32_MAX;
    for (uint32_t i = 0; i < info.queueFamilyCount; ++i) 
    {
        if ((info.queueProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0)
        {
            if (info.graphicsQueueFamilyIndex == UINT32_MAX)
            {
                info.graphicsQueueFamilyIndex = i;
            }

            if (pSupportsPresent[i] == VK_TRUE) 
            {
                info.graphicsQueueFamilyIndex = i;
                info.presentQueueFamilyIndex = i;
                break;
            }
        }
    }

    if (info.presentQueueFamilyIndex == UINT32_MAX)
    {
        // If didn't find a queue that supports both graphics and present, then
        // find a separate present queue.
        for (size_t i = 0; i < info.queueFamilyCount; ++i)
        {
            if (pSupportsPresent[i] == VK_TRUE)
            {
                info.presentQueueFamilyIndex = i;
                break;
            }
        }
    }
    free(pSupportsPresent);

    // Generate error if could not find queues that support graphics
    // and present
    if (info.graphicsQueueFamilyIndex == UINT32_MAX || info.presentQueueFamilyIndex == UINT32_MAX)
    {
        Logger::LogError("Vulkan: Could not find a queues for both graphics and present");
        return VulkanUtils::VK_ERROR;
    }

    // Get the list of VkFormats that are supported:
    uint32_t formatCount;
    res = vkGetPhysicalDeviceSurfaceFormatsKHR(info.gpus[0], info.surface, &formatCount, NULL);
    if (VulkanUtils::Failed(res))
    {
        return res;
    }

    VkSurfaceFormatKHR *surfFormats = (VkSurfaceFormatKHR *)malloc(formatCount * sizeof(VkSurfaceFormatKHR));
    res = vkGetPhysicalDeviceSurfaceFormatsKHR(info.gpus[0], info.surface, &formatCount, surfFormats);
    if (VulkanUtils::Failed(res))
    {
        return res;
    }

    // If the format list includes just one entry of VK_FORMAT_UNDEFINED,
    // the surface has no preferred format.  Otherwise, at least one
    // supported format will be returned.
    if (formatCount == 1 && surfFormats[0].format == VK_FORMAT_UNDEFINED) 
    {
        info.format = VK_FORMAT_B8G8R8A8_UNORM;
    }
    else 
    {
        if (formatCount < 1)
        {
            return VulkanUtils::VK_ERROR;
        }
        info.format = surfFormats[0].format;
    }
    free(surfFormats);
    return VK_SUCCESS;
}

VkResult VulkanInit::InitCommandPool(VulkanData &info)
{
    VkCommandPoolCreateInfo cmdPoolInfo = {};
    cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cmdPoolInfo.pNext = NULL;
    cmdPoolInfo.queueFamilyIndex = info.graphicsQueueFamilyIndex;
    cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    return vkCreateCommandPool(info.device, &cmdPoolInfo, NULL, &info.cmdPool);
}

VkResult VulkanInit::InitCommandBuffer(VulkanData &info)
{
    info.cmd.resize(info.swapchainImageCount);

    VkCommandBufferAllocateInfo cmd = {};
    cmd.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmd.pNext = NULL;
    cmd.commandPool = info.cmdPool;
    cmd.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    cmd.commandBufferCount = info.cmd.size();

    VkResult result = vkAllocateCommandBuffers(info.device, &cmd, &info.cmd[0]);
    if(!VulkanUtils::Failed(result))
    {
        for (int i = 0; i < (int)info.cmd.size(); ++i)
        {
            VulkanUtils::SetDebugName(info, (uint64_t)info.cmd[i], typeid(info.cmd[i]), 
                ("CommandBUffer" + std::to_string(i)).c_str());
        }
    }
    return result;
}

VkResult VulkanInit::InitDeviceQueue(VulkanData &info)
{
    vkGetDeviceQueue(info.device, info.graphicsQueueFamilyIndex, 0, &info.graphicsQueue);
    if (info.graphicsQueueFamilyIndex == info.presentQueueFamilyIndex) 
    {
        info.presentQueue = info.graphicsQueue;
    }
    else 
    {
        vkGetDeviceQueue(info.device, info.presentQueueFamilyIndex, 0, &info.presentQueue);
    }

    VulkanUtils::SetDebugName(info, (uint64_t)info.graphicsQueue, typeid(info.graphicsQueue), "GraphicsQueue");
    VulkanUtils::SetDebugName(info, (uint64_t)info.presentQueue, typeid(info.presentQueue), "PresentQueue");

    return VK_SUCCESS;
}

VkResult VulkanInit::InitSwapChain(VulkanData &info)
{
    VkImageUsageFlags usageFlags =
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
        VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

    VkSurfaceCapabilitiesKHR surfCapabilities;

    VkResult res = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(info.gpus[0], info.surface, &surfCapabilities);
    if (VulkanUtils::Failed(res))
    {
        return res;
    }

    uint32_t presentModeCount;
    res = vkGetPhysicalDeviceSurfacePresentModesKHR(info.gpus[0], info.surface, &presentModeCount, NULL);
    if (VulkanUtils::Failed(res))
    {
        return res;
    }

    VkPresentModeKHR *presentModes = (VkPresentModeKHR *)malloc(presentModeCount * sizeof(VkPresentModeKHR));
    if (!presentModes)
    {
        return VulkanUtils::VK_ERROR;
    }

    res = vkGetPhysicalDeviceSurfacePresentModesKHR(info.gpus[0], info.surface, &presentModeCount, presentModes);
    if (VulkanUtils::Failed(res))
    {
        return res;
    }

    VkExtent2D swapchainExtent;

    // width and height are either both 0xFFFFFFFF, or both not 0xFFFFFFFF.
    if (surfCapabilities.currentExtent.width == 0xFFFFFFFF) 
    {
        // If the surface size is undefined, the size is set to
        // the size of the images requested.
        swapchainExtent.width = WINDOW_WIDTH;
        swapchainExtent.height = WINDOW_HEIGHT;
        if (swapchainExtent.width < surfCapabilities.minImageExtent.width) 
        {
            swapchainExtent.width = surfCapabilities.minImageExtent.width;
        }
        else if (swapchainExtent.width > surfCapabilities.maxImageExtent.width) 
        {
            swapchainExtent.width = surfCapabilities.maxImageExtent.width;
        }

        if (swapchainExtent.height < surfCapabilities.minImageExtent.height) 
        {
            swapchainExtent.height = surfCapabilities.minImageExtent.height;
        }
        else if (swapchainExtent.height > surfCapabilities.maxImageExtent.height) 
        {
            swapchainExtent.height = surfCapabilities.maxImageExtent.height;
        }
    }
    else 
    {
        // If the surface size is defined, the swap chain size must match
        swapchainExtent = surfCapabilities.currentExtent;
    }

    // The FIFO present mode is guaranteed by the spec to be supported
    // Also note that current Android driver only supports FIFO
    VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;

    // Determine the number of VkImage's to use in the swap chain.
    // We need to acquire only 1 presentable image at at time.
    // Asking for minImageCount images ensures that we can acquire
    // 1 presentable image as long as we present it before attempting
    // to acquire another.
    uint32_t desiredNumberOfSwapChainImages = surfCapabilities.minImageCount;

    VkSurfaceTransformFlagBitsKHR preTransform;
    if (surfCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) 
    {
        preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    }
    else 
    {
        preTransform = surfCapabilities.currentTransform;
    }

    VkSwapchainCreateInfoKHR swapchainCi = {};
    swapchainCi.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainCi.pNext = NULL;
    swapchainCi.surface = info.surface;
    swapchainCi.minImageCount = desiredNumberOfSwapChainImages;
    swapchainCi.imageFormat = info.format;
    swapchainCi.imageExtent.width = swapchainExtent.width;
    swapchainCi.imageExtent.height = swapchainExtent.height;
    swapchainCi.preTransform = preTransform;
    swapchainCi.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchainCi.imageArrayLayers = 1;
    swapchainCi.presentMode = swapchainPresentMode;
    swapchainCi.oldSwapchain = VK_NULL_HANDLE;
    swapchainCi.clipped = true;
    swapchainCi.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
    swapchainCi.imageUsage = usageFlags;
    swapchainCi.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapchainCi.queueFamilyIndexCount = 0;
    swapchainCi.pQueueFamilyIndices = NULL;
    uint32_t queueFamilyIndices[2] = { (uint32_t)info.graphicsQueueFamilyIndex, (uint32_t)info.presentQueueFamilyIndex };
    if (info.graphicsQueueFamilyIndex != info.presentQueueFamilyIndex) 
    {
        // If the graphics and present queues are from different queue families,
        // we either have to explicitly transfer ownership of images between the
        // queues, or we have to create the swapchain with imageSharingMode
        // as VK_SHARINGMODE_CONCURRENT
        swapchainCi.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapchainCi.queueFamilyIndexCount = 2;
        swapchainCi.pQueueFamilyIndices = queueFamilyIndices;
    }

    res = vkCreateSwapchainKHR(info.device, &swapchainCi, NULL, &info.swapChain);
    if (VulkanUtils::Failed(res))
    {
        return res;
    }

    res = vkGetSwapchainImagesKHR(info.device, info.swapChain, &info.swapchainImageCount, NULL);
    if (VulkanUtils::Failed(res))
    {
        return res;
    }

    VkImage *swapchainImages = (VkImage *)malloc(info.swapchainImageCount * sizeof(VkImage));
    if (!swapchainImages)
    {
        return VulkanUtils::VK_ERROR;
    }

    res = vkGetSwapchainImagesKHR(info.device, info.swapChain, &info.swapchainImageCount, swapchainImages);
    if (VulkanUtils::Failed(res))
    {
        return res;
    }

    for (uint32_t i = 0; i < info.swapchainImageCount; i++) 
    {
        SwapChainBuffer scBuffer;

        VkImageViewCreateInfo colorImageView = {};
        colorImageView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        colorImageView.pNext = NULL;
        colorImageView.format = info.format;
        colorImageView.components.r = VK_COMPONENT_SWIZZLE_R;
        colorImageView.components.g = VK_COMPONENT_SWIZZLE_G;
        colorImageView.components.b = VK_COMPONENT_SWIZZLE_B;
        colorImageView.components.a = VK_COMPONENT_SWIZZLE_A;
        colorImageView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        colorImageView.subresourceRange.baseMipLevel = 0;
        colorImageView.subresourceRange.levelCount = 1;
        colorImageView.subresourceRange.baseArrayLayer = 0;
        colorImageView.subresourceRange.layerCount = 1;
        colorImageView.viewType = VK_IMAGE_VIEW_TYPE_2D;
        colorImageView.flags = 0;

        scBuffer.image = swapchainImages[i];
        colorImageView.image = scBuffer.image;

        VulkanUtils::SetDebugName(info, (uint64_t)scBuffer.image, typeid(scBuffer.image),
            ("SwapChain" + std::to_string(i) + "Image").c_str());

        res = vkCreateImageView(info.device, &colorImageView, NULL, &scBuffer.view);
        info.buffers.push_back(scBuffer);
        if (VulkanUtils::Failed(res))
        {
            return res;
        }
    }
    free(swapchainImages);
    info.currentBuffer = 0;

    if (NULL != presentModes) 
    {
        free(presentModes);
    }
    return VK_SUCCESS;
}

VkResult VulkanInit::InitDepthBuffer(VulkanData &info)
{
    VkImageCreateInfo imageInfo = {};

    // allow custom depth formats
    if (info.depth.format == VK_FORMAT_UNDEFINED)
    {
        info.depth.format = VK_FORMAT_D16_UNORM;
    }

    const VkFormat depthFormat = info.depth.format;
    VkFormatProperties props;
    vkGetPhysicalDeviceFormatProperties(info.gpus[0], depthFormat, &props);
    if (props.linearTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) 
    {
        imageInfo.tiling = VK_IMAGE_TILING_LINEAR;
    }
    else if (props.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
    {
        imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    }
    else 
    {
        Logger::LogError("Vulkan: Depth Format Unsupported");
        return VulkanUtils::VK_ERROR;
    }

    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.pNext = NULL;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.format = depthFormat;
    imageInfo.extent.width = WINDOW_WIDTH;
    imageInfo.extent.height = WINDOW_HEIGHT;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.samples = VulkanUtils::NUM_SAMPLES;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.queueFamilyIndexCount = 0;
    imageInfo.pQueueFamilyIndices = NULL;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    imageInfo.flags = 0;

    VkMemoryAllocateInfo memAlloc = {};
    memAlloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memAlloc.pNext = NULL;
    memAlloc.allocationSize = 0;
    memAlloc.memoryTypeIndex = 0;

    VkImageViewCreateInfo viewInfo = {};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.pNext = NULL;
    viewInfo.image = VK_NULL_HANDLE;
    viewInfo.format = depthFormat;
    viewInfo.components.r = VK_COMPONENT_SWIZZLE_R;
    viewInfo.components.g = VK_COMPONENT_SWIZZLE_G;
    viewInfo.components.b = VK_COMPONENT_SWIZZLE_B;
    viewInfo.components.a = VK_COMPONENT_SWIZZLE_A;
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.flags = 0;

    if (depthFormat == VK_FORMAT_D16_UNORM_S8_UINT || 
        depthFormat == VK_FORMAT_D24_UNORM_S8_UINT ||
        depthFormat == VK_FORMAT_D32_SFLOAT_S8_UINT)
    {
        viewInfo.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
    }

    VkMemoryRequirements memReqs;

    // Create image 
    VkResult res = vkCreateImage(info.device, &imageInfo, NULL, &info.depth.image);
    if (VulkanUtils::Failed(res))
    {
        return res;
    }

    VulkanUtils::SetDebugName(info, (uint64_t)info.depth.image, typeid(info.depth.image), "DepthImage");
    vkGetImageMemoryRequirements(info.device, info.depth.image, &memReqs);

    memAlloc.allocationSize = memReqs.size;

    // Use the memory properties to determine the type of memory required
    res = VulkanUtils::MemoryTypeFromProperties(info, memReqs.memoryTypeBits, 0, &memAlloc.memoryTypeIndex);
    if (VulkanUtils::Failed(res))
    {
        return res;
    }

    // Allocate memory
    res = vkAllocateMemory(info.device, &memAlloc, NULL, &info.depth.memory);
    if (VulkanUtils::Failed(res))
    {
        return res;
    }

    // Bind memory
    res = vkBindImageMemory(info.device, info.depth.image, info.depth.memory, 0);
    if (VulkanUtils::Failed(res))
    {
        return res;
    }

    // Create image view
    viewInfo.image = info.depth.image;
    return vkCreateImageView(info.device, &viewInfo, NULL, &info.depth.view);
}

VkResult VulkanInit::InitDescriptorAndPipelineLayouts(VulkanData &info)
{
    bool use_texture = false;

    VkDescriptorSetLayoutBinding layoutBindings[2];
    layoutBindings[0].binding = 0;
    layoutBindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    layoutBindings[0].descriptorCount = 1;
    layoutBindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    layoutBindings[0].pImmutableSamplers = NULL;

    if (use_texture) 
    {
        layoutBindings[1].binding = 1;
        layoutBindings[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        layoutBindings[1].descriptorCount = 1;
        layoutBindings[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        layoutBindings[1].pImmutableSamplers = NULL;
    }

    // Next take layout bindings and use them to create a descriptor set layout
    VkDescriptorSetLayoutCreateInfo descriptorLayout = {};
    descriptorLayout.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptorLayout.pNext = NULL;
    descriptorLayout.bindingCount = use_texture ? 2 : 1;
    descriptorLayout.pBindings = layoutBindings;

    info.descLayout.resize(VulkanUtils::NUM_DESCRIPTOR_SETS);
    VkResult res = vkCreateDescriptorSetLayout(info.device, &descriptorLayout, NULL, info.descLayout.data());
    if (VulkanUtils::Failed(res))
    {
        return res;
    }

    for (int i = 0; i < (int)info.descLayout.size(); ++i)
    {
        VulkanUtils::SetDebugName(info, (uint64_t)info.descLayout[i], typeid(info.descLayout[i]), 
            ("DescLayout" + std::to_string(i)).c_str());
    }

    // Now use the descriptor layout to create a pipeline layout
    VkPipelineLayoutCreateInfo pPipelineLayoutCreateInfo = {};
    pPipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pPipelineLayoutCreateInfo.pNext = NULL;
    pPipelineLayoutCreateInfo.pushConstantRangeCount = 0;
    pPipelineLayoutCreateInfo.pPushConstantRanges = NULL;
    pPipelineLayoutCreateInfo.setLayoutCount = VulkanUtils::NUM_DESCRIPTOR_SETS;
    pPipelineLayoutCreateInfo.pSetLayouts = info.descLayout.data();

    res = vkCreatePipelineLayout(info.device, &pPipelineLayoutCreateInfo, NULL, &info.pipelineLayout);
    if (VulkanUtils::Failed(res))
    {
        return res;
    }

    VulkanUtils::SetDebugName(info, (uint64_t)info.pipelineLayout, typeid(info.pipelineLayout), "PipelineLayout");
    return VK_SUCCESS;
}

VkResult VulkanInit::InitDescriptorPool(VulkanData& info)
{
    bool use_texture = false;

    VkDescriptorPoolSize typeCount[2];
    typeCount[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    typeCount[0].descriptorCount = 1;
    if (use_texture) 
    {
        typeCount[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        typeCount[1].descriptorCount = 1;
    }

    VkDescriptorPoolCreateInfo descriptorPool = {};
    descriptorPool.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptorPool.pNext = NULL;
    descriptorPool.maxSets = 1;
    descriptorPool.poolSizeCount = use_texture ? 2 : 1;
    descriptorPool.pPoolSizes = typeCount;

    return vkCreateDescriptorPool(info.device, &descriptorPool, NULL, &info.descPool);
}

VkResult VulkanInit::InitRenderpass(VulkanData &info)
{
    bool includeDepth = true;
    bool clear = true;
    VkImageLayout finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    // Need attachments for render target and depth buffer
    VkAttachmentDescription attachments[2];
    attachments[0].format = info.format;
    attachments[0].samples = VulkanUtils::NUM_SAMPLES;
    attachments[0].loadOp = clear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[0].finalLayout = finalLayout;
    attachments[0].flags = 0;

    if (includeDepth) 
    {
        attachments[1].format = info.depth.format;
        attachments[1].samples = VulkanUtils::NUM_SAMPLES;
        attachments[1].loadOp = clear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
        attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        attachments[1].flags = 0;
    }

    VkAttachmentReference color_reference = {};
    color_reference.attachment = 0;
    color_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depth_reference = {};
    depth_reference.attachment = 1;
    depth_reference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.flags = 0;
    subpass.inputAttachmentCount = 0;
    subpass.pInputAttachments = NULL;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &color_reference;
    subpass.pResolveAttachments = NULL;
    subpass.pDepthStencilAttachment = includeDepth ? &depth_reference : NULL;
    subpass.preserveAttachmentCount = 0;
    subpass.pPreserveAttachments = NULL;

    VkRenderPassCreateInfo rpInfo = {};
    rpInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    rpInfo.pNext = NULL;
    rpInfo.attachmentCount = includeDepth ? 2 : 1;
    rpInfo.pAttachments = attachments;
    rpInfo.subpassCount = 1;
    rpInfo.pSubpasses = &subpass;
    rpInfo.dependencyCount = 0;
    rpInfo.pDependencies = NULL;

    VkResult res = vkCreateRenderPass(info.device, &rpInfo, NULL, &info.renderPass);
    if (VulkanUtils::Failed(res))
    {
        return res;
    }

    VulkanUtils::SetDebugName(info, (uint64_t)info.renderPass, typeid(info.renderPass), "RenderPass");
    return VK_SUCCESS;
}

VkResult VulkanInit::InitFramebuffers(VulkanData &info)
{
    bool includeDepth = true;

    VkImageView attachments[2];
    attachments[1] = info.depth.view;

    VkFramebufferCreateInfo fbInfo = {};
    fbInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    fbInfo.pNext = NULL;
    fbInfo.renderPass = info.renderPass;
    fbInfo.attachmentCount = includeDepth ? 2 : 1;
    fbInfo.pAttachments = attachments;
    fbInfo.width = WINDOW_WIDTH;
    fbInfo.height = WINDOW_HEIGHT;
    fbInfo.layers = 1;

    uint32_t i;

    info.framebuffers.resize(info.swapchainImageCount);

    for (i = 0; i < info.swapchainImageCount; i++) 
    {
        attachments[0] = info.buffers[i].view;
        VkResult res = vkCreateFramebuffer(info.device, &fbInfo, NULL, &info.framebuffers[i]);
        if (VulkanUtils::Failed(res))
        {
            return res;
        }
        VulkanUtils::SetDebugName(info, (uint64_t)info.framebuffers[i], typeid(info.framebuffers[i]),
            ("FrameBuffer" + std::to_string(i)).c_str());
    }
    return VK_SUCCESS;
}

VkResult VulkanInit::InitSemaphores(VulkanData& info)
{
    VkSemaphoreCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    createInfo.pNext = NULL;
    createInfo.flags = 0;

    VkResult res = vkCreateSemaphore(info.device, &createInfo, NULL, &info.renderCompleteSemaphore);
    if (VulkanUtils::Failed(res))
    {
        return res;
    }

    VulkanUtils::SetDebugName(info, (uint64_t)info.renderCompleteSemaphore,
        typeid(info.renderCompleteSemaphore), "RenderCompleteSemaphore");

    res = vkCreateSemaphore(info.device, &createInfo, NULL, &info.presentCompleteSemaphore);
    if (VulkanUtils::Failed(res))
    {
        return res;
    }

    VulkanUtils::SetDebugName(info, (uint64_t)info.presentCompleteSemaphore,
        typeid(info.presentCompleteSemaphore), "PresentCompleteSemaphore");

    return VK_SUCCESS;
}

VkResult VulkanInit::InitFence(VulkanData& info)
{
    info.fences.resize(info.swapchainImageCount);

    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.pNext = NULL;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (int i = 0; i < (int)info.fences.size(); ++i)
    {
        VkResult res = vkCreateFence(info.device, &fenceInfo, NULL, &info.fences[i]);
        if (VulkanUtils::Failed(res))
        {
            return res;
        }
        VulkanUtils::SetDebugName(info, (uint64_t)info.fences[i], typeid(info.fences[i]),
            ("Fence" + std::to_string(i)).c_str());
    }

    return VK_SUCCESS;
}
