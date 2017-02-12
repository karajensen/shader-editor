////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkanutils.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "vulkanutils.h"
#include "vulkandata.h"
#include "logger.h"

bool failed(VkResult result)
{
    return result != VK_SUCCESS;
}

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

VkResult init_enumerate_device(VulkanData &info, uint32_t gpu_count) 
{
    uint32_t const req_count = gpu_count;
    VkResult result = vkEnumeratePhysicalDevices(info.instance, &gpu_count, NULL);
    if (gpu_count < 1)
    {
        return VK_ERROR_FORMAT_NOT_SUPPORTED;
    }
    info.gpus.resize(gpu_count);

    result = vkEnumeratePhysicalDevices(info.instance, &gpu_count, info.gpus.data());
    if (failed(result))
    {
        return result;
    }

    if (gpu_count < req_count)
    {
        return VK_ERROR_FORMAT_NOT_SUPPORTED;
    }

    vkGetPhysicalDeviceQueueFamilyProperties(info.gpus[0], &info.queue_family_count, NULL);
    if (info.queue_family_count < 1)
    {
        return VK_ERROR_FORMAT_NOT_SUPPORTED;
    }

    info.queue_props.resize(info.queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(info.gpus[0], &info.queue_family_count, info.queue_props.data());
    if (info.queue_family_count < 1)
    {
        return VK_ERROR_FORMAT_NOT_SUPPORTED;
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

VkResult init_global_layer_properties(VulkanData& info)
{
    uint32_t instance_layer_count;
    VkLayerProperties* vk_props = nullptr;
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
    do 
    {
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