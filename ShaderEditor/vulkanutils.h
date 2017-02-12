////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkanutils.cpp
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "common.h"
#include "renderdata.h"
#include "vulkan.h"

struct VulkanData;
struct LayerProperties;

bool FAIL(VkResult result);
VkResult init_global_extension_properties(LayerProperties& layer_props);
VkResult init_global_layer_properties(VulkanData& info);