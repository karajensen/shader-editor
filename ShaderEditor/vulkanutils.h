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
VkResult init_global_layer_properties(VulkanData& info);
VkResult init_instance(VulkanData &info);

#undef FAILED // Defined for DirectX
#define FAILED(result) log_fail(result, __FILE__, __LINE__)