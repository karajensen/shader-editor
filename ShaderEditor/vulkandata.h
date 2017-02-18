////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkandata.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "vulkan.h"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include <vector>

struct LayerProperties
{
    VkLayerProperties properties;
    std::vector<VkExtensionProperties> extensions;
};

struct SwapChainBuffer 
{
    VkImage image;
    VkImageView view;
};

struct Depth
{
    VkFormat format = VK_FORMAT_UNDEFINED;
    VkImage image = 0;
    VkDeviceMemory mem = 0;
    VkImageView view = 0;
};

struct UniformData
{
    VkBuffer buffer = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
    VkDescriptorBufferInfo buffer_info = {};
};

struct VulkanData
{
    /**
    * Constructor
    */
    VulkanData(HINSTANCE hinstance, HWND hwnd);

    /**
    * Destructor
    */
    ~VulkanData();

    /**
    * Releases resources
    */
    void Release();

    /**
    * Resets resources
    */
    void Reset();

    HINSTANCE connection;
    HWND window;

    glm::vec3 cameraPosition;  ///< Position of the camera
    glm::vec3 cameraUp;        ///< The up vector of the camera
    glm::mat4 projection;      ///< Projection matrix
    glm::mat4 view;            ///< View matrix
    glm::mat4 viewProjection;  ///< View projection matrix

    VkInstance instance;
    VkDevice device;
    VkCommandBuffer cmd;
    VkSurfaceKHR surface;
    VkSwapchainKHR swap_chain;

    VkDebugReportCallbackEXT debug_callback;
    PFN_vkCreateDebugReportCallbackEXT CreateDebugReportFn;
    PFN_vkDestroyDebugReportCallbackEXT DestroyDebugReportFn;

    Depth depth;
    VkPhysicalDeviceMemoryProperties memory_properties;
    VkPhysicalDeviceProperties gpu_props;
    UniformData uniform_data;

    std::vector<SwapChainBuffer> buffers;
    std::vector<VkQueueFamilyProperties> queue_props;
    std::vector<LayerProperties> instance_layer_properties;
    std::vector<const char*> instance_layer_names;
    std::vector<const char*> instance_extension_names;
    std::vector<VkPhysicalDevice> gpus;
    std::vector<const char*> device_extension_names;

    uint32_t queue_family_count;
    uint32_t graphics_queue_family_index;
    uint32_t present_queue_family_index;
    uint32_t swapchainImageCount;
    VkCommandPool cmd_pool;
    VkFormat format;
};