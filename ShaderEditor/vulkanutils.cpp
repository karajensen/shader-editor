////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkanutils.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "vulkanutils.h"
#include "vulkandata.h"
#include "logger.h"

const VkResult VK_ERROR = VK_ERROR_INITIALIZATION_FAILED;

bool log_fail(VkResult result, const char* file, int line)
{
    switch (result)
    {
    case VK_SUCCESS:
        return false;
    case VK_NOT_READY:
        Logger::LogError("Vulkan: %s - %d: VK_NOT_READY", file, line);
        return true;
    case VK_TIMEOUT:
        Logger::LogError("Vulkan: %s - %d: VK_TIMEOUT", file, line);
        return true;
    case VK_EVENT_SET:
        Logger::LogError("Vulkan: %s - %d: VK_EVENT_SET", file, line);
        return true;
    case VK_EVENT_RESET:
        Logger::LogError("Vulkan: %s - %d: VK_EVENT_RESET", file, line);
        return true;
    case VK_INCOMPLETE:
        Logger::LogError("Vulkan: %s - %d: VK_INCOMPLETE", file, line);
        return true;
    case VK_ERROR_OUT_OF_HOST_MEMORY:
        Logger::LogError("Vulkan: %s - %d: VK_ERROR_OUT_OF_HOST_MEMORY", file, line);
        return true;
    case VK_ERROR_OUT_OF_DEVICE_MEMORY:
        Logger::LogError("Vulkan: %s - %d: VK_ERROR_OUT_OF_DEVICE_MEMORY", file, line);
        return true;
    case VK_ERROR_INITIALIZATION_FAILED:
        Logger::LogError("Vulkan: %s - %d: VK_ERROR_INITIALIZATION_FAILED", file, line);
        return true;
    case VK_ERROR_DEVICE_LOST:
        Logger::LogError("Vulkan: %s - %d: VK_ERROR_DEVICE_LOST", file, line);
        return true;
    case VK_ERROR_MEMORY_MAP_FAILED:
        Logger::LogError("Vulkan: %s - %d: VK_ERROR_MEMORY_MAP_FAILED", file, line);
        return true;
    case VK_ERROR_LAYER_NOT_PRESENT:
        Logger::LogError("Vulkan: %s - %d: VK_ERROR_LAYER_NOT_PRESENT", file, line);
        return true;
    case VK_ERROR_EXTENSION_NOT_PRESENT:
        Logger::LogError("Vulkan: %s - %d: VK_ERROR_EXTENSION_NOT_PRESENT", file, line);
        return true;
    case VK_ERROR_FEATURE_NOT_PRESENT:
        Logger::LogError("Vulkan: %s - %d: VK_ERROR_FEATURE_NOT_PRESENT", file, line);
        return true;
    case VK_ERROR_INCOMPATIBLE_DRIVER:
        Logger::LogError("Vulkan: %s - %d: VK_ERROR_INCOMPATIBLE_DRIVER", file, line);
        return true;
    case VK_ERROR_TOO_MANY_OBJECTS:
        Logger::LogError("Vulkan: %s - %d: VK_ERROR_TOO_MANY_OBJECTS", file, line);
        return true;
    case VK_ERROR_FORMAT_NOT_SUPPORTED:
        Logger::LogError("Vulkan: %s - %d: VK_ERROR_FORMAT_NOT_SUPPORTED", file, line);
        return true;
    case VK_ERROR_SURFACE_LOST_KHR:
        Logger::LogError("Vulkan: %s - %d: VK_ERROR_SURFACE_LOST_KHR", file, line);
        return true;
    case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
        Logger::LogError("Vulkan: %s - %d: VK_ERROR_NATIVE_WINDOW_IN_USE_KHR", file, line);
        return true;
    case VK_SUBOPTIMAL_KHR:
        Logger::LogError("Vulkan: %s - %d: VK_SUBOPTIMAL_KHR", file, line);
        return true;
    case VK_ERROR_OUT_OF_DATE_KHR:
        Logger::LogError("Vulkan: %s - %d: VK_ERROR_OUT_OF_DATE_KHR", file, line);
        return true;
    case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
        Logger::LogError("Vulkan: %s - %d: VK_ERROR_INCOMPATIBLE_DISPLAY_KHR", file, line);
        return true;
    case VK_ERROR_VALIDATION_FAILED_EXT:
        Logger::LogError("Vulkan: %s - %d: VK_ERROR_VALIDATION_FAILED_EXT", file, line);
        return true;
    case VK_ERROR_INVALID_SHADER_NV:
        Logger::LogError("Vulkan: %s - %d: VK_ERROR_INVALID_SHADER_NV", file, line);
        return true;
    default:
        Logger::LogError("Vulkan: %s - %d: UNKNOWN", file, line);
        return true;
    }
}

bool failed(VkResult result)
{
    return result != VK_SUCCESS;
}

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

VkBool32 debug_message_callback(VkDebugReportFlagsEXT flags,
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

VkResult init_enumerate_device(VulkanData &info, uint32_t gpu_count) 
{
    uint32_t const req_count = gpu_count;
    VkResult result = vkEnumeratePhysicalDevices(info.instance, &gpu_count, NULL);
    if (gpu_count < 1)
    {
        return VK_ERROR;
    }
    info.gpus.resize(gpu_count);

    result = vkEnumeratePhysicalDevices(info.instance, &gpu_count, info.gpus.data());
    if (failed(result))
    {
        return result;
    }

    if (gpu_count < req_count)
    {
        return VK_ERROR;
    }

    vkGetPhysicalDeviceQueueFamilyProperties(info.gpus[0], &info.queue_family_count, NULL);
    if (info.queue_family_count < 1)
    {
        return VK_ERROR;
    }

    info.queue_props.resize(info.queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(info.gpus[0], &info.queue_family_count, info.queue_props.data());
    if (info.queue_family_count < 1)
    {
        return VK_ERROR;
    }

    /* This is as good a place as any to do this */
    vkGetPhysicalDeviceMemoryProperties(info.gpus[0], &info.memory_properties);
    vkGetPhysicalDeviceProperties(info.gpus[0], &info.gpu_props);

    return result;
}

VkResult init_global_extension_properties(LayerProperties& layer_props) 
{
    VkExtensionProperties* instance_extensions = nullptr;
    uint32_t instance_extension_count;
    VkResult result;
    char* layer_name = nullptr;
    layer_name = layer_props.properties.layerName;

    do 
    {
        result = vkEnumerateInstanceExtensionProperties(layer_name, &instance_extension_count, nullptr);
        if (result)
        {
            return result;
        }

        if (instance_extension_count == 0) 
        {
            return VK_SUCCESS;
        }

        layer_props.extensions.resize(instance_extension_count);
        instance_extensions = layer_props.extensions.data();
        result = vkEnumerateInstanceExtensionProperties(layer_name, &instance_extension_count, instance_extensions);

    } while (result == VK_INCOMPLETE);

    return result;
}

VkResult init_window_size(VulkanData &info) 
{
    info.width = WINDOW_WIDTH;
    info.height = WINDOW_HEIGHT;
    return VK_SUCCESS;
}

VkResult init_device_extension_names(VulkanData &info)
{
    info.device_extension_names =
    {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
    return VK_SUCCESS;
}

VkResult init_instance_extension_names(VulkanData &info)
{
    info.instance_extension_names =
    {
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
        VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
    };
    return VK_SUCCESS;
}

VkResult init_instance(VulkanData &info)
{
    VkApplicationInfo app_info = {};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pNext = NULL;
    app_info.pApplicationName = "";
    app_info.applicationVersion = 1;
    app_info.pEngineName = "";
    app_info.engineVersion = 1;
    app_info.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo inst_info = {};
    inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    inst_info.pNext = NULL;
    inst_info.flags = 0;
    inst_info.pApplicationInfo = &app_info;
    inst_info.enabledLayerCount = info.instance_layer_names.size();
    inst_info.ppEnabledLayerNames = info.instance_layer_names.size() ? info.instance_layer_names.data() : NULL;
    inst_info.enabledExtensionCount = info.instance_extension_names.size();
    inst_info.ppEnabledExtensionNames = info.instance_extension_names.data();

    return vkCreateInstance(&inst_info, NULL, &info.instance);
}

VkResult init_queue_family_index(VulkanData &info)
{
    // This routine simply finds a graphics queue for a later vkCreateDevice,
    // without consideration for which queue family can present an image.
    // Do not use this if your intent is to present later in your sample,
    // instead use the init_connection, init_window, init_swapchain_extension,
    // init_device call sequence to get a graphics and present compatible queue family

    vkGetPhysicalDeviceQueueFamilyProperties(info.gpus[0], &info.queue_family_count, NULL);
    assert(info.queue_family_count >= 1);

    info.queue_props.resize(info.queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(info.gpus[0], &info.queue_family_count, info.queue_props.data());
    assert(info.queue_family_count >= 1);

    bool found = false;
    for (unsigned int i = 0; i < info.queue_family_count; i++) {
        if (info.queue_props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            info.graphics_queue_family_index = i;
            found = true;
            break;
        }
    }
    
    return found ? VK_SUCCESS : VK_ERROR_VALIDATION_FAILED_EXT;
}

VkResult init_device(VulkanData &info) 
{
    VkDeviceQueueCreateInfo queue_info = {};

    float queue_priorities[1] = { 0.0 };
    queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_info.pNext = NULL;
    queue_info.queueCount = 1;
    queue_info.pQueuePriorities = queue_priorities;
    queue_info.queueFamilyIndex = info.graphics_queue_family_index;

    VkDeviceCreateInfo device_info = {};
    device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_info.pNext = NULL;
    device_info.queueCreateInfoCount = 1;
    device_info.pQueueCreateInfos = &queue_info;
    device_info.enabledExtensionCount = info.device_extension_names.size();
    device_info.ppEnabledExtensionNames = device_info.enabledExtensionCount ? info.device_extension_names.data() : NULL;
    device_info.pEnabledFeatures = NULL;

    return vkCreateDevice(info.gpus[0], &device_info, NULL, &info.device);
}

VkResult init_debugging(VulkanData &info)
{
    if (!GetProcAddress(info.instance, "vkCreateDebugReportCallbackEXT", info.CreateDebugReportFn) ||
        !GetProcAddress(info.instance, "vkDestroyDebugReportCallbackEXT", info.DestroyDebugReportFn))
    {
        return VK_ERROR;
    }

    VkDebugReportCallbackCreateInfoEXT dbgCreateInfo = {};
    dbgCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
    dbgCreateInfo.pfnCallback = (PFN_vkDebugReportCallbackEXT)debug_message_callback;
    dbgCreateInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
    return info.CreateDebugReportFn(info.instance, &dbgCreateInfo, nullptr, &info.debug_callback);
}

VkResult init_global_layer_properties(VulkanData& info)
{
    uint32_t instance_layer_count;
    VkLayerProperties *vk_props = NULL;
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
        result = vkEnumerateInstanceLayerProperties(&instance_layer_count, NULL);
        if (result)
        {
            return result;
        }

        if (instance_layer_count == 0) 
        {
            return VK_SUCCESS;
        }

        vk_props = (VkLayerProperties *)realloc(vk_props, instance_layer_count * sizeof(VkLayerProperties));

        result = vkEnumerateInstanceLayerProperties(&instance_layer_count, vk_props);
    } while (result == VK_INCOMPLETE);

    // Now gather the extension list for each instance layer.
    for (uint32_t i = 0; i < instance_layer_count; i++) 
    {
        LayerProperties layer_props;
        layer_props.properties = vk_props[i];
        result = init_global_extension_properties(layer_props);
        if (result)
        {
            return result;
        }
        info.instance_layer_properties.push_back(layer_props);
    }

    free(vk_props);
    return result;
}

VkResult init_swapchain_extension(VulkanData &info) 
{
    VkWin32SurfaceCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.pNext = NULL;
    createInfo.hinstance = info.connection;
    createInfo.hwnd = info.window;
    VkResult res = vkCreateWin32SurfaceKHR(info.instance, &createInfo, NULL, &info.surface);
    if (failed(res))
    {
        return res;
    }

    // Iterate over each queue to learn whether it supports presenting:
    VkBool32 *pSupportsPresent = (VkBool32 *)malloc(info.queue_family_count * sizeof(VkBool32));
    for (uint32_t i = 0; i < info.queue_family_count; i++) 
    {
        vkGetPhysicalDeviceSurfaceSupportKHR(info.gpus[0], i, info.surface, &pSupportsPresent[i]);
    }

    // Search for a graphics and a present queue in the array of queue
    // families, try to find one that supports both
    info.graphics_queue_family_index = UINT32_MAX;
    info.present_queue_family_index = UINT32_MAX;
    for (uint32_t i = 0; i < info.queue_family_count; ++i) 
    {
        if ((info.queue_props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0)
        {
            if (info.graphics_queue_family_index == UINT32_MAX) info.graphics_queue_family_index = i;

            if (pSupportsPresent[i] == VK_TRUE) 
            {
                info.graphics_queue_family_index = i;
                info.present_queue_family_index = i;
                break;
            }
        }
    }

    if (info.present_queue_family_index == UINT32_MAX) 
    {
        // If didn't find a queue that supports both graphics and present, then
        // find a separate present queue.
        for (size_t i = 0; i < info.queue_family_count; ++i)
        {
            if (pSupportsPresent[i] == VK_TRUE)
            {
                info.present_queue_family_index = i;
                break;
            }
        }
    }
    free(pSupportsPresent);

    // Generate error if could not find queues that support graphics
    // and present
    if (info.graphics_queue_family_index == UINT32_MAX || info.present_queue_family_index == UINT32_MAX)
    {
        Logger::LogError("Vulkan: Could not find a queues for both graphics and present");
        return VK_ERROR;
    }

    // Get the list of VkFormats that are supported:
    uint32_t formatCount;
    res = vkGetPhysicalDeviceSurfaceFormatsKHR(info.gpus[0], info.surface, &formatCount, NULL);
    if (failed(res))
    {
        return res;
    }

    VkSurfaceFormatKHR *surfFormats = (VkSurfaceFormatKHR *)malloc(formatCount * sizeof(VkSurfaceFormatKHR));
    res = vkGetPhysicalDeviceSurfaceFormatsKHR(info.gpus[0], info.surface, &formatCount, surfFormats);
    if (failed(res))
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

bool memory_type_from_properties(VulkanData &info, 
                                 uint32_t typeBits, 
                                 VkFlags requirements_mask, 
                                 uint32_t* typeIndex) 
{
    // Search memtypes to find first index with those properties
    for (uint32_t i = 0; i < info.memory_properties.memoryTypeCount; i++) 
    {
        if ((typeBits & 1) == 1)
        {
            // Type is available, does it match user properties?
            if ((info.memory_properties.memoryTypes[i].propertyFlags & requirements_mask) == requirements_mask) 
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