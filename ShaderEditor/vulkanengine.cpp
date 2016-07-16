////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkanengine.cpp
// Reference: https://github.com/SaschaWillems/Vulkan
////////////////////////////////////////////////////////////////////////////////////////

#include "vulkanengine.h"
#include "vulkanfunctions.h"
#include "sceneInterface.h"
#include <array>
#include <fstream>

#include "vulkancommon.h"

/**
* Internal data for the vulkan rendering engine
*/
struct VulkanData
{
    /**
    * Constructor
    */
    VulkanData();

    /**
    * Destructor
    */
    ~VulkanData();

    /**
    * Releases the device/context
    */
    void Release();

    struct SwapChainBuffer 
    {
        VkImage Image;
        VkImageView View;
    };

    VkInstance Instance;
    VkDevice Device;
    VkPhysicalDevice PhysicalDevice;
    VkSurfaceKHR Surface;
    VkSwapchainKHR SwapChain = VK_NULL_HANDLE;
    std::vector<VkImage> Images;
    std::vector<SwapChainBuffer> Buffers;
    uint32_t QueueNodeIndex = UINT32_MAX;
    VkFormat ColorFormat = VK_FORMAT_UNDEFINED;
    VkFormat DepthFormat = VK_FORMAT_UNDEFINED;
    VkColorSpaceKHR ColorSpace;
    VkPhysicalDeviceFeatures EnabledFeatures = {};
    VkPhysicalDeviceProperties DeviceProperties;
    VkPhysicalDeviceFeatures DeviceFeatures;
    VkPhysicalDeviceMemoryProperties DeviceMemoryProperties;
    VkPipelineStageFlags SubmitPipelineStages = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    VkQueue Queue;
    VkSemaphore PresentCompleteSemaphore;
    VkSemaphore RenderCompleteSemaphore;
    VulkanFunctions Fn;

    bool EnableValidation = true;
    int ValidationLayerCount = 1;
    const char* ValidationLayerNames = "VK_LAYER_LUNARG_standard_validation";
    VkDebugReportCallbackEXT DebugCallback;
};

VulkanData::VulkanData()
{
}

VulkanData::~VulkanData()
{
    Release();
}

void VulkanData::Release()
{
    for (size_t i = 0; i < Buffers.size(); ++i)
    {
        vkDestroyImageView(Device, Buffers[i].View, nullptr);
    }

    Fn.DestroySwapchain(Device, SwapChain, nullptr);
    vkDestroySurfaceKHR(Instance, Surface, nullptr);

    vkDestroySemaphore(Device, PresentCompleteSemaphore, nullptr);
    vkDestroySemaphore(Device, RenderCompleteSemaphore, nullptr);

    vkDestroyDevice(Device, nullptr);

    if (DebugCallback != VK_NULL_HANDLE)
    {
        Fn.DestroyDebugReport(Instance, DebugCallback, nullptr);
    }

    vkDestroyInstance(Instance, nullptr);
}

VulkanEngine::VulkanEngine(HWND hwnd, HINSTANCE hinstance) :
    m_data(new VulkanData()),
    m_hinstance(hinstance),
    m_hwnd(hwnd)
{
}

VulkanEngine::~VulkanEngine()
{
    Release();
}

void VulkanEngine::Release()
{
    m_data->Release();
}

bool VulkanEngine::Initialize()
{
    if (InitializeInstance() &&
        InitializeEngine() &&
        InitializeSwapChain() &&
        InitializeDebugging())
    {
        Logger::LogInfo("Vulkan: Initialize sucessful");
        return true;
    }
    return false;
}

bool VulkanEngine::InitializeInstance()
{
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "";
    appInfo.pEngineName = "";
    appInfo.apiVersion = VK_API_VERSION_1_0;

    std::vector<const char*> enabledExtensions = { VK_KHR_SURFACE_EXTENSION_NAME };
    enabledExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);

    VkInstanceCreateInfo instanceCreateInfo = {};
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pNext = NULL;
    instanceCreateInfo.pApplicationInfo = &appInfo;
    if (enabledExtensions.size() > 0)
    {
        if (m_data->EnableValidation)
        {
            enabledExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
        }
        instanceCreateInfo.enabledExtensionCount = (uint32_t)enabledExtensions.size();
        instanceCreateInfo.ppEnabledExtensionNames = enabledExtensions.data();
    }
    if (m_data->EnableValidation)
    {
        instanceCreateInfo.enabledLayerCount = m_data->ValidationLayerCount;
        instanceCreateInfo.ppEnabledLayerNames = &m_data->ValidationLayerNames;
    }
    
    if (FAIL(vkCreateInstance(&instanceCreateInfo, nullptr, &m_data->Instance)))
    {
        Logger::LogError("Vulkan: vkCreateInstance failed");
        return false;
    }
    return true;
}

bool VulkanEngine::InitializeEngine()
{
    uint32_t gpuCount = 0;

    // Get number of available physical devices
    if (FAIL(vkEnumeratePhysicalDevices(m_data->Instance, &gpuCount, nullptr)))
    {
        Logger::LogError("Vulkan: vkEnumeratePhysicalDevices failed");
        return false;
    }
    assert(gpuCount > 0);

    // Enumerate devices
    std::vector<VkPhysicalDevice> physicalDevices(gpuCount);
    if(FAIL(vkEnumeratePhysicalDevices(m_data->Instance, &gpuCount, physicalDevices.data())))
    {
        Logger::LogError("Vulkan: Could not enumerate phyiscal devices");
        return false;
    }

    // use the first physical device reported, change the vector index if you have 
    // multiple Vulkan devices installed and want to use another one
    m_data->PhysicalDevice = physicalDevices[0];

    // Find a queue that supports graphics operations
    uint32_t graphicsQueueIndex = 0;
    uint32_t queueCount;
    vkGetPhysicalDeviceQueueFamilyProperties(m_data->PhysicalDevice, &queueCount, NULL);
    assert(queueCount >= 1);

    std::vector<VkQueueFamilyProperties> queueProps;
    queueProps.resize(queueCount);
    vkGetPhysicalDeviceQueueFamilyProperties(m_data->PhysicalDevice, &queueCount, queueProps.data());

    for (graphicsQueueIndex = 0; graphicsQueueIndex < queueCount; graphicsQueueIndex++)
    {
        if (queueProps[graphicsQueueIndex].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            break;
        }
    }
    assert(graphicsQueueIndex < queueCount);

    // Create the Vulkan device
    std::array<float, 1> queuePriorities = { 0.0f };
    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = graphicsQueueIndex;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = queuePriorities.data();

    std::vector<const char*> enabledExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pNext = NULL;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
    deviceCreateInfo.pEnabledFeatures = &m_data->EnabledFeatures;

    if (enabledExtensions.size() > 0)
    {
        deviceCreateInfo.enabledExtensionCount = (uint32_t)enabledExtensions.size();
        deviceCreateInfo.ppEnabledExtensionNames = enabledExtensions.data();
    }

    if (FAIL(vkCreateDevice(m_data->PhysicalDevice, &deviceCreateInfo, nullptr, &m_data->Device)))
    {
        Logger::LogError("Vulkan: vkCreateDevice failed");
        return false;
    }

    // Store properties (including limits) and features of the phyiscal device
    // So examples can check against them and see if a feature is actually supported
    vkGetPhysicalDeviceProperties(m_data->PhysicalDevice, &m_data->DeviceProperties);
    vkGetPhysicalDeviceFeatures(m_data->PhysicalDevice, &m_data->DeviceFeatures);

    // Gather physical device memory properties
    vkGetPhysicalDeviceMemoryProperties(m_data->PhysicalDevice, &m_data->DeviceMemoryProperties);

    // Get the graphics queue
    vkGetDeviceQueue(m_data->Device, graphicsQueueIndex, 0, &m_data->Queue);

    // Find a suitable depth format
    // Start with the highest precision packed format
    std::vector<VkFormat> depthFormats = {
        VK_FORMAT_D32_SFLOAT_S8_UINT,
        VK_FORMAT_D32_SFLOAT,
        VK_FORMAT_D24_UNORM_S8_UINT,
        VK_FORMAT_D16_UNORM_S8_UINT,
        VK_FORMAT_D16_UNORM
    };

    for (auto& format : depthFormats)
    {
        VkFormatProperties formatProps;
        vkGetPhysicalDeviceFormatProperties(m_data->PhysicalDevice, format, &formatProps);
        
        // Format must support depth stencil attachment for optimal tiling
        if (formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
        {
            m_data->DepthFormat = format;
            break;
        }
    }

    if (m_data->DepthFormat == VK_FORMAT_UNDEFINED)
    {
        Logger::LogError("Vulkan: Could not find valid depth format");
        return false;
    }

    // Get all required function pointers
    if (!m_data->Fn.Initialise(m_data->Instance, m_data->Device))
    {
        Logger::LogError("Vulkan: Function pointer initialisation failed");
        return false;
    }

    // Create synchronization objects
    VkSemaphoreCreateInfo semaphoreCreateInfo = {};
    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreCreateInfo.pNext = NULL;
    semaphoreCreateInfo.flags = 0;

    // Create a semaphore used to synchronize image presentation
    // Ensures that the image is displayed before we start submitting new commands to the queu
    if (FAIL(vkCreateSemaphore(m_data->Device, &semaphoreCreateInfo, nullptr, &m_data->PresentCompleteSemaphore)))
    {
        Logger::LogError("Vulkan: vkCreateSemaphore failed");
        return false;
    }
    
    // Create a semaphore used to synchronize command submission
    // Ensures that the image is not presented until all commands have been sumbitted and executed
    if (FAIL(vkCreateSemaphore(m_data->Device, &semaphoreCreateInfo, nullptr, &m_data->RenderCompleteSemaphore)))
    {
        Logger::LogError("Vulkan: vkCreateSemaphore failed");
        return false;
    }

    // Set up submit info structure
    // Semaphores will stay the same during application lifetime
    // Command buffer submission info is set by each example
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = NULL;
    submitInfo.pWaitDstStageMask = &m_data->SubmitPipelineStages;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &m_data->PresentCompleteSemaphore;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &m_data->RenderCompleteSemaphore;

    return true;
}

bool VulkanEngine::InitializeSwapChain()
{
    VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {};
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    surfaceCreateInfo.hinstance = m_hinstance;
    surfaceCreateInfo.hwnd = m_hwnd;
    if (FAIL(vkCreateWin32SurfaceKHR(m_data->Instance, &surfaceCreateInfo, nullptr, &m_data->Surface)))
    {
        Logger::LogError("Vulkan: vkCreateWin32SurfaceKHR failed");
        return false;
    }

    uint32_t queueCount;
    vkGetPhysicalDeviceQueueFamilyProperties(m_data->PhysicalDevice, &queueCount, 0);
    assert(queueCount >= 1);

    std::vector<VkQueueFamilyProperties> queueProps(queueCount);
    vkGetPhysicalDeviceQueueFamilyProperties(m_data->PhysicalDevice, &queueCount, queueProps.data());

    // Iterate over each queue to learn whether it supports presenting
    // Will be used to present the swap chain images to the windowing system
    std::vector<VkBool32> supportsPresent(queueCount);
    for (uint32_t i = 0; i < queueCount; i++)
    {
        if (FAIL(vkGetPhysicalDeviceSurfaceSupportKHR(m_data->PhysicalDevice, i, m_data->Surface, &supportsPresent[i])))
        {
            Logger::LogError("Vulkan: getPhysicalDeviceSurfaceSupportKHR failed for " + std::to_string(i));
            return false;
        }
    }

    // Search for a graphics and a present queue in the array 
    // of queue families, try to find one that supports both
    uint32_t graphicsQueueNodeIndex = UINT32_MAX;
    uint32_t presentQueueNodeIndex = UINT32_MAX;
    for (uint32_t i = 0; i < queueCount; ++i)
    {
        if ((queueProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0)
        {
            if (graphicsQueueNodeIndex == UINT32_MAX)
            {
                graphicsQueueNodeIndex = i;
            }

            if (supportsPresent[i] == VK_TRUE)
            {
                graphicsQueueNodeIndex = i;
                presentQueueNodeIndex = i;
                break;
            }
        }
    }

    if (presentQueueNodeIndex == UINT32_MAX)
    {
        // If there's no queue that supports both present and graphics
        // try to find a separate present queue
        for (uint32_t i = 0; i < queueCount; ++i)
        {
            if (supportsPresent[i] == VK_TRUE)
            {
                presentQueueNodeIndex = i;
                break;
            }
        }
    }

    // Exit if either a graphics or a presenting queue hasn't been found
    if (graphicsQueueNodeIndex == UINT32_MAX || presentQueueNodeIndex == UINT32_MAX)
    {
        Logger::LogError("Vulkan: Could not find a graphics and/or presenting queue");
        return false;
    }
    if (graphicsQueueNodeIndex != presentQueueNodeIndex)
    {
        Logger::LogError("Vulkan: Separate graphics and presenting queues are not supported yet");
        return false;
    }

    m_data->QueueNodeIndex = graphicsQueueNodeIndex;

    // Get list of supported surface formats
    uint32_t formatCount;
    if (FAIL(vkGetPhysicalDeviceSurfaceFormatsKHR(m_data->PhysicalDevice, m_data->Surface, &formatCount, 0)))
    {
        Logger::LogError("Vulkan: getPhysicalDeviceSurfaceFormatsKHR failed");
        return false;
    }
    assert(formatCount > 0);

    std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
    if (FAIL(vkGetPhysicalDeviceSurfaceFormatsKHR(m_data->PhysicalDevice, m_data->Surface, &formatCount, surfaceFormats.data())))
    {
        Logger::LogError("Vulkan: getPhysicalDeviceSurfaceFormatsKHR failed");
        return false;
    }

    // If the surface format list only includes one entry with VK_FORMAT_UNDEFINED,
    // there is no preferered format, so we assume VK_FORMAT_B8G8R8A8_UNORM
    if ((formatCount == 1) && (surfaceFormats[0].format == VK_FORMAT_UNDEFINED))
    {
        m_data->ColorFormat = VK_FORMAT_B8G8R8A8_UNORM;
    }
    else
    {
        // Always select the first available color format
        // If you need a specific format (e.g. SRGB) you'd need to
        // iterate over the list of available surface format and check for it's presence
        m_data->ColorFormat = surfaceFormats[0].format;
    }
    m_data->ColorSpace = surfaceFormats[0].colorSpace;

    return true;
}

bool VulkanEngine::InitializeDebugging()
{
    return true;
}

std::string VulkanEngine::CompileShader(int index)
{
    return "";
}

bool VulkanEngine::InitialiseScene(const IScene& scene)
{
    return ReInitialiseScene();
}

bool VulkanEngine::ReInitialiseScene()
{
    Logger::LogInfo("Vulkan: Re-Initialised");
    return true;
}

bool VulkanEngine::FadeView(bool in, float amount)
{
    return true;
}

void VulkanEngine::Render(const IScene& scene, float timer)
{

}

void VulkanEngine::ToggleWireframe()
{

}

std::string VulkanEngine::GetName() const
{
    return "Vulkan";
}

void VulkanEngine::UpdateView(const Matrix& world)
{
}

std::string VulkanEngine::GetShaderText(int index) const
{
    return "";
}

std::string VulkanEngine::GetShaderAssembly(int index)
{
    return "";
}

void VulkanEngine::SetFade(float value)
{
    
}

void VulkanEngine::ReloadTerrain(int index)
{
}

void VulkanEngine::ReloadTexture(int index)
{
}

void VulkanEngine::WriteToShader(const Shader& shader,
                                 const std::string& text)
{
}