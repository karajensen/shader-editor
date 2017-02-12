////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkanutils.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "vulkanutils.h"
#include "vulkandata.h"
#include "logger.h"

bool FAIL(VkResult result)
{
    switch (result)
    {
    case VK_SUCCESS:
        return false;
    case VK_NOT_READY:
        Logger::LogError("Vulkan: VK_NOT_READY");
        return true;
    case VK_TIMEOUT:
        Logger::LogError("Vulkan: VK_TIMEOUT");
        return true;
    case VK_EVENT_SET:
        Logger::LogError("Vulkan: VK_EVENT_SET");
        return true;
    case VK_EVENT_RESET:
        Logger::LogError("Vulkan: VK_EVENT_RESET");
        return true;
    case VK_INCOMPLETE:
        Logger::LogError("Vulkan: VK_INCOMPLETE");
        return true;
    case VK_ERROR_OUT_OF_HOST_MEMORY:
        Logger::LogError("Vulkan: VK_ERROR_OUT_OF_HOST_MEMORY");
        return true;
    case VK_ERROR_OUT_OF_DEVICE_MEMORY:
        Logger::LogError("Vulkan: VK_ERROR_OUT_OF_DEVICE_MEMORY");
        return true;
    case VK_ERROR_INITIALIZATION_FAILED:
        Logger::LogError("Vulkan: VK_ERROR_INITIALIZATION_FAILED");
        return true;
    case VK_ERROR_DEVICE_LOST:
        Logger::LogError("Vulkan: VK_ERROR_DEVICE_LOST");
        return true;
    case VK_ERROR_MEMORY_MAP_FAILED:
        Logger::LogError("Vulkan: VK_ERROR_MEMORY_MAP_FAILED");
        return true;
    case VK_ERROR_LAYER_NOT_PRESENT:
        Logger::LogError("Vulkan: VK_ERROR_LAYER_NOT_PRESENT");
        return true;
    case VK_ERROR_EXTENSION_NOT_PRESENT:
        Logger::LogError("Vulkan: VK_ERROR_EXTENSION_NOT_PRESENT");
        return true;
    case VK_ERROR_FEATURE_NOT_PRESENT:
        Logger::LogError("Vulkan: VK_ERROR_FEATURE_NOT_PRESENT");
        return true;
    case VK_ERROR_INCOMPATIBLE_DRIVER:
        Logger::LogError("Vulkan: VK_ERROR_INCOMPATIBLE_DRIVER");
        return true;
    case VK_ERROR_TOO_MANY_OBJECTS:
        Logger::LogError("Vulkan: VK_ERROR_TOO_MANY_OBJECTS");
        return true;
    case VK_ERROR_FORMAT_NOT_SUPPORTED:
        Logger::LogError("Vulkan: VK_ERROR_FORMAT_NOT_SUPPORTED");
        return true;
    case VK_ERROR_SURFACE_LOST_KHR:
        Logger::LogError("Vulkan: VK_ERROR_SURFACE_LOST_KHR");
        return true;
    case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
        Logger::LogError("Vulkan: VK_ERROR_NATIVE_WINDOW_IN_USE_KHR");
        return true;
    case VK_SUBOPTIMAL_KHR:
        Logger::LogError("Vulkan: VK_SUBOPTIMAL_KHR");
        return true;
    case VK_ERROR_OUT_OF_DATE_KHR:
        Logger::LogError("Vulkan: VK_ERROR_OUT_OF_DATE_KHR");
        return true;
    case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
        Logger::LogError("Vulkan: VK_ERROR_INCOMPATIBLE_DISPLAY_KHR");
        return true;
    case VK_ERROR_VALIDATION_FAILED_EXT:
        Logger::LogError("Vulkan: VK_ERROR_VALIDATION_FAILED_EXT");
        return true;
    case VK_ERROR_INVALID_SHADER_NV:
        Logger::LogError("Vulkan: VK_ERROR_INVALID_SHADER_NV");
        return true;
    default:
        Logger::LogError("Vulkan: UNKNOWN");
        return true;
    }
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