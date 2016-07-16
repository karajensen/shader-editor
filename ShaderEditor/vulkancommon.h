////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkancommon.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "common.h"
#include "renderdata.h"
#include "mesh.h"
#include "water.h"
#include "shader.h"
#include "texture.h"
#include "textureProcedural.h"
#include "postprocessing.h"
#include "emitter.h"
#include "terrain.h"
#include "light.h"

#include "vulkan.h"

/**
* Vulkan call checking
* @return whether the last call to OpenGL has failed
* @note requires an OpenGL context to be created
*/
inline bool FAIL(VkResult result)
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