////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkanutils.cpp
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "common.h"
#include "renderdata.h"
#include "vulkan.h"

struct VulkanData;
struct LayerProperties;

bool log_fail(VkResult result, const char* file, int line);
VkResult init_global_extension_properties(LayerProperties& layer_props);
VkResult init_instance(VulkanData &info);
VkResult init_enumerate_device(VulkanData &info, uint32_t gpu_count = 1);
VkResult init_queue_family_index(VulkanData &info);
VkResult init_device(VulkanData &info);

#undef FAILED
#define FAILED(result) log_fail(result, __FILE__, __LINE__)