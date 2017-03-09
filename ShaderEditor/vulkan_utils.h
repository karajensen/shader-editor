////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkan_utils.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "common.h"
#include "render_data.h"
#include "vulkan.h"
#include "glm/mat4x4.hpp"

struct VulkanData;
struct LayerProperties;

/**
* Callbacks for pre-rendering elements
*/
typedef std::function<void(VkCommandBuffer, const glm::mat4&, int)> PreRenderMesh;

/**
* Utilities for managing Vulkan
*/
class VulkanUtils
{
public:

    /**
    * Number of viewports and number of scissors have to be the same
    * at pipeline creation and in any call to set them dynamically
    * They also have to be the same as each other
    */
    static const int NUM_VIEWPORTS_AND_SCISSORS = 1;

    /**
    * Number of samples needs to be the same at image creation
    * renderpass creation and pipeline creation.      
    */
    static const VkSampleCountFlagBits NUM_SAMPLES = VK_SAMPLE_COUNT_1_BIT;

    /**
    * Amount of time, in nanoseconds, to wait for a command buffer to complete
    */
    static const int FENCE_TIMEOUT = 100000000;

    /**
    * Generic Vulkan initualisation error
    */
    static const VkResult VK_ERROR = VK_ERROR_INITIALIZATION_FAILED;

    /**
    * Sets the debug name for the Vulkan object
    */
    static void SetDebugName(VulkanData& info, 
                             uint64_t obj, 
                             const type_info& objType, 
                             const std::string& name);

    /**
    * @return whether the given result indicates failure
    */
    static bool Failed(VkResult result);

    /**
    * Logs the given result if a failure
    * @return whether the given result indicates failure
    */
    static bool LogFail(VkResult result, const char* file, int line);

    /**
    * @return a text description of the failure or empty if a success
    */
    static const char* GetFailText(VkResult result);

    /**
    * Search memtypes to find first index with those properties
    */
    static VkResult MemoryTypeFromProperties(VulkanData &info,
                                             uint32_t typeBits, 
                                             VkFlags requirementsMask, 
                                             uint32_t* typeIndex);
};

#define CHECK_FAIL(result) VulkanUtils::LogFail(result, __FILE__, __LINE__)