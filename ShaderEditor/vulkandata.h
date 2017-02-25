////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkandata.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "vulkan.h"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include <vector>
#include <memory>

class VkShader;

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
    VkDescriptorBufferInfo bufferInfo = {};
};

struct VertexBuffer
{
    VkBuffer buffer = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
    VkDescriptorBufferInfo bufferInfo = {};
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
    VkInstance instance;
    VkDevice device;
    VkCommandBuffer cmd;
    VkSurfaceKHR surface;
    VkSwapchainKHR swapChain;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    Depth depth;
    VkPhysicalDeviceMemoryProperties memoryProperties;
    VkPhysicalDeviceProperties gpuProps;
    UniformData uniformData;
    VkPipelineLayout pipelineLayout;
    VkSemaphore imageAcquiredSemaphore;
    VkPipeline pipeline;
    VkPipelineCache pipelineCache;
    VkDescriptorPool descPool;
    VertexBuffer vertexBuffer;
    uint32_t currentBuffer;
    uint32_t queueFamilyCount;
    uint32_t graphicsQueueFamilyIndex;
    uint32_t presentQueueFamilyIndex;
    uint32_t swapchainImageCount;
    VkCommandPool cmdPool;
    VkFormat format;
    VkRenderPass renderPass;
    VkVertexInputBindingDescription viBinding;
    VkDescriptorImageInfo imageInfo;
    VkViewport viewport;
    VkRect2D scissor;
    VkFence drawFence;

    VkDebugReportCallbackEXT debugCallback;
    PFN_vkCreateDebugReportCallbackEXT createDebugReportFn;
    PFN_vkDestroyDebugReportCallbackEXT destroyDebugReportFn;

    std::vector<VkFramebuffer> framebuffers;
    std::vector<SwapChainBuffer> buffers;
    std::vector<VkQueueFamilyProperties> queueProps;
    std::vector<LayerProperties> instanceLayerProperties;
    std::vector<const char*> instanceLayerNames;
    std::vector<const char*> instanceExtensionNames;
    std::vector<VkPhysicalDevice> gpus;
    std::vector<const char*> deviceExtensionNames;
    std::vector<VkDescriptorSetLayout> descLayout;
    std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
    std::vector<VkVertexInputAttributeDescription> viAttribs;
    std::vector<VkDescriptorSet> descSet;
    std::vector<VkClearValue> clearValues;

    std::vector<std::unique_ptr<VkShader>> shaders;  ///< Shaders shared by all meshes

    glm::vec3 cameraPosition;  ///< Position of the camera
    glm::vec3 cameraUp;        ///< The up vector of the camera
    glm::mat4 projection;      ///< Projection matrix
    glm::mat4 view;            ///< View matrix
    glm::mat4 viewProjection;  ///< View projection matrix

    // TODO: Temporary until reorganising engine
    glm::mat4 model;
    glm::mat4 clip;
    glm::mat4 mvp;
};