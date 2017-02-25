////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkanutils.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "vulkanutils.h"
#include "vulkandata.h"
#include "logger.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace 
{
    struct Vertex
    {
        float posX, posY, posZ, posW;  // Position data
        float r, g, b, a;              // Color
    };

    #define XYZ1(_x_, _y_, _z_) (_x_), (_y_), (_z_), 1.f

    static const Vertex g_vbSolidFaceColors_Data[] = {
        // red face
        { XYZ1(-1, -1, 1), XYZ1(1.f, 0.f, 0.f) },
        { XYZ1(-1, 1, 1), XYZ1(1.f, 0.f, 0.f) },
        { XYZ1(1, -1, 1), XYZ1(1.f, 0.f, 0.f) },
        { XYZ1(1, -1, 1), XYZ1(1.f, 0.f, 0.f) },
        { XYZ1(-1, 1, 1), XYZ1(1.f, 0.f, 0.f) },
        { XYZ1(1, 1, 1), XYZ1(1.f, 0.f, 0.f) },
        // green face
        { XYZ1(-1, -1, -1), XYZ1(0.f, 1.f, 0.f) },
        { XYZ1(1, -1, -1), XYZ1(0.f, 1.f, 0.f) },
        { XYZ1(-1, 1, -1), XYZ1(0.f, 1.f, 0.f) },
        { XYZ1(-1, 1, -1), XYZ1(0.f, 1.f, 0.f) },
        { XYZ1(1, -1, -1), XYZ1(0.f, 1.f, 0.f) },
        { XYZ1(1, 1, -1), XYZ1(0.f, 1.f, 0.f) },
        // blue face
        { XYZ1(-1, 1, 1), XYZ1(0.f, 0.f, 1.f) },
        { XYZ1(-1, -1, 1), XYZ1(0.f, 0.f, 1.f) },
        { XYZ1(-1, 1, -1), XYZ1(0.f, 0.f, 1.f) },
        { XYZ1(-1, 1, -1), XYZ1(0.f, 0.f, 1.f) },
        { XYZ1(-1, -1, 1), XYZ1(0.f, 0.f, 1.f) },
        { XYZ1(-1, -1, -1), XYZ1(0.f, 0.f, 1.f) },
        // yellow face
        { XYZ1(1, 1, 1), XYZ1(1.f, 1.f, 0.f) },
        { XYZ1(1, 1, -1), XYZ1(1.f, 1.f, 0.f) },
        { XYZ1(1, -1, 1), XYZ1(1.f, 1.f, 0.f) },
        { XYZ1(1, -1, 1), XYZ1(1.f, 1.f, 0.f) },
        { XYZ1(1, 1, -1), XYZ1(1.f, 1.f, 0.f) },
        { XYZ1(1, -1, -1), XYZ1(1.f, 1.f, 0.f) },
        // magenta face
        { XYZ1(1, 1, 1), XYZ1(1.f, 0.f, 1.f) },
        { XYZ1(-1, 1, 1), XYZ1(1.f, 0.f, 1.f) },
        { XYZ1(1, 1, -1), XYZ1(1.f, 0.f, 1.f) },
        { XYZ1(1, 1, -1), XYZ1(1.f, 0.f, 1.f) },
        { XYZ1(-1, 1, 1), XYZ1(1.f, 0.f, 1.f) },
        { XYZ1(-1, 1, -1), XYZ1(1.f, 0.f, 1.f) },
        // cyan face
        { XYZ1(1, -1, 1), XYZ1(0.f, 1.f, 1.f) },
        { XYZ1(1, -1, -1), XYZ1(0.f, 1.f, 1.f) },
        { XYZ1(-1, -1, 1), XYZ1(0.f, 1.f, 1.f) },
        { XYZ1(-1, -1, 1), XYZ1(0.f, 1.f, 1.f) },
        { XYZ1(1, -1, -1), XYZ1(0.f, 1.f, 1.f) },
        { XYZ1(-1, -1, -1), XYZ1(0.f, 1.f, 1.f) },
    };

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

    bool MemoryTypeFromProperties(VulkanData &info, 
                                  uint32_t typeBits, 
                                  VkFlags requirementsMask, 
                                  uint32_t* typeIndex) 
    {
        // Search memtypes to find first index with those properties
        for (uint32_t i = 0; i < info.memoryProperties.memoryTypeCount; i++) 
        {
            if ((typeBits & 1) == 1)
            {
                // Type is available, does it match user properties?
                if ((info.memoryProperties.memoryTypes[i].propertyFlags & requirementsMask) == requirementsMask) 
                {
                    *typeIndex = i;
                    return true;
                }
            }
            typeBits >>= 1;
        }
        // No memory types matched, return failure
        return false;
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

const char* VulkanUtils::GetFailText(VkResult result)
{
    switch (result)
    {
    case VK_SUCCESS:
        return "";
    case VK_NOT_READY:
        return "VK_NOT_READY";
    case VK_TIMEOUT:
        return "VK_TIMEOUT";
    case VK_EVENT_SET:
        return "VK_EVENT_SET";
    case VK_EVENT_RESET:
        return "VK_EVENT_RESET";
    case VK_INCOMPLETE:
        return "VK_INCOMPLETE";
    case VK_ERROR_OUT_OF_HOST_MEMORY:
        return "VK_ERROR_OUT_OF_HOSTMEMORY";
    case VK_ERROR_OUT_OF_DEVICE_MEMORY:
        return "VK_ERROR_OUT_OF_DEVICEMEMORY";
    case VK_ERROR_INITIALIZATION_FAILED:
        return "VK_ERROR_INITIALIZATION_FAILED";
    case VK_ERROR_DEVICE_LOST:
        return "VK_ERROR_DEVICE_LOST";
    case VK_ERROR_MEMORY_MAP_FAILED:
        return "VK_ERRORMEMORYMAP_FAILED";
    case VK_ERROR_LAYER_NOT_PRESENT:
        return "VK_ERROR_LAYER_NOTPRESENT";
    case VK_ERROR_EXTENSION_NOT_PRESENT:
        return "VK_ERROR_EXTENSION_NOTPRESENT";
    case VK_ERROR_FEATURE_NOT_PRESENT:
        return "VK_ERROR_FEATURE_NOTPRESENT";
    case VK_ERROR_INCOMPATIBLE_DRIVER:
        return "VK_ERROR_INCOMPATIBLE_DRIVER";
    case VK_ERROR_TOO_MANY_OBJECTS:
        return "VK_ERROR_TOOMANY_OBJECTS";
    case VK_ERROR_FORMAT_NOT_SUPPORTED:
        return "VK_ERROR_FORMAT_NOT_SUPPORTED";
    case VK_ERROR_SURFACE_LOST_KHR:
        return "VK_ERROR_SURFACE_LOST_KHR";
    case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
        return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
    case VK_SUBOPTIMAL_KHR:
        return "VK_SUBOPTIMAL_KHR";
    case VK_ERROR_OUT_OF_DATE_KHR:
        return "VK_ERROR_OUT_OF_DATE_KHR";
    case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
        return "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
    case VK_ERROR_VALIDATION_FAILED_EXT:
        return "VK_ERROR_VALIDATION_FAILED_EXT";
    case VK_ERROR_INVALID_SHADER_NV:
        return "VK_ERROR_INVALID_SHADER_NV";
    default:
        return "UNKNOWN";
    }
}

bool VulkanUtils::LogFail(VkResult result, const char* file, int line)
{
    if (Failed(result))
    {
        std::string error(GetFailText(result));
        Logger::LogError(("Vulkan: %s - %d :" + error).c_str(), file, line);
        return true;
    }
    return false;
}

bool VulkanUtils::Failed(VkResult result)
{
    return result != VK_SUCCESS;
}

VkResult VulkanUtils::InitEnumerateDevice(VulkanData &info)
{
    uint32_t gpuCount = 1;
    uint32_t const reqCount = gpuCount;
    VkResult result = vkEnumeratePhysicalDevices(info.instance, &gpuCount, NULL);
    if (gpuCount < 1)
    {
        return VK_ERROR;
    }
    info.gpus.resize(gpuCount);

    result = vkEnumeratePhysicalDevices(info.instance, &gpuCount, info.gpus.data());
    if (Failed(result))
    {
        return result;
    }

    if (gpuCount < reqCount)
    {
        return VK_ERROR;
    }

    vkGetPhysicalDeviceQueueFamilyProperties(info.gpus[0], &info.queueFamilyCount, NULL);
    if (info.queueFamilyCount < 1)
    {
        return VK_ERROR;
    }

    info.queueProps.resize(info.queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(info.gpus[0], &info.queueFamilyCount, info.queueProps.data());
    if (info.queueFamilyCount < 1)
    {
        return VK_ERROR;
    }

    /* This is as good a place as any to do this */
    vkGetPhysicalDeviceMemoryProperties(info.gpus[0], &info.memoryProperties);
    vkGetPhysicalDeviceProperties(info.gpus[0], &info.gpuProps);

    return result;
}

VkResult VulkanUtils::InitExtensionNames(VulkanData &info)
{
    info.deviceExtensionNames =
    {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
    info.instanceExtensionNames =
    {
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
        VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
    };
    return VK_SUCCESS;
}

VkResult VulkanUtils::InitInstance(VulkanData &info)
{
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

VkResult VulkanUtils::InitDevice(VulkanData &info)
{
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

VkResult VulkanUtils::InitDebugging(VulkanData &info)
{
    if (!GetProcAddress(info.instance, "vkCreateDebugReportCallbackEXT", info.createDebugReportFn) ||
        !GetProcAddress(info.instance, "vkDestroyDebugReportCallbackEXT", info.destroyDebugReportFn))
    {
        return VK_ERROR;
    }

    VkDebugReportCallbackCreateInfoEXT dbgCreateInfo = {};
    dbgCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
    dbgCreateInfo.pfnCallback = (PFN_vkDebugReportCallbackEXT)DebugMessageCallback;
    dbgCreateInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
    return info.createDebugReportFn(info.instance, &dbgCreateInfo, nullptr, &info.debugCallback);
}

VkResult VulkanUtils::InitGlobalLayerProperties(VulkanData& info)
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

VkResult VulkanUtils::InitSwapchainExtension(VulkanData &info)
{
    VkWin32SurfaceCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.pNext = NULL;
    createInfo.hinstance = info.connection;
    createInfo.hwnd = info.window;
    VkResult res = vkCreateWin32SurfaceKHR(info.instance, &createInfo, NULL, &info.surface);
    if (Failed(res))
    {
        return res;
    }

    // Iterate over each queue to learn whether it supports presenting:
    VkBool32 *pSupportsPresent = (VkBool32 *)malloc(info.queueFamilyCount * sizeof(VkBool32));
    for (uint32_t i = 0; i < info.queueFamilyCount; i++) 
    {
        res = vkGetPhysicalDeviceSurfaceSupportKHR(info.gpus[0], i, info.surface, &pSupportsPresent[i]);
        if (Failed(res))
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
        return VK_ERROR;
    }

    // Get the list of VkFormats that are supported:
    uint32_t formatCount;
    res = vkGetPhysicalDeviceSurfaceFormatsKHR(info.gpus[0], info.surface, &formatCount, NULL);
    if (Failed(res))
    {
        return res;
    }

    VkSurfaceFormatKHR *surfFormats = (VkSurfaceFormatKHR *)malloc(formatCount * sizeof(VkSurfaceFormatKHR));
    res = vkGetPhysicalDeviceSurfaceFormatsKHR(info.gpus[0], info.surface, &formatCount, surfFormats);
    if (Failed(res))
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
            return VK_ERROR;
        }
        info.format = surfFormats[0].format;
    }
    free(surfFormats);
    return VK_SUCCESS;
}

VkResult VulkanUtils::InitCommandPool(VulkanData &info)
{
    VkCommandPoolCreateInfo cmdPoolInfo = {};
    cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cmdPoolInfo.pNext = NULL;
    cmdPoolInfo.queueFamilyIndex = info.graphicsQueueFamilyIndex;
    cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    return vkCreateCommandPool(info.device, &cmdPoolInfo, NULL, &info.cmdPool);
}

VkResult VulkanUtils::InitCommandBuffer(VulkanData &info)
{
    VkCommandBufferAllocateInfo cmd = {};
    cmd.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmd.pNext = NULL;
    cmd.commandPool = info.cmdPool;
    cmd.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    cmd.commandBufferCount = 1;

    return vkAllocateCommandBuffers(info.device, &cmd, &info.cmd);
}

VkResult VulkanUtils::InitDeviceQueue(VulkanData &info)
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
    return VK_SUCCESS;
}

VkResult VulkanUtils::InitSwapChain(VulkanData &info)
{
    VkImageUsageFlags usageFlags =
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
        VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

    VkSurfaceCapabilitiesKHR surfCapabilities;

    VkResult res = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(info.gpus[0], info.surface, &surfCapabilities);
    if (Failed(res))
    {
        return res;
    }

    uint32_t presentModeCount;
    res = vkGetPhysicalDeviceSurfacePresentModesKHR(info.gpus[0], info.surface, &presentModeCount, NULL);
    if (Failed(res))
    {
        return res;
    }

    VkPresentModeKHR *presentModes = (VkPresentModeKHR *)malloc(presentModeCount * sizeof(VkPresentModeKHR));
    if (!presentModes)
    {
        return VK_ERROR;
    }

    res = vkGetPhysicalDeviceSurfacePresentModesKHR(info.gpus[0], info.surface, &presentModeCount, presentModes);
    if (Failed(res))
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
    if (Failed(res))
    {
        return res;
    }

    res = vkGetSwapchainImagesKHR(info.device, info.swapChain, &info.swapchainImageCount, NULL);
    if (Failed(res))
    {
        return res;
    }

    VkImage *swapchainImages = (VkImage *)malloc(info.swapchainImageCount * sizeof(VkImage));
    if (!swapchainImages)
    {
        return VK_ERROR;
    }

    res = vkGetSwapchainImagesKHR(info.device, info.swapChain, &info.swapchainImageCount, swapchainImages);
    if (Failed(res))
    {
        return res;
    }

    for (uint32_t i = 0; i < info.swapchainImageCount; i++) 
    {
        SwapChainBuffer scBuffer;

        VkImageViewCreateInfo colorImage_view = {};
        colorImage_view.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        colorImage_view.pNext = NULL;
        colorImage_view.format = info.format;
        colorImage_view.components.r = VK_COMPONENT_SWIZZLE_R;
        colorImage_view.components.g = VK_COMPONENT_SWIZZLE_G;
        colorImage_view.components.b = VK_COMPONENT_SWIZZLE_B;
        colorImage_view.components.a = VK_COMPONENT_SWIZZLE_A;
        colorImage_view.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        colorImage_view.subresourceRange.baseMipLevel = 0;
        colorImage_view.subresourceRange.levelCount = 1;
        colorImage_view.subresourceRange.baseArrayLayer = 0;
        colorImage_view.subresourceRange.layerCount = 1;
        colorImage_view.viewType = VK_IMAGE_VIEW_TYPE_2D;
        colorImage_view.flags = 0;

        scBuffer.image = swapchainImages[i];

        colorImage_view.image = scBuffer.image;

        res = vkCreateImageView(info.device, &colorImage_view, NULL, &scBuffer.view);
        info.buffers.push_back(scBuffer);
        if (Failed(res))
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

VkResult VulkanUtils::InitDepthBuffer(VulkanData &info)
{
    VkImageCreateInfo imageInfo = {};

    /* allow custom depth formats */
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
        return VK_ERROR;
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
    imageInfo.samples = NUM_SAMPLES;
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

    VkMemoryRequirements mem_reqs;

    /* Create image */
    VkResult res = vkCreateImage(info.device, &imageInfo, NULL, &info.depth.image);
    if (Failed(res))
    {
        return res;
    }

    vkGetImageMemoryRequirements(info.device, info.depth.image, &mem_reqs);

    memAlloc.allocationSize = mem_reqs.size;

    /* Use the memory properties to determine the type of memory required */
    if (!MemoryTypeFromProperties(info, mem_reqs.memoryTypeBits, 0, &memAlloc.memoryTypeIndex))
    {
        return VK_ERROR;
    }

    /* Allocate memory */
    res = vkAllocateMemory(info.device, &memAlloc, NULL, &info.depth.memory);
    if (Failed(res))
    {
        return res;
    }

    /* Bind memory */
    res = vkBindImageMemory(info.device, info.depth.image, info.depth.memory, 0);
    if (Failed(res))
    {
        return res;
    }

    /* Create image view */
    viewInfo.image = info.depth.image;
    return vkCreateImageView(info.device, &viewInfo, NULL, &info.depth.view);
}

VkResult VulkanUtils::InitUniformBuffer(VulkanData& info)
{
    float fov = glm::radians(45.0f);
    if (WINDOW_WIDTH > WINDOW_HEIGHT) 
    {
        fov *= static_cast<float>(WINDOW_HEIGHT) / static_cast<float>(WINDOW_WIDTH);
    }

    info.projection = glm::perspective(fov, static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT), 0.1f, 100.0f);
    
    info.view = 
        glm::lookAt(glm::vec3(-5, 3, -10),  // Camera is at (-5,3,-10), in World Space
        glm::vec3(0, 0, 0),     // and looks at the origin
        glm::vec3(0, -1, 0)     // Head is up (set to 0,-1,0 to look upside-down)
    );
    info.model = glm::mat4(1.0f);

    // Vulkan clip space has inverted Y and half Z.
    info.clip = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.5f, 1.0f);

    info.mvp = info.clip * info.projection * info.view * info.model;

    VkBufferCreateInfo bufInfo = {};
    bufInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufInfo.pNext = NULL;
    bufInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    bufInfo.size = sizeof(info.mvp);
    bufInfo.queueFamilyIndexCount = 0;
    bufInfo.pQueueFamilyIndices = NULL;
    bufInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufInfo.flags = 0;
    VkResult res = vkCreateBuffer(info.device, &bufInfo, NULL, &info.uniformData.buffer);
    if (Failed(res))
    {
        return res;
    }

    VkMemoryRequirements mem_reqs;
    vkGetBufferMemoryRequirements(info.device, info.uniformData.buffer, &mem_reqs);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.pNext = NULL;
    allocInfo.memoryTypeIndex = 0;

    allocInfo.allocationSize = mem_reqs.size;
    bool pass = MemoryTypeFromProperties(info, mem_reqs.memoryTypeBits,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        &allocInfo.memoryTypeIndex);
    if (!pass)
    {
        Logger::LogError("Vulkan: No mappable, coherent memory");
        return VK_ERROR;
    }

    res = vkAllocateMemory(info.device, &allocInfo, NULL, &(info.uniformData.memory));
    if (Failed(res))
    {
        return res;
    }

    uint8_t *pData;
    res = vkMapMemory(info.device, info.uniformData.memory, 0, mem_reqs.size, 0, (void **)&pData);
    if (Failed(res))
    {
        return res;
    }

    memcpy(pData, &info.mvp, sizeof(info.mvp));

    vkUnmapMemory(info.device, info.uniformData.memory);

    res = vkBindBufferMemory(info.device, info.uniformData.buffer, info.uniformData.memory, 0);
    if (Failed(res))
    {
        return res;
    }

    info.uniformData.bufferInfo.buffer = info.uniformData.buffer;
    info.uniformData.bufferInfo.offset = 0;
    info.uniformData.bufferInfo.range = sizeof(info.mvp);
    return VK_SUCCESS;
}

VkResult VulkanUtils::InitDescriptorAndPipelineLayouts(VulkanData &info)
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

    /* Next take layout bindings and use them to create a descriptor set layout
    */
    VkDescriptorSetLayoutCreateInfo descriptorLayout = {};
    descriptorLayout.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptorLayout.pNext = NULL;
    descriptorLayout.bindingCount = use_texture ? 2 : 1;
    descriptorLayout.pBindings = layoutBindings;

    info.descLayout.resize(NUM_DESCRIPTOR_SETS);
    VkResult res = vkCreateDescriptorSetLayout(info.device, &descriptorLayout, NULL, info.descLayout.data());
    if (Failed(res))
    {
        return res;
    }

    /* Now use the descriptor layout to create a pipeline layout */
    VkPipelineLayoutCreateInfo pPipelineLayoutCreateInfo = {};
    pPipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pPipelineLayoutCreateInfo.pNext = NULL;
    pPipelineLayoutCreateInfo.pushConstantRangeCount = 0;
    pPipelineLayoutCreateInfo.pPushConstantRanges = NULL;
    pPipelineLayoutCreateInfo.setLayoutCount = NUM_DESCRIPTOR_SETS;
    pPipelineLayoutCreateInfo.pSetLayouts = info.descLayout.data();

    return vkCreatePipelineLayout(info.device, &pPipelineLayoutCreateInfo, NULL, &info.pipelineLayout);
}

VkResult VulkanUtils::InitPipeline(VulkanData& info)
{
    VkPipelineCacheCreateInfo pipelineCache;
    pipelineCache.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    pipelineCache.pNext = NULL;
    pipelineCache.initialDataSize = 0;
    pipelineCache.pInitialData = NULL;
    pipelineCache.flags = 0;
    VkResult res = vkCreatePipelineCache(info.device, &pipelineCache, NULL, &info.pipelineCache);
    if (Failed(res))
    {
        return res;
    }

    VkBool32 includeDepth = true;
    VkBool32 include_vi = true;

    VkDynamicState dynamicStateEnables[VK_DYNAMIC_STATE_RANGE_SIZE];
    VkPipelineDynamicStateCreateInfo dynamicState = {};
    memset(dynamicStateEnables, 0, sizeof dynamicStateEnables);
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.pNext = NULL;
    dynamicState.pDynamicStates = dynamicStateEnables;
    dynamicState.dynamicStateCount = 0;

    VkPipelineVertexInputStateCreateInfo vi;
    memset(&vi, 0, sizeof(vi));
    vi.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    if (include_vi)
    {
        vi.pNext = NULL;
        vi.flags = 0;
        vi.vertexBindingDescriptionCount = 1;
        vi.pVertexBindingDescriptions = &info.viBinding;
        vi.vertexAttributeDescriptionCount = info.viAttribs.size();
        vi.pVertexAttributeDescriptions = &info.viAttribs[0];
    }
    VkPipelineInputAssemblyStateCreateInfo ia;
    ia.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    ia.pNext = NULL;
    ia.flags = 0;
    ia.primitiveRestartEnable = VK_FALSE;
    ia.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    VkPipelineRasterizationStateCreateInfo rs;
    rs.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rs.pNext = NULL;
    rs.flags = 0;
    rs.polygonMode = VK_POLYGON_MODE_FILL;
    rs.cullMode = VK_CULL_MODE_BACK_BIT;
    rs.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rs.depthClampEnable = includeDepth;
    rs.rasterizerDiscardEnable = VK_FALSE;
    rs.depthBiasEnable = VK_FALSE;
    rs.depthBiasConstantFactor = 0;
    rs.depthBiasClamp = 0;
    rs.depthBiasSlopeFactor = 0;
    rs.lineWidth = 1.0f;

    VkPipelineColorBlendStateCreateInfo cb;
    cb.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    cb.flags = 0;
    cb.pNext = NULL;

    VkPipelineColorBlendAttachmentState attState[1];
    attState[0].colorWriteMask = 0xf;
    attState[0].blendEnable = VK_FALSE;
    attState[0].alphaBlendOp = VK_BLEND_OP_ADD;
    attState[0].colorBlendOp = VK_BLEND_OP_ADD;
    attState[0].srcColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    attState[0].dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    attState[0].srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    attState[0].dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    cb.attachmentCount = 1;
    cb.pAttachments = attState;
    cb.logicOpEnable = VK_FALSE;
    cb.logicOp = VK_LOGIC_OP_NO_OP;
    cb.blendConstants[0] = 1.0f;
    cb.blendConstants[1] = 1.0f;
    cb.blendConstants[2] = 1.0f;
    cb.blendConstants[3] = 1.0f;

    VkPipelineViewportStateCreateInfo vp = {};
    vp.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    vp.pNext = NULL;
    vp.flags = 0;
    vp.viewportCount = NUM_VIEWPORTS_AND_SCISSORS;
    dynamicStateEnables[dynamicState.dynamicStateCount++] = VK_DYNAMIC_STATE_VIEWPORT;
    vp.scissorCount = NUM_VIEWPORTS_AND_SCISSORS;
    dynamicStateEnables[dynamicState.dynamicStateCount++] = VK_DYNAMIC_STATE_SCISSOR;
    vp.pScissors = NULL;
    vp.pViewports = NULL;

    VkPipelineDepthStencilStateCreateInfo ds;
    ds.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    ds.pNext = NULL;
    ds.flags = 0;
    ds.depthTestEnable = includeDepth;
    ds.depthWriteEnable = includeDepth;
    ds.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
    ds.depthBoundsTestEnable = VK_FALSE;
    ds.stencilTestEnable = VK_FALSE;
    ds.back.failOp = VK_STENCIL_OP_KEEP;
    ds.back.passOp = VK_STENCIL_OP_KEEP;
    ds.back.compareOp = VK_COMPARE_OP_ALWAYS;
    ds.back.compareMask = 0;
    ds.back.reference = 0;
    ds.back.depthFailOp = VK_STENCIL_OP_KEEP;
    ds.back.writeMask = 0;
    ds.minDepthBounds = 0;
    ds.maxDepthBounds = 0;
    ds.stencilTestEnable = VK_FALSE;
    ds.front = ds.back;

    VkPipelineMultisampleStateCreateInfo ms;
    ms.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    ms.pNext = NULL;
    ms.flags = 0;
    ms.pSampleMask = NULL;
    ms.rasterizationSamples = NUM_SAMPLES;
    ms.sampleShadingEnable = VK_FALSE;
    ms.alphaToCoverageEnable = VK_FALSE;
    ms.alphaToOneEnable = VK_FALSE;
    ms.minSampleShading = 0.0;

    VkGraphicsPipelineCreateInfo pipeline;
    pipeline.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipeline.pNext = NULL;
    pipeline.layout = info.pipelineLayout;
    pipeline.basePipelineHandle = VK_NULL_HANDLE;
    pipeline.basePipelineIndex = 0;
    pipeline.flags = 0;
    pipeline.pVertexInputState = &vi;
    pipeline.pInputAssemblyState = &ia;
    pipeline.pRasterizationState = &rs;
    pipeline.pColorBlendState = &cb;
    pipeline.pTessellationState = NULL;
    pipeline.pMultisampleState = &ms;
    pipeline.pDynamicState = &dynamicState;
    pipeline.pViewportState = &vp;
    pipeline.pDepthStencilState = &ds;
    pipeline.pStages = &info.shaderStages[0];
    pipeline.stageCount = info.shaderStages.size();
    pipeline.renderPass = info.renderPass;
    pipeline.subpass = 0;

    return vkCreateGraphicsPipelines(info.device, info.pipelineCache, 1, &pipeline, NULL, &info.pipeline);
}

VkResult VulkanUtils::InitDescriptorPool(VulkanData& info)
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

VkResult VulkanUtils::InitDescriptorSet(VulkanData& info)
{
    bool use_texture = false;

    VkDescriptorSetAllocateInfo allocInfo[1];
    allocInfo[0].sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo[0].pNext = NULL;
    allocInfo[0].descriptorPool = info.descPool;
    allocInfo[0].descriptorSetCount = NUM_DESCRIPTOR_SETS;
    allocInfo[0].pSetLayouts = info.descLayout.data();

    info.descSet.resize(NUM_DESCRIPTOR_SETS);
    VkResult res = vkAllocateDescriptorSets(info.device, allocInfo, info.descSet.data());
    if (Failed(res))
    {
        return res;
    }

    VkWriteDescriptorSet writes[2];

    writes[0] = {};
    writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writes[0].pNext = NULL;
    writes[0].dstSet = info.descSet[0];
    writes[0].descriptorCount = 1;
    writes[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    writes[0].pBufferInfo = &info.uniformData.bufferInfo;
    writes[0].dstArrayElement = 0;
    writes[0].dstBinding = 0;

    if (use_texture) 
    {
        writes[1] = {};
        writes[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writes[1].dstSet = info.descSet[0];
        writes[1].dstBinding = 1;
        writes[1].descriptorCount = 1;
        writes[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        writes[1].pImageInfo = &info.imageInfo;
        writes[1].dstArrayElement = 0;
    }

    vkUpdateDescriptorSets(info.device, use_texture ? 2 : 1, writes, 0, NULL);
    return VK_SUCCESS;
}

VkResult VulkanUtils::InitVertexBuffer(VulkanData& info)
{
    const void* vertexData = g_vbSolidFaceColors_Data;
    uint32_t dataSize = sizeof(g_vbSolidFaceColors_Data);
    uint32_t dataStride = sizeof(g_vbSolidFaceColors_Data[0]);
    bool use_texture = false;

    VkBufferCreateInfo bufInfo = {};
    bufInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufInfo.pNext = NULL;
    bufInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufInfo.size = dataSize;
    bufInfo.queueFamilyIndexCount = 0;
    bufInfo.pQueueFamilyIndices = NULL;
    bufInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufInfo.flags = 0;
    VkResult res = vkCreateBuffer(info.device, &bufInfo, NULL, &info.vertexBuffer.buffer);
    if (Failed(res))
    {
        return res;
    }

    VkMemoryRequirements mem_reqs;
    vkGetBufferMemoryRequirements(info.device, info.vertexBuffer.buffer, &mem_reqs);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.pNext = NULL;
    allocInfo.memoryTypeIndex = 0;

    allocInfo.allocationSize = mem_reqs.size;
    bool pass = MemoryTypeFromProperties(info, mem_reqs.memoryTypeBits,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        &allocInfo.memoryTypeIndex);
    if (!pass)
    {
        Logger::LogError("Vulkan: No mappable, coherent memory");
        return VK_ERROR;
    }

    res = vkAllocateMemory(info.device, &allocInfo, NULL, &(info.vertexBuffer.memory));
    if (Failed(res))
    {
        return res;
    }

    info.vertexBuffer.bufferInfo.range = mem_reqs.size;
    info.vertexBuffer.bufferInfo.offset = 0;

    uint8_t *pData;
    res = vkMapMemory(info.device, info.vertexBuffer.memory, 0, mem_reqs.size, 0, (void **)&pData);
    if (Failed(res))
    {
        return res;
    }

    memcpy(pData, vertexData, dataSize);

    vkUnmapMemory(info.device, info.vertexBuffer.memory);

    res = vkBindBufferMemory(info.device, info.vertexBuffer.buffer, info.vertexBuffer.memory, 0);
    if (Failed(res))
    {
        return res;
    }

    info.viBinding.binding = 0;
    info.viBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    info.viBinding.stride = dataStride;

    info.viAttribs[0].binding = 0;
    info.viAttribs[0].location = 0;
    info.viAttribs[0].format = VK_FORMAT_R32G32B32A32_SFLOAT;
    info.viAttribs[0].offset = 0;
    info.viAttribs[1].binding = 0;
    info.viAttribs[1].location = 1;
    info.viAttribs[1].format = use_texture ? VK_FORMAT_R32G32_SFLOAT : VK_FORMAT_R32G32B32A32_SFLOAT;
    info.viAttribs[1].offset = 16;

    return VK_SUCCESS;
}

VkResult VulkanUtils::InitRenderpass(VulkanData &info)
{
    bool includeDepth = true;
    bool clear = true;
    VkImageLayout finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    /* Need attachments for render target and depth buffer */
    VkAttachmentDescription attachments[2];
    attachments[0].format = info.format;
    attachments[0].samples = NUM_SAMPLES;
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
        attachments[1].samples = NUM_SAMPLES;
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

    return vkCreateRenderPass(info.device, &rpInfo, NULL, &info.renderPass);
}

VkResult VulkanUtils::InitFramebuffers(VulkanData &info)
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
        if (Failed(res))
        {
            return res;
        }
    }
    return VK_SUCCESS;
}

VkResult VulkanUtils::InitViewports(VulkanData& info)
{
    info.clearValues[0].color.float32[0] = 1.0f;
    info.clearValues[0].color.float32[1] = 0.0f;
    info.clearValues[0].color.float32[2] = 0.0f;
    info.clearValues[0].color.float32[3] = 1.0f;
    info.clearValues[1].depthStencil.depth = 1.0f;
    info.clearValues[1].depthStencil.stencil = 0;

    info.viewport.height = (float)WINDOW_HEIGHT;
    info.viewport.width = (float)WINDOW_WIDTH;
    info.viewport.minDepth = 0.0f;
    info.viewport.maxDepth = 1.0f;
    info.viewport.x = 0;
    info.viewport.y = 0;
    vkCmdSetViewport(info.cmd, 0, NUM_VIEWPORTS_AND_SCISSORS, &info.viewport);
    return VK_SUCCESS;
}

VkResult VulkanUtils::InitScissors(VulkanData& info)
{
    info.scissor.extent.width = WINDOW_WIDTH;
    info.scissor.extent.height = WINDOW_HEIGHT;
    info.scissor.offset.x = 0;
    info.scissor.offset.y = 0;
    vkCmdSetScissor(info.cmd, 0, NUM_VIEWPORTS_AND_SCISSORS, &info.scissor);
    return VK_SUCCESS;
}

VkResult VulkanUtils::InitSemaphores(VulkanData& info)
{
    VkSemaphoreCreateInfo imageAcquiredSemaphoreCreateInfo;
    imageAcquiredSemaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    imageAcquiredSemaphoreCreateInfo.pNext = NULL;
    imageAcquiredSemaphoreCreateInfo.flags = 0;
    return vkCreateSemaphore(info.device, &imageAcquiredSemaphoreCreateInfo, NULL, &info.imageAcquiredSemaphore);
}

VkResult VulkanUtils::InitFence(VulkanData& info)
{
    VkFenceCreateInfo fenceInfo;
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.pNext = NULL;
    fenceInfo.flags = 0;
    return vkCreateFence(info.device, &fenceInfo, NULL, &info.drawFence);
}

void VulkanUtils::BeginCommandBuffer(VulkanData &info)
{
    VkCommandBufferBeginInfo cmdBufInfo = {};
    cmdBufInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmdBufInfo.pNext = NULL;
    cmdBufInfo.flags = 0;
    cmdBufInfo.pInheritanceInfo = NULL;
    FAILED(vkBeginCommandBuffer(info.cmd, &cmdBufInfo));
}

void VulkanUtils::EndCommandBuffer(VulkanData &info)
{
    FAILED(vkEndCommandBuffer(info.cmd));
}
