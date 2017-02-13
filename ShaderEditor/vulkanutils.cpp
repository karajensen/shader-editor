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

VkResult init_instance(VulkanData &info)
{
    info.instance_extension_names =
    {
        VK_EXT_DEBUG_REPORT_EXTENSION_NAME
    };

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

