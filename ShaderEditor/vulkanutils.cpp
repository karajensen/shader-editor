////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkanutils.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "vulkanutils.h"
#include "vulkandata.h"
#include "logger.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

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

void VulkanUtils::SetDebugName(VulkanData& info, uint64_t obj, const type_info& objType, const std::string& name)
{
    VkDebugReportObjectTypeEXT type;
    if (objType == typeid(VkCommandBuffer))
    {
        type = VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_BUFFER_EXT;
    }
    else if (objType == typeid(VkQueue))
    {
        type = VK_DEBUG_REPORT_OBJECT_TYPE_QUEUE_EXT;
    }
    else if (objType == typeid(VkImage))
    {
        type = VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT;
    }
    else if (objType == typeid(VkSampler))
    {
        type = VK_DEBUG_REPORT_OBJECT_TYPE_SAMPLER_EXT;
    }
    else if (objType == typeid(VkBuffer))
    {
        type = VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT;
    }
    else if (objType == typeid(VkDeviceMemory))
    {
        type = VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT;
    }
    else if (objType == typeid(VkShaderModule))
    {
        type = VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT;
    }
    else if (objType == typeid(VkPipeline))
    {
        type = VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_EXT;
    }
    else if (objType == typeid(VkPipelineLayout))
    {
        type = VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_LAYOUT_EXT;
    }
    else if (objType == typeid(VkRenderPass))
    {
        type = VK_DEBUG_REPORT_OBJECT_TYPE_RENDER_PASS_EXT;
    }
    else if (objType == typeid(VkFramebuffer))
    {
        type = VK_DEBUG_REPORT_OBJECT_TYPE_FRAMEBUFFER_EXT;
    }
    else if (objType == typeid(VkDescriptorSetLayout))
    {
        type = VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT_EXT;
    }
    else if (objType == typeid(VkDescriptorSet))
    {
        type = VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_EXT;
    }
    else if (objType == typeid(VkFence))
    {
        type = VK_DEBUG_REPORT_OBJECT_TYPE_FENCE_EXT;
    }
    else if (objType == typeid(VkSemaphore))
    {
        type = VK_DEBUG_REPORT_OBJECT_TYPE_SEMAPHORE_EXT;
    }
    else if (objType == typeid(VkEvent))
    {
        type = VK_DEBUG_REPORT_OBJECT_TYPE_EVENT_EXT;
    }
    else
    {
        Logger::LogError("Vulkan: Cannot set name %s for unknown type", name);
        return;
    }

    VkDebugMarkerObjectNameInfoEXT nameInfo = {};
    nameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_NAME_INFO_EXT;
    nameInfo.objectType = type;
    nameInfo.object = obj;
    nameInfo.pObjectName = name.c_str();
    info.setDebugNameFn(info.device, &nameInfo);
}

void VulkanUtils::Render(VulkanData& info)
{
    CHECK_FAIL(vkAcquireNextImageKHR(info.device,
                                     info.swapChain, 
                                     UINT64_MAX, 
                                     info.presentCompleteSemaphore, 
                                     VK_NULL_HANDLE,
                                     &info.currentBuffer));

    CHECK_FAIL(vkWaitForFences(info.device, 1, &info.fences[info.currentBuffer], VK_TRUE, UINT64_MAX));
    CHECK_FAIL(vkResetFences(info.device, 1, &info.fences[info.currentBuffer]));

    VkSubmitInfo submitInfo;
    submitInfo.pNext = NULL;
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &info.presentCompleteSemaphore;
    submitInfo.pWaitDstStageMask = &info.pipeStageFlags;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &info.cmd[info.currentBuffer];
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &info.renderCompleteSemaphore;
    CHECK_FAIL(vkQueueSubmit(info.graphicsQueue, 1, &submitInfo, info.fences[info.currentBuffer]));

    VkPresentInfoKHR presentInfo;
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.pNext = NULL;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &info.swapChain;
    presentInfo.pImageIndices = &info.currentBuffer;
    presentInfo.pWaitSemaphores = &info.renderCompleteSemaphore;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pResults = NULL;
    CHECK_FAIL(vkQueuePresentKHR(info.presentQueue, &presentInfo));
}

VkResult VulkanUtils::MemoryTypeFromProperties(VulkanData &info,
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
                return VK_SUCCESS;
            }
        }
        typeBits >>= 1;
    }
    Logger::LogError("Vulkan: No memory types matched");
    return VK_ERROR;
}