////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkanutils.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "vulkanutils.h"
#include "vulkandata.h"
#include "logger.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace 
{
    const VkResult VK_ERROR = VK_ERROR_INITIALIZATION_FAILED;

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

    EShLanguage find_language(const VkShaderStageFlagBits shader_type)
    {
        switch (shader_type)
        {
        case VK_SHADER_STAGE_VERTEX_BIT:
            return EShLangVertex;
        case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:
            return EShLangTessControl;
        case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:
            return EShLangTessEvaluation;
        case VK_SHADER_STAGE_GEOMETRY_BIT:
            return EShLangGeometry;
        case VK_SHADER_STAGE_FRAGMENT_BIT:
            return EShLangFragment;
        case VK_SHADER_STAGE_COMPUTE_BIT:
            return EShLangCompute;
        default:
            return EShLangVertex;
        }
    }

    void init_shader_resources(TBuiltInResource& resources)
    {
        resources.maxLights = 32;
        resources.maxClipPlanes = 6;
        resources.maxTextureUnits = 32;
        resources.maxTextureCoords = 32;
        resources.maxVertexAttribs = 64;
        resources.maxVertexUniformComponents = 4096;
        resources.maxVaryingFloats = 64;
        resources.maxVertexTextureImageUnits = 32;
        resources.maxCombinedTextureImageUnits = 80;
        resources.maxTextureImageUnits = 32;
        resources.maxFragmentUniformComponents = 4096;
        resources.maxDrawBuffers = 32;
        resources.maxVertexUniformVectors = 128;
        resources.maxVaryingVectors = 8;
        resources.maxFragmentUniformVectors = 16;
        resources.maxVertexOutputVectors = 16;
        resources.maxFragmentInputVectors = 15;
        resources.minProgramTexelOffset = -8;
        resources.maxProgramTexelOffset = 7;
        resources.maxClipDistances = 8;
        resources.maxComputeWorkGroupCountX = 65535;
        resources.maxComputeWorkGroupCountY = 65535;
        resources.maxComputeWorkGroupCountZ = 65535;
        resources.maxComputeWorkGroupSizeX = 1024;
        resources.maxComputeWorkGroupSizeY = 1024;
        resources.maxComputeWorkGroupSizeZ = 64;
        resources.maxComputeUniformComponents = 1024;
        resources.maxComputeTextureImageUnits = 16;
        resources.maxComputeImageUniforms = 8;
        resources.maxComputeAtomicCounters = 8;
        resources.maxComputeAtomicCounterBuffers = 1;
        resources.maxVaryingComponents = 60;
        resources.maxVertexOutputComponents = 64;
        resources.maxGeometryInputComponents = 64;
        resources.maxGeometryOutputComponents = 128;
        resources.maxFragmentInputComponents = 128;
        resources.maxImageUnits = 8;
        resources.maxCombinedImageUnitsAndFragmentOutputs = 8;
        resources.maxCombinedShaderOutputResources = 8;
        resources.maxImageSamples = 0;
        resources.maxVertexImageUniforms = 0;
        resources.maxTessControlImageUniforms = 0;
        resources.maxTessEvaluationImageUniforms = 0;
        resources.maxGeometryImageUniforms = 0;
        resources.maxFragmentImageUniforms = 8;
        resources.maxCombinedImageUniforms = 8;
        resources.maxGeometryTextureImageUnits = 16;
        resources.maxGeometryOutputVertices = 256;
        resources.maxGeometryTotalOutputComponents = 1024;
        resources.maxGeometryUniformComponents = 1024;
        resources.maxGeometryVaryingComponents = 64;
        resources.maxTessControlInputComponents = 128;
        resources.maxTessControlOutputComponents = 128;
        resources.maxTessControlTextureImageUnits = 16;
        resources.maxTessControlUniformComponents = 1024;
        resources.maxTessControlTotalOutputComponents = 4096;
        resources.maxTessEvaluationInputComponents = 128;
        resources.maxTessEvaluationOutputComponents = 128;
        resources.maxTessEvaluationTextureImageUnits = 16;
        resources.maxTessEvaluationUniformComponents = 1024;
        resources.maxTessPatchComponents = 120;
        resources.maxPatchVertices = 32;
        resources.maxTessGenLevel = 64;
        resources.maxViewports = 16;
        resources.maxVertexAtomicCounters = 0;
        resources.maxTessControlAtomicCounters = 0;
        resources.maxTessEvaluationAtomicCounters = 0;
        resources.maxGeometryAtomicCounters = 0;
        resources.maxFragmentAtomicCounters = 8;
        resources.maxCombinedAtomicCounters = 8;
        resources.maxAtomicCounterBindings = 1;
        resources.maxVertexAtomicCounterBuffers = 0;
        resources.maxTessControlAtomicCounterBuffers = 0;
        resources.maxTessEvaluationAtomicCounterBuffers = 0;
        resources.maxGeometryAtomicCounterBuffers = 0;
        resources.maxFragmentAtomicCounterBuffers = 1;
        resources.maxCombinedAtomicCounterBuffers = 1;
        resources.maxAtomicCounterBufferSize = 16384;
        resources.maxTransformFeedbackBuffers = 4;
        resources.maxTransformFeedbackInterleavedComponents = 64;
        resources.maxCullDistances = 8;
        resources.maxCombinedClipAndCullDistances = 8;
        resources.maxSamples = 4;
        resources.limits.nonInductiveForLoops = 1;
        resources.limits.whileLoops = 1;
        resources.limits.doWhileLoops = 1;
        resources.limits.generalUniformIndexing = 1;
        resources.limits.generalAttributeMatrixVectorIndexing = 1;
        resources.limits.generalVaryingIndexing = 1;
        resources.limits.generalSamplerIndexing = 1;
        resources.limits.generalVariableIndexing = 1;
        resources.limits.generalConstantMatrixVectorIndexing = 1;
    }

    bool glsl_to_spb(const VkShaderStageFlagBits shader_type, const char* pshader, std::vector<unsigned int>& spirv)
    {
        EShLanguage stage = find_language(shader_type);
        glslang::TShader shader(stage);
        glslang::TProgram program;
        const char *shaderStrings[1];

        TBuiltInResource Resources;
        init_shader_resources(Resources);

        // Enable SPIR-V and Vulkan rules when parsing GLSL
        EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules);

        shaderStrings[0] = pshader;
        shader.setStrings(shaderStrings, 1);

        if (!shader.parse(&Resources, 100, false, messages))
        {
            puts(shader.getInfoLog());
            puts(shader.getInfoDebugLog());
            return false;  // something didn't work
        }

        program.addShader(&shader);

        // Program-level processing...
        if (!program.link(messages))
        {
            puts(shader.getInfoLog());
            puts(shader.getInfoDebugLog());
            fflush(stdout);
            return false;
        }

        glslang::GlslangToSpv(*program.getIntermediate(stage), spirv);
        return true;
    }
}

bool VulkanUtils::log_fail(VkResult result, const char* file, int line)
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

VkResult VulkanUtils::init_enumerate_device(VulkanData &info, uint32_t gpu_count)
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

VkResult VulkanUtils::init_global_extension_properties(LayerProperties& layer_props)
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

VkResult VulkanUtils::init_device_extension_names(VulkanData &info)
{
    info.device_extension_names =
    {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
    return VK_SUCCESS;
}

VkResult VulkanUtils::init_instance_extension_names(VulkanData &info)
{
    info.instance_extension_names =
    {
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
        VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
    };
    return VK_SUCCESS;
}

VkResult VulkanUtils::init_instance(VulkanData &info)
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

VkResult VulkanUtils::init_queue_family_index(VulkanData &info)
{
    // This routine simply finds a graphics queue for a later vkCreateDevice,
    // without consideration for which queue family can present an image.
    // Do not use this if your intent is to present later in your sample,
    // instead use the init_connection, init_window, init_swapchain_extension,
    // init_device call sequence to get a graphics and present compatible queue family

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

    bool found = false;
    for (unsigned int i = 0; i < info.queue_family_count; i++) 
    {
        if (info.queue_props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            info.graphics_queue_family_index = i;
            found = true;
            break;
        }
    }
    
    return found ? VK_SUCCESS : VK_ERROR_VALIDATION_FAILED_EXT;
}

VkResult VulkanUtils::init_device(VulkanData &info)
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

VkResult VulkanUtils::init_debugging(VulkanData &info)
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

VkResult VulkanUtils::init_global_layer_properties(VulkanData& info)
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

VkResult VulkanUtils::init_swapchain_extension(VulkanData &info)
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
        res = vkGetPhysicalDeviceSurfaceSupportKHR(info.gpus[0], i, info.surface, &pSupportsPresent[i]);
        if (failed(res))
        {
            return res;
        }
    }

    // Search for a graphics and a present queue in the array of queue
    // families, try to find one that supports both
    info.graphics_queue_family_index = UINT32_MAX;
    info.present_queue_family_index = UINT32_MAX;
    for (uint32_t i = 0; i < info.queue_family_count; ++i) 
    {
        if ((info.queue_props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0)
        {
            if (info.graphics_queue_family_index == UINT32_MAX)
            {
                info.graphics_queue_family_index = i;
            }

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

VkResult VulkanUtils::init_command_pool(VulkanData &info)
{
    VkCommandPoolCreateInfo cmd_pool_info = {};
    cmd_pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cmd_pool_info.pNext = NULL;
    cmd_pool_info.queueFamilyIndex = info.graphics_queue_family_index;
    cmd_pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    return vkCreateCommandPool(info.device, &cmd_pool_info, NULL, &info.cmd_pool);
}

VkResult VulkanUtils::init_command_buffer(VulkanData &info)
{
    VkCommandBufferAllocateInfo cmd = {};
    cmd.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmd.pNext = NULL;
    cmd.commandPool = info.cmd_pool;
    cmd.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    cmd.commandBufferCount = 1;

    return vkAllocateCommandBuffers(info.device, &cmd, &info.cmd);
}

VkResult VulkanUtils::init_device_queue(VulkanData &info)
{
    vkGetDeviceQueue(info.device, info.graphics_queue_family_index, 0, &info.graphics_queue);
    if (info.graphics_queue_family_index == info.present_queue_family_index) 
    {
        info.present_queue = info.graphics_queue;
    }
    else 
    {
        vkGetDeviceQueue(info.device, info.present_queue_family_index, 0, &info.present_queue);
    }
    return VK_SUCCESS;
}

VkResult VulkanUtils::init_swap_chain(VulkanData &info)
{
    VkImageUsageFlags usageFlags =
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
        VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

    VkSurfaceCapabilitiesKHR surfCapabilities;

    VkResult res = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(info.gpus[0], info.surface, &surfCapabilities);
    if (failed(res))
    {
        return res;
    }

    uint32_t presentModeCount;
    res = vkGetPhysicalDeviceSurfacePresentModesKHR(info.gpus[0], info.surface, &presentModeCount, NULL);
    if (failed(res))
    {
        return res;
    }

    VkPresentModeKHR *presentModes = (VkPresentModeKHR *)malloc(presentModeCount * sizeof(VkPresentModeKHR));
    if (!presentModes)
    {
        return VK_ERROR;
    }

    res = vkGetPhysicalDeviceSurfacePresentModesKHR(info.gpus[0], info.surface, &presentModeCount, presentModes);
    if (failed(res))
    {
        return res;
    }

    VkExtent2D swapchainExtent;

    // width and height are either both 0xFFFFFFFF, or both not 0xFFFFFFFF.
    if (surfCapabilities.currentExtent.width == 0xFFFFFFFF) 
    {
        // If the surface size is undefined, the size is set to
        // the size of the images requested.
        swapchainExtent.width = WINDOW_WIDTH;
        swapchainExtent.height = WINDOW_HEIGHT;
        if (swapchainExtent.width < surfCapabilities.minImageExtent.width) 
        {
            swapchainExtent.width = surfCapabilities.minImageExtent.width;
        }
        else if (swapchainExtent.width > surfCapabilities.maxImageExtent.width) 
        {
            swapchainExtent.width = surfCapabilities.maxImageExtent.width;
        }

        if (swapchainExtent.height < surfCapabilities.minImageExtent.height) 
        {
            swapchainExtent.height = surfCapabilities.minImageExtent.height;
        }
        else if (swapchainExtent.height > surfCapabilities.maxImageExtent.height) 
        {
            swapchainExtent.height = surfCapabilities.maxImageExtent.height;
        }
    }
    else 
    {
        // If the surface size is defined, the swap chain size must match
        swapchainExtent = surfCapabilities.currentExtent;
    }

    // The FIFO present mode is guaranteed by the spec to be supported
    // Also note that current Android driver only supports FIFO
    VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;

    // Determine the number of VkImage's to use in the swap chain.
    // We need to acquire only 1 presentable image at at time.
    // Asking for minImageCount images ensures that we can acquire
    // 1 presentable image as long as we present it before attempting
    // to acquire another.
    uint32_t desiredNumberOfSwapChainImages = surfCapabilities.minImageCount;

    VkSurfaceTransformFlagBitsKHR preTransform;
    if (surfCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) 
    {
        preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    }
    else 
    {
        preTransform = surfCapabilities.currentTransform;
    }

    VkSwapchainCreateInfoKHR swapchain_ci = {};
    swapchain_ci.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchain_ci.pNext = NULL;
    swapchain_ci.surface = info.surface;
    swapchain_ci.minImageCount = desiredNumberOfSwapChainImages;
    swapchain_ci.imageFormat = info.format;
    swapchain_ci.imageExtent.width = swapchainExtent.width;
    swapchain_ci.imageExtent.height = swapchainExtent.height;
    swapchain_ci.preTransform = preTransform;
    swapchain_ci.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchain_ci.imageArrayLayers = 1;
    swapchain_ci.presentMode = swapchainPresentMode;
    swapchain_ci.oldSwapchain = VK_NULL_HANDLE;
    swapchain_ci.clipped = true;
    swapchain_ci.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
    swapchain_ci.imageUsage = usageFlags;
    swapchain_ci.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapchain_ci.queueFamilyIndexCount = 0;
    swapchain_ci.pQueueFamilyIndices = NULL;
    uint32_t queueFamilyIndices[2] = { (uint32_t)info.graphics_queue_family_index, (uint32_t)info.present_queue_family_index };
    if (info.graphics_queue_family_index != info.present_queue_family_index) 
    {
        // If the graphics and present queues are from different queue families,
        // we either have to explicitly transfer ownership of images between the
        // queues, or we have to create the swapchain with imageSharingMode
        // as VK_SHARING_MODE_CONCURRENT
        swapchain_ci.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapchain_ci.queueFamilyIndexCount = 2;
        swapchain_ci.pQueueFamilyIndices = queueFamilyIndices;
    }

    res = vkCreateSwapchainKHR(info.device, &swapchain_ci, NULL, &info.swap_chain);
    if (failed(res))
    {
        return res;
    }

    res = vkGetSwapchainImagesKHR(info.device, info.swap_chain, &info.swapchainImageCount, NULL);
    if (failed(res))
    {
        return res;
    }

    VkImage *swapchainImages = (VkImage *)malloc(info.swapchainImageCount * sizeof(VkImage));
    if (!swapchainImages)
    {
        return VK_ERROR;
    }

    res = vkGetSwapchainImagesKHR(info.device, info.swap_chain, &info.swapchainImageCount, swapchainImages);
    if (failed(res))
    {
        return res;
    }

    for (uint32_t i = 0; i < info.swapchainImageCount; i++) 
    {
        SwapChainBuffer sc_buffer;

        VkImageViewCreateInfo color_image_view = {};
        color_image_view.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        color_image_view.pNext = NULL;
        color_image_view.format = info.format;
        color_image_view.components.r = VK_COMPONENT_SWIZZLE_R;
        color_image_view.components.g = VK_COMPONENT_SWIZZLE_G;
        color_image_view.components.b = VK_COMPONENT_SWIZZLE_B;
        color_image_view.components.a = VK_COMPONENT_SWIZZLE_A;
        color_image_view.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        color_image_view.subresourceRange.baseMipLevel = 0;
        color_image_view.subresourceRange.levelCount = 1;
        color_image_view.subresourceRange.baseArrayLayer = 0;
        color_image_view.subresourceRange.layerCount = 1;
        color_image_view.viewType = VK_IMAGE_VIEW_TYPE_2D;
        color_image_view.flags = 0;

        sc_buffer.image = swapchainImages[i];

        color_image_view.image = sc_buffer.image;

        res = vkCreateImageView(info.device, &color_image_view, NULL, &sc_buffer.view);
        info.buffers.push_back(sc_buffer);
        if (failed(res))
        {
            return res;
        }
    }
    free(swapchainImages);
    info.current_buffer = 0;

    if (NULL != presentModes) 
    {
        free(presentModes);
    }
    return VK_SUCCESS;
}

VkResult VulkanUtils::init_depth_buffer(VulkanData &info)
{
    VkImageCreateInfo image_info = {};

    /* allow custom depth formats */
    if (info.depth.format == VK_FORMAT_UNDEFINED)
    {
        info.depth.format = VK_FORMAT_D16_UNORM;
    }

    const VkFormat depth_format = info.depth.format;
    VkFormatProperties props;
    vkGetPhysicalDeviceFormatProperties(info.gpus[0], depth_format, &props);
    if (props.linearTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) 
    {
        image_info.tiling = VK_IMAGE_TILING_LINEAR;
    }
    else if (props.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
    {
        image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
    }
    else 
    {
        Logger::LogError("Vulkan: Depth Format Unsupported");
        return VK_ERROR;
    }

    image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_info.pNext = NULL;
    image_info.imageType = VK_IMAGE_TYPE_2D;
    image_info.format = depth_format;
    image_info.extent.width = WINDOW_WIDTH;
    image_info.extent.height = WINDOW_HEIGHT;
    image_info.extent.depth = 1;
    image_info.mipLevels = 1;
    image_info.arrayLayers = 1;
    image_info.samples = NUM_SAMPLES;
    image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    image_info.queueFamilyIndexCount = 0;
    image_info.pQueueFamilyIndices = NULL;
    image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    image_info.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    image_info.flags = 0;

    VkMemoryAllocateInfo mem_alloc = {};
    mem_alloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    mem_alloc.pNext = NULL;
    mem_alloc.allocationSize = 0;
    mem_alloc.memoryTypeIndex = 0;

    VkImageViewCreateInfo view_info = {};
    view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    view_info.pNext = NULL;
    view_info.image = VK_NULL_HANDLE;
    view_info.format = depth_format;
    view_info.components.r = VK_COMPONENT_SWIZZLE_R;
    view_info.components.g = VK_COMPONENT_SWIZZLE_G;
    view_info.components.b = VK_COMPONENT_SWIZZLE_B;
    view_info.components.a = VK_COMPONENT_SWIZZLE_A;
    view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    view_info.subresourceRange.baseMipLevel = 0;
    view_info.subresourceRange.levelCount = 1;
    view_info.subresourceRange.baseArrayLayer = 0;
    view_info.subresourceRange.layerCount = 1;
    view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    view_info.flags = 0;

    if (depth_format == VK_FORMAT_D16_UNORM_S8_UINT || 
        depth_format == VK_FORMAT_D24_UNORM_S8_UINT ||
        depth_format == VK_FORMAT_D32_SFLOAT_S8_UINT)
    {
        view_info.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
    }

    VkMemoryRequirements mem_reqs;

    /* Create image */
    VkResult res = vkCreateImage(info.device, &image_info, NULL, &info.depth.image);
    if (failed(res))
    {
        return res;
    }

    vkGetImageMemoryRequirements(info.device, info.depth.image, &mem_reqs);

    mem_alloc.allocationSize = mem_reqs.size;

    /* Use the memory properties to determine the type of memory required */
    if (!memory_type_from_properties(info, mem_reqs.memoryTypeBits, 0, &mem_alloc.memoryTypeIndex))
    {
        return VK_ERROR;
    }

    /* Allocate memory */
    res = vkAllocateMemory(info.device, &mem_alloc, NULL, &info.depth.memory);
    if (failed(res))
    {
        return res;
    }

    /* Bind memory */
    res = vkBindImageMemory(info.device, info.depth.image, info.depth.memory, 0);
    if (failed(res))
    {
        return res;
    }

    /* Create image view */
    view_info.image = info.depth.image;
    return vkCreateImageView(info.device, &view_info, NULL, &info.depth.view);
}

VkResult VulkanUtils::init_uniform_buffer(VulkanData& info)
{
    float fov = glm::radians(45.0f);
    if (WINDOW_WIDTH > WINDOW_HEIGHT) 
    {
        fov *= static_cast<float>(WINDOW_HEIGHT) / static_cast<float>(WINDOW_WIDTH);
    }

    info.projection = glm::perspective(fov, static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT), 0.1f, 100.0f);
    
    info.view = 
        glm::lookAt(glm::vec3(-5, 3, -10),  // Camera is at (-5,3,-10), in World Space
        glm::vec3(0, 0, 0),     // and looks at the origin
        glm::vec3(0, -1, 0)     // Head is up (set to 0,-1,0 to look upside-down)
    );
    info.Model = glm::mat4(1.0f);

    // Vulkan clip space has inverted Y and half Z.
    info.Clip = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.5f, 1.0f);

    info.MVP = info.Clip * info.projection * info.view * info.Model;

    /* VULKAN_KEY_START */
    VkBufferCreateInfo buf_info = {};
    buf_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buf_info.pNext = NULL;
    buf_info.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    buf_info.size = sizeof(info.MVP);
    buf_info.queueFamilyIndexCount = 0;
    buf_info.pQueueFamilyIndices = NULL;
    buf_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    buf_info.flags = 0;
    VkResult res = vkCreateBuffer(info.device, &buf_info, NULL, &info.uniform_data.buffer);
    if (failed(res))
    {
        return res;
    }

    VkMemoryRequirements mem_reqs;
    vkGetBufferMemoryRequirements(info.device, info.uniform_data.buffer, &mem_reqs);

    VkMemoryAllocateInfo alloc_info = {};
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.pNext = NULL;
    alloc_info.memoryTypeIndex = 0;

    alloc_info.allocationSize = mem_reqs.size;
    bool pass = memory_type_from_properties(info, mem_reqs.memoryTypeBits,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        &alloc_info.memoryTypeIndex);
    if (!pass)
    {
        Logger::LogError("Vulkan: No mappable, coherent memory");
        return VK_ERROR;
    }

    res = vkAllocateMemory(info.device, &alloc_info, NULL, &(info.uniform_data.memory));
    if (failed(res))
    {
        return res;
    }

    uint8_t *pData;
    res = vkMapMemory(info.device, info.uniform_data.memory, 0, mem_reqs.size, 0, (void **)&pData);
    if (failed(res))
    {
        return res;
    }

    memcpy(pData, &info.MVP, sizeof(info.MVP));

    vkUnmapMemory(info.device, info.uniform_data.memory);

    res = vkBindBufferMemory(info.device, info.uniform_data.buffer, info.uniform_data.memory, 0);
    if (failed(res))
    {
        return res;
    }

    info.uniform_data.buffer_info.buffer = info.uniform_data.buffer;
    info.uniform_data.buffer_info.offset = 0;
    info.uniform_data.buffer_info.range = sizeof(info.MVP);
    return VK_SUCCESS;
}

VkResult VulkanUtils::init_descriptor_and_pipeline_layouts(VulkanData &info)
{
    bool use_texture = false;

    VkDescriptorSetLayoutBinding layout_bindings[2];
    layout_bindings[0].binding = 0;
    layout_bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    layout_bindings[0].descriptorCount = 1;
    layout_bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    layout_bindings[0].pImmutableSamplers = NULL;

    if (use_texture) 
    {
        layout_bindings[1].binding = 1;
        layout_bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        layout_bindings[1].descriptorCount = 1;
        layout_bindings[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        layout_bindings[1].pImmutableSamplers = NULL;
    }

    /* Next take layout bindings and use them to create a descriptor set layout
    */
    VkDescriptorSetLayoutCreateInfo descriptor_layout = {};
    descriptor_layout.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptor_layout.pNext = NULL;
    descriptor_layout.bindingCount = use_texture ? 2 : 1;
    descriptor_layout.pBindings = layout_bindings;

    info.desc_layout.resize(NUM_DESCRIPTOR_SETS);
    VkResult res = vkCreateDescriptorSetLayout(info.device, &descriptor_layout, NULL, info.desc_layout.data());
    if (failed(res))
    {
        return res;
    }

    /* Now use the descriptor layout to create a pipeline layout */
    VkPipelineLayoutCreateInfo pPipelineLayoutCreateInfo = {};
    pPipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pPipelineLayoutCreateInfo.pNext = NULL;
    pPipelineLayoutCreateInfo.pushConstantRangeCount = 0;
    pPipelineLayoutCreateInfo.pPushConstantRanges = NULL;
    pPipelineLayoutCreateInfo.setLayoutCount = NUM_DESCRIPTOR_SETS;
    pPipelineLayoutCreateInfo.pSetLayouts = info.desc_layout.data();

    return vkCreatePipelineLayout(info.device, &pPipelineLayoutCreateInfo, NULL, &info.pipeline_layout);
}

VkResult VulkanUtils::init_pipeline(VulkanData& info)
{
    VkBool32 include_depth = true;
    VkBool32 include_vi = true;

    VkDynamicState dynamicStateEnables[VK_DYNAMIC_STATE_RANGE_SIZE];
    VkPipelineDynamicStateCreateInfo dynamicState = {};
    memset(dynamicStateEnables, 0, sizeof dynamicStateEnables);
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.pNext = NULL;
    dynamicState.pDynamicStates = dynamicStateEnables;
    dynamicState.dynamicStateCount = 0;

    VkPipelineVertexInputStateCreateInfo vi;
    memset(&vi, 0, sizeof(vi));
    vi.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    if (include_vi)
    {
        vi.pNext = NULL;
        vi.flags = 0;
        vi.vertexBindingDescriptionCount = 1;
        vi.pVertexBindingDescriptions = &info.vi_binding;
        vi.vertexAttributeDescriptionCount = info.vi_attribs.size();
        vi.pVertexAttributeDescriptions = &info.vi_attribs[0];
    }
    VkPipelineInputAssemblyStateCreateInfo ia;
    ia.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    ia.pNext = NULL;
    ia.flags = 0;
    ia.primitiveRestartEnable = VK_FALSE;
    ia.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    VkPipelineRasterizationStateCreateInfo rs;
    rs.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rs.pNext = NULL;
    rs.flags = 0;
    rs.polygonMode = VK_POLYGON_MODE_FILL;
    rs.cullMode = VK_CULL_MODE_BACK_BIT;
    rs.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rs.depthClampEnable = include_depth;
    rs.rasterizerDiscardEnable = VK_FALSE;
    rs.depthBiasEnable = VK_FALSE;
    rs.depthBiasConstantFactor = 0;
    rs.depthBiasClamp = 0;
    rs.depthBiasSlopeFactor = 0;
    rs.lineWidth = 1.0f;

    VkPipelineColorBlendStateCreateInfo cb;
    cb.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    cb.flags = 0;
    cb.pNext = NULL;
    VkPipelineColorBlendAttachmentState att_state[1];
    att_state[0].colorWriteMask = 0xf;
    att_state[0].blendEnable = VK_FALSE;
    att_state[0].alphaBlendOp = VK_BLEND_OP_ADD;
    att_state[0].colorBlendOp = VK_BLEND_OP_ADD;
    att_state[0].srcColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    att_state[0].dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    att_state[0].srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    att_state[0].dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    cb.attachmentCount = 1;
    cb.pAttachments = att_state;
    cb.logicOpEnable = VK_FALSE;
    cb.logicOp = VK_LOGIC_OP_NO_OP;
    cb.blendConstants[0] = 1.0f;
    cb.blendConstants[1] = 1.0f;
    cb.blendConstants[2] = 1.0f;
    cb.blendConstants[3] = 1.0f;

    VkPipelineViewportStateCreateInfo vp = {};
    vp.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    vp.pNext = NULL;
    vp.flags = 0;
    vp.viewportCount = NUM_VIEWPORTS_AND_SCISSORS;
    dynamicStateEnables[dynamicState.dynamicStateCount++] = VK_DYNAMIC_STATE_VIEWPORT;
    vp.scissorCount = NUM_VIEWPORTS_AND_SCISSORS;
    dynamicStateEnables[dynamicState.dynamicStateCount++] = VK_DYNAMIC_STATE_SCISSOR;
    vp.pScissors = NULL;
    vp.pViewports = NULL;

    VkPipelineDepthStencilStateCreateInfo ds;
    ds.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    ds.pNext = NULL;
    ds.flags = 0;
    ds.depthTestEnable = include_depth;
    ds.depthWriteEnable = include_depth;
    ds.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
    ds.depthBoundsTestEnable = VK_FALSE;
    ds.stencilTestEnable = VK_FALSE;
    ds.back.failOp = VK_STENCIL_OP_KEEP;
    ds.back.passOp = VK_STENCIL_OP_KEEP;
    ds.back.compareOp = VK_COMPARE_OP_ALWAYS;
    ds.back.compareMask = 0;
    ds.back.reference = 0;
    ds.back.depthFailOp = VK_STENCIL_OP_KEEP;
    ds.back.writeMask = 0;
    ds.minDepthBounds = 0;
    ds.maxDepthBounds = 0;
    ds.stencilTestEnable = VK_FALSE;
    ds.front = ds.back;

    VkPipelineMultisampleStateCreateInfo ms;
    ms.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    ms.pNext = NULL;
    ms.flags = 0;
    ms.pSampleMask = NULL;
    ms.rasterizationSamples = NUM_SAMPLES;
    ms.sampleShadingEnable = VK_FALSE;
    ms.alphaToCoverageEnable = VK_FALSE;
    ms.alphaToOneEnable = VK_FALSE;
    ms.minSampleShading = 0.0;

    VkGraphicsPipelineCreateInfo pipeline;
    pipeline.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipeline.pNext = NULL;
    pipeline.layout = info.pipeline_layout;
    pipeline.basePipelineHandle = VK_NULL_HANDLE;
    pipeline.basePipelineIndex = 0;
    pipeline.flags = 0;
    pipeline.pVertexInputState = &vi;
    pipeline.pInputAssemblyState = &ia;
    pipeline.pRasterizationState = &rs;
    pipeline.pColorBlendState = &cb;
    pipeline.pTessellationState = NULL;
    pipeline.pMultisampleState = &ms;
    pipeline.pDynamicState = &dynamicState;
    pipeline.pViewportState = &vp;
    pipeline.pDepthStencilState = &ds;
    pipeline.pStages = &info.shaderStages[0];
    pipeline.stageCount = info.shaderStages.size();
    pipeline.renderPass = info.render_pass;
    pipeline.subpass = 0;

    return vkCreateGraphicsPipelines(info.device, info.pipelineCache, 1, &pipeline, NULL, &info.pipeline);
}

VkResult VulkanUtils::init_pipeline_cache(VulkanData& info) 
{
    VkPipelineCacheCreateInfo pipelineCache;
    pipelineCache.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    pipelineCache.pNext = NULL;
    pipelineCache.initialDataSize = 0;
    pipelineCache.pInitialData = NULL;
    pipelineCache.flags = 0;
    return VK_SUCCESS;
}

VkResult VulkanUtils::init_descriptor_pool(VulkanData& info)
{
    bool use_texture = false;

    VkDescriptorPoolSize type_count[2];
    type_count[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    type_count[0].descriptorCount = 1;
    if (use_texture) 
    {
        type_count[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        type_count[1].descriptorCount = 1;
    }

    VkDescriptorPoolCreateInfo descriptor_pool = {};
    descriptor_pool.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptor_pool.pNext = NULL;
    descriptor_pool.maxSets = 1;
    descriptor_pool.poolSizeCount = use_texture ? 2 : 1;
    descriptor_pool.pPoolSizes = type_count;

    return vkCreateDescriptorPool(info.device, &descriptor_pool, NULL, &info.desc_pool);
}

VkResult VulkanUtils::init_descriptor_set(VulkanData& info)
{
    bool use_texture = false;

    VkDescriptorSetAllocateInfo alloc_info[1];
    alloc_info[0].sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    alloc_info[0].pNext = NULL;
    alloc_info[0].descriptorPool = info.desc_pool;
    alloc_info[0].descriptorSetCount = NUM_DESCRIPTOR_SETS;
    alloc_info[0].pSetLayouts = info.desc_layout.data();

    info.desc_set.resize(NUM_DESCRIPTOR_SETS);
    VkResult res = vkAllocateDescriptorSets(info.device, alloc_info, info.desc_set.data());
    if (failed(res))
    {
        return res;
    }

    VkWriteDescriptorSet writes[2];

    writes[0] = {};
    writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writes[0].pNext = NULL;
    writes[0].dstSet = info.desc_set[0];
    writes[0].descriptorCount = 1;
    writes[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    writes[0].pBufferInfo = &info.uniform_data.buffer_info;
    writes[0].dstArrayElement = 0;
    writes[0].dstBinding = 0;

    if (use_texture) {
        writes[1] = {};
        writes[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writes[1].dstSet = info.desc_set[0];
        writes[1].dstBinding = 1;
        writes[1].descriptorCount = 1;
        writes[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        writes[1].pImageInfo = &info.image_info;
        writes[1].dstArrayElement = 0;
    }

    vkUpdateDescriptorSets(info.device, use_texture ? 2 : 1, writes, 0, NULL);
    return VK_SUCCESS;
}

VkResult VulkanUtils::init_vertex_buffer(VulkanData& info)
{
    struct Vertex {
        float posX, posY, posZ, posW;  // Position data
        float r, g, b, a;              // Color
    };

    #define XYZ1(_x_, _y_, _z_) (_x_), (_y_), (_z_), 1.f

    static const Vertex g_vb_solid_face_colors_Data[] = {
        // red face
        { XYZ1(-1, -1, 1), XYZ1(1.f, 0.f, 0.f) },
        { XYZ1(-1, 1, 1), XYZ1(1.f, 0.f, 0.f) },
        { XYZ1(1, -1, 1), XYZ1(1.f, 0.f, 0.f) },
        { XYZ1(1, -1, 1), XYZ1(1.f, 0.f, 0.f) },
        { XYZ1(-1, 1, 1), XYZ1(1.f, 0.f, 0.f) },
        { XYZ1(1, 1, 1), XYZ1(1.f, 0.f, 0.f) },
        // green face
        { XYZ1(-1, -1, -1), XYZ1(0.f, 1.f, 0.f) },
        { XYZ1(1, -1, -1), XYZ1(0.f, 1.f, 0.f) },
        { XYZ1(-1, 1, -1), XYZ1(0.f, 1.f, 0.f) },
        { XYZ1(-1, 1, -1), XYZ1(0.f, 1.f, 0.f) },
        { XYZ1(1, -1, -1), XYZ1(0.f, 1.f, 0.f) },
        { XYZ1(1, 1, -1), XYZ1(0.f, 1.f, 0.f) },
        // blue face
        { XYZ1(-1, 1, 1), XYZ1(0.f, 0.f, 1.f) },
        { XYZ1(-1, -1, 1), XYZ1(0.f, 0.f, 1.f) },
        { XYZ1(-1, 1, -1), XYZ1(0.f, 0.f, 1.f) },
        { XYZ1(-1, 1, -1), XYZ1(0.f, 0.f, 1.f) },
        { XYZ1(-1, -1, 1), XYZ1(0.f, 0.f, 1.f) },
        { XYZ1(-1, -1, -1), XYZ1(0.f, 0.f, 1.f) },
        // yellow face
        { XYZ1(1, 1, 1), XYZ1(1.f, 1.f, 0.f) },
        { XYZ1(1, 1, -1), XYZ1(1.f, 1.f, 0.f) },
        { XYZ1(1, -1, 1), XYZ1(1.f, 1.f, 0.f) },
        { XYZ1(1, -1, 1), XYZ1(1.f, 1.f, 0.f) },
        { XYZ1(1, 1, -1), XYZ1(1.f, 1.f, 0.f) },
        { XYZ1(1, -1, -1), XYZ1(1.f, 1.f, 0.f) },
        // magenta face
        { XYZ1(1, 1, 1), XYZ1(1.f, 0.f, 1.f) },
        { XYZ1(-1, 1, 1), XYZ1(1.f, 0.f, 1.f) },
        { XYZ1(1, 1, -1), XYZ1(1.f, 0.f, 1.f) },
        { XYZ1(1, 1, -1), XYZ1(1.f, 0.f, 1.f) },
        { XYZ1(-1, 1, 1), XYZ1(1.f, 0.f, 1.f) },
        { XYZ1(-1, 1, -1), XYZ1(1.f, 0.f, 1.f) },
        // cyan face
        { XYZ1(1, -1, 1), XYZ1(0.f, 1.f, 1.f) },
        { XYZ1(1, -1, -1), XYZ1(0.f, 1.f, 1.f) },
        { XYZ1(-1, -1, 1), XYZ1(0.f, 1.f, 1.f) },
        { XYZ1(-1, -1, 1), XYZ1(0.f, 1.f, 1.f) },
        { XYZ1(1, -1, -1), XYZ1(0.f, 1.f, 1.f) },
        { XYZ1(-1, -1, -1), XYZ1(0.f, 1.f, 1.f) },
    };

    const void* vertexData = g_vb_solid_face_colors_Data;
    uint32_t dataSize = sizeof(g_vb_solid_face_colors_Data);
    uint32_t dataStride = sizeof(g_vb_solid_face_colors_Data[0]);
    bool use_texture = false;

    VkBufferCreateInfo buf_info = {};
    buf_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buf_info.pNext = NULL;
    buf_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    buf_info.size = dataSize;
    buf_info.queueFamilyIndexCount = 0;
    buf_info.pQueueFamilyIndices = NULL;
    buf_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    buf_info.flags = 0;
    VkResult res = vkCreateBuffer(info.device, &buf_info, NULL, &info.vertex_buffer.buffer);
    if (failed(res))
    {
        return res;
    }

    VkMemoryRequirements mem_reqs;
    vkGetBufferMemoryRequirements(info.device, info.vertex_buffer.buffer, &mem_reqs);

    VkMemoryAllocateInfo alloc_info = {};
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.pNext = NULL;
    alloc_info.memoryTypeIndex = 0;

    alloc_info.allocationSize = mem_reqs.size;
    bool pass = memory_type_from_properties(info, mem_reqs.memoryTypeBits,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        &alloc_info.memoryTypeIndex);
    if (!pass)
    {
        Logger::LogError("Vulkan: No mappable, coherent memory");
        return VK_ERROR;
    }

    res = vkAllocateMemory(info.device, &alloc_info, NULL, &(info.vertex_buffer.memory));
    if (failed(res))
    {
        return res;
    }

    info.vertex_buffer.buffer_info.range = mem_reqs.size;
    info.vertex_buffer.buffer_info.offset = 0;

    uint8_t *pData;
    res = vkMapMemory(info.device, info.vertex_buffer.memory, 0, mem_reqs.size, 0, (void **)&pData);
    if (failed(res))
    {
        return res;
    }

    memcpy(pData, vertexData, dataSize);

    vkUnmapMemory(info.device, info.vertex_buffer.memory);

    res = vkBindBufferMemory(info.device, info.vertex_buffer.buffer, info.vertex_buffer.memory, 0);
    if (failed(res))
    {
        return res;
    }

    info.vi_binding.binding = 0;
    info.vi_binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    info.vi_binding.stride = dataStride;

    info.vi_attribs[0].binding = 0;
    info.vi_attribs[0].location = 0;
    info.vi_attribs[0].format = VK_FORMAT_R32G32B32A32_SFLOAT;
    info.vi_attribs[0].offset = 0;
    info.vi_attribs[1].binding = 0;
    info.vi_attribs[1].location = 1;
    info.vi_attribs[1].format = use_texture ? VK_FORMAT_R32G32_SFLOAT : VK_FORMAT_R32G32B32A32_SFLOAT;
    info.vi_attribs[1].offset = 16;

    return VK_SUCCESS;
}

VkResult VulkanUtils::init_renderpass(VulkanData &info)
{
    bool include_depth = true;
    bool clear = true;
    VkImageLayout finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    /* Need attachments for render target and depth buffer */
    VkAttachmentDescription attachments[2];
    attachments[0].format = info.format;
    attachments[0].samples = NUM_SAMPLES;
    attachments[0].loadOp = clear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[0].finalLayout = finalLayout;
    attachments[0].flags = 0;

    if (include_depth) 
    {
        attachments[1].format = info.depth.format;
        attachments[1].samples = NUM_SAMPLES;
        attachments[1].loadOp = clear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
        attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        attachments[1].flags = 0;
    }

    VkAttachmentReference color_reference = {};
    color_reference.attachment = 0;
    color_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depth_reference = {};
    depth_reference.attachment = 1;
    depth_reference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.flags = 0;
    subpass.inputAttachmentCount = 0;
    subpass.pInputAttachments = NULL;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &color_reference;
    subpass.pResolveAttachments = NULL;
    subpass.pDepthStencilAttachment = include_depth ? &depth_reference : NULL;
    subpass.preserveAttachmentCount = 0;
    subpass.pPreserveAttachments = NULL;

    VkRenderPassCreateInfo rp_info = {};
    rp_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    rp_info.pNext = NULL;
    rp_info.attachmentCount = include_depth ? 2 : 1;
    rp_info.pAttachments = attachments;
    rp_info.subpassCount = 1;
    rp_info.pSubpasses = &subpass;
    rp_info.dependencyCount = 0;
    rp_info.pDependencies = NULL;

    return vkCreateRenderPass(info.device, &rp_info, NULL, &info.render_pass);
}

VkResult VulkanUtils::init_shaders(VulkanData& info)
{
    static const char *vertShaderText =
        "#version 400\n"
        "#extension GL_ARB_separate_shader_objects : enable\n"
        "#extension GL_ARB_shading_language_420pack : enable\n"
        "layout (std140, binding = 0) uniform bufferVals {\n"
        "    mat4 mvp;\n"
        "} myBufferVals;\n"
        "layout (location = 0) in vec4 pos;\n"
        "layout (location = 1) in vec4 inColor;\n"
        "layout (location = 0) out vec4 outColor;\n"
        "out gl_PerVertex { \n"
        "    vec4 gl_Position;\n"
        "};\n"
        "void main() {\n"
        "   outColor = inColor;\n"
        "   gl_Position = myBufferVals.mvp * pos;\n"
        "}\n";

    static const char *fragShaderText =
        "#version 400\n"
        "#extension GL_ARB_separate_shader_objects : enable\n"
        "#extension GL_ARB_shading_language_420pack : enable\n"
        "layout (location = 0) in vec4 color;\n"
        "layout (location = 0) out vec4 outColor;\n"
        "void main() {\n"
        "   outColor = color;\n"
        "}\n";

    if (!glslang::InitializeProcess())
    {
        return VK_ERROR;
    }

    VkShaderModuleCreateInfo moduleCreateInfo;
    if (vertShaderText) 
    {
        std::vector<unsigned int> vtx_spv;
        info.shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        info.shaderStages[0].pNext = NULL;
        info.shaderStages[0].pSpecializationInfo = NULL;
        info.shaderStages[0].flags = 0;
        info.shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
        info.shaderStages[0].pName = "main";

        if (!glsl_to_spb(VK_SHADER_STAGE_VERTEX_BIT, vertShaderText, vtx_spv))
        {
            return VK_ERROR;
        }

        moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        moduleCreateInfo.pNext = NULL;
        moduleCreateInfo.flags = 0;
        moduleCreateInfo.codeSize = vtx_spv.size() * sizeof(unsigned int);
        moduleCreateInfo.pCode = vtx_spv.data();
        VkResult res = vkCreateShaderModule(info.device, &moduleCreateInfo, NULL, &info.shaderStages[0].module);
        if (failed(res))
        {
            return res;
        }
    }

    if (fragShaderText) 
    {
        std::vector<unsigned int> frag_spv;
        info.shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        info.shaderStages[1].pNext = NULL;
        info.shaderStages[1].pSpecializationInfo = NULL;
        info.shaderStages[1].flags = 0;
        info.shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        info.shaderStages[1].pName = "main";

        if (!glsl_to_spb(VK_SHADER_STAGE_FRAGMENT_BIT, fragShaderText, frag_spv))
        {
            return VK_ERROR;
        }

        moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        moduleCreateInfo.pNext = NULL;
        moduleCreateInfo.flags = 0;
        moduleCreateInfo.codeSize = frag_spv.size() * sizeof(unsigned int);
        moduleCreateInfo.pCode = frag_spv.data();
        VkResult res = vkCreateShaderModule(info.device, &moduleCreateInfo, NULL, &info.shaderStages[1].module);
        if (failed(res))
        {
            return res;
        }
    }

    glslang::FinalizeProcess();
    return VK_SUCCESS;
}

VkResult VulkanUtils::init_framebuffers(VulkanData &info)
{
    bool include_depth = true;

    VkImageView attachments[2];
    attachments[1] = info.depth.view;

    VkFramebufferCreateInfo fb_info = {};
    fb_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    fb_info.pNext = NULL;
    fb_info.renderPass = info.render_pass;
    fb_info.attachmentCount = include_depth ? 2 : 1;
    fb_info.pAttachments = attachments;
    fb_info.width = WINDOW_WIDTH;
    fb_info.height = WINDOW_HEIGHT;
    fb_info.layers = 1;

    uint32_t i;

    info.framebuffers.resize(info.swapchainImageCount);

    for (i = 0; i < info.swapchainImageCount; i++) 
    {
        attachments[0] = info.buffers[i].view;
        VkResult res = vkCreateFramebuffer(info.device, &fb_info, NULL, &info.framebuffers[i]);
        if (failed(res))
        {
            return res;
        }
    }
    return VK_SUCCESS;
}

VkResult VulkanUtils::init_viewports(VulkanData& info)
{
    info.clear_values[0].color.float32[0] = 0.2f;
    info.clear_values[0].color.float32[1] = 0.2f;
    info.clear_values[0].color.float32[2] = 0.2f;
    info.clear_values[0].color.float32[3] = 0.2f;
    info.clear_values[1].depthStencil.depth = 1.0f;
    info.clear_values[1].depthStencil.stencil = 0;

    info.viewport.height = (float)WINDOW_HEIGHT;
    info.viewport.width = (float)WINDOW_WIDTH;
    info.viewport.minDepth = (float)0.0f;
    info.viewport.maxDepth = (float)1.0f;
    info.viewport.x = 0;
    info.viewport.y = 0;
    vkCmdSetViewport(info.cmd, 0, NUM_VIEWPORTS_AND_SCISSORS, &info.viewport);
    return VK_SUCCESS;
}

VkResult VulkanUtils::init_scissors(VulkanData& info)
{
    info.scissor.extent.width = WINDOW_WIDTH;
    info.scissor.extent.height = WINDOW_HEIGHT;
    info.scissor.offset.x = 0;
    info.scissor.offset.y = 0;
    vkCmdSetScissor(info.cmd, 0, NUM_VIEWPORTS_AND_SCISSORS, &info.scissor);
    return VK_SUCCESS;
}

VkResult VulkanUtils::init_semaphores(VulkanData& info)
{
    VkSemaphoreCreateInfo imageAcquiredSemaphoreCreateInfo;
    imageAcquiredSemaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    imageAcquiredSemaphoreCreateInfo.pNext = NULL;
    imageAcquiredSemaphoreCreateInfo.flags = 0;
    return vkCreateSemaphore(info.device, &imageAcquiredSemaphoreCreateInfo, NULL, &info.imageAcquiredSemaphore);
}

VkResult VulkanUtils::init_fence(VulkanData& info)
{
    VkFenceCreateInfo fenceInfo;
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.pNext = NULL;
    fenceInfo.flags = 0;
    return vkCreateFence(info.device, &fenceInfo, NULL, &info.drawFence);
}

void VulkanUtils::begin_command_buffer(VulkanData &info)
{
    VkCommandBufferBeginInfo cmd_buf_info = {};
    cmd_buf_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmd_buf_info.pNext = NULL;
    cmd_buf_info.flags = 0;
    cmd_buf_info.pInheritanceInfo = NULL;
    FAILED(vkBeginCommandBuffer(info.cmd, &cmd_buf_info));
}

void VulkanUtils::end_command_buffer(VulkanData &info)
{
    FAILED(vkEndCommandBuffer(info.cmd));
}