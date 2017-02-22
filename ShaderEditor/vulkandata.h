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
    VkImage image = VK_NULL_HANDLE;
    VkImageView view = VK_NULL_HANDLE;
};

struct Depth
{
    VkFormat format = VK_FORMAT_UNDEFINED;
    VkImage image = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
    VkImageView view = VK_NULL_HANDLE;
};

struct UniformData
{
    VkBuffer buffer = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
    VkDescriptorBufferInfo buffer_info = {};
};

struct VertexBuffer
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

    // TODO: Temporary until reorganising engine
    glm::mat4 Model;
    glm::mat4 Clip;
    glm::mat4 MVP;

    VkInstance instance;
    VkDevice device;
    VkCommandBuffer cmd;
    VkSurfaceKHR surface;
    VkSwapchainKHR swap_chain;
    VkQueue graphics_queue;
    VkQueue present_queue;
    Depth depth;
    VkPhysicalDeviceMemoryProperties memory_properties;
    VkPhysicalDeviceProperties gpu_props;
    UniformData uniform_data;
    VkPipelineLayout pipeline_layout;
    VkSemaphore imageAcquiredSemaphore;
    VkPipeline pipeline;
    VkPipelineCache pipelineCache;
    VkDescriptorPool desc_pool;
    VertexBuffer vertex_buffer;
    uint32_t current_buffer;
    uint32_t queue_family_count;
    uint32_t graphics_queue_family_index;
    uint32_t present_queue_family_index;
    uint32_t swapchainImageCount;
    VkCommandPool cmd_pool;
    VkFormat format;
    VkRenderPass render_pass;
    VkVertexInputBindingDescription vi_binding;
    VkDescriptorImageInfo image_info;
    VkViewport viewport;
    VkRect2D scissor;
    VkFence drawFence;

    VkDebugReportCallbackEXT debug_callback;
    PFN_vkCreateDebugReportCallbackEXT CreateDebugReportFn;
    PFN_vkDestroyDebugReportCallbackEXT DestroyDebugReportFn;

    std::vector<VkFramebuffer> framebuffers;
    std::vector<SwapChainBuffer> buffers;
    std::vector<VkQueueFamilyProperties> queue_props;
    std::vector<LayerProperties> instance_layer_properties;
    std::vector<const char*> instance_layer_names;
    std::vector<const char*> instance_extension_names;
    std::vector<VkPhysicalDevice> gpus;
    std::vector<const char*> device_extension_names;
    std::vector<VkDescriptorSetLayout> desc_layout;
    std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
    std::vector<VkVertexInputAttributeDescription> vi_attribs;
    std::vector<VkDescriptorSet> desc_set;
    std::vector<VkClearValue> clear_values;
};