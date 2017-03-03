////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkandata.h
////////////////////////////////////////////////////////////////////////////////////////

#include "vulkan.h"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include <vector>
#include <memory>

class VkShader;
class VkMesh;

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
    VkSurfaceKHR surface;
    VkSwapchainKHR swapChain;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    Depth depth;
    VkPhysicalDeviceMemoryProperties memoryProperties;
    VkPhysicalDeviceProperties gpuProps;
    VkSemaphore presentCompleteSemaphore;
    VkSemaphore renderCompleteSemaphore;
    VkPipelineCache pipelineCache;
    VkDescriptorPool descPool;
    uint32_t currentBuffer;
    uint32_t queueFamilyCount;
    uint32_t graphicsQueueFamilyIndex;
    uint32_t presentQueueFamilyIndex;
    uint32_t swapchainImageCount;
    VkCommandPool cmdPool;
    VkFormat format;
    VkRenderPass renderPass;
    VkPipelineLayout pipelineLayout;
    VkDescriptorImageInfo imageInfo;
    VkPipelineStageFlags pipeStageFlags;
    VkClearValue clearValues[2];
    VkDescriptorSetLayout descLayout;
    VkDescriptorSet descSet;

    VkDebugReportCallbackEXT debugCallback;
    PFN_vkCreateDebugReportCallbackEXT createDebugReportFn;
    PFN_vkDestroyDebugReportCallbackEXT destroyDebugReportFn;
    PFN_vkDebugMarkerSetObjectNameEXT setDebugNameFn;

    std::vector<VkFence> fences;
    std::vector<VkFramebuffer> framebuffers;
    std::vector<VkCommandBuffer> cmd;
    std::vector<SwapChainBuffer> buffers;
    std::vector<VkQueueFamilyProperties> queueProps;
    std::vector<LayerProperties> instanceLayerProperties;
    std::vector<const char*> instanceLayerNames;
    std::vector<const char*> instanceExtensionNames;
    std::vector<VkPhysicalDevice> gpus;
    std::vector<const char*> deviceExtensionNames;

    std::vector<std::unique_ptr<VkShader>> shaders;  ///< Shaders shared by all meshes
    std::vector<std::unique_ptr<VkMesh>> meshes;     ///< Each mesh in the scene

    glm::vec3 cameraPosition;                        ///< Position of the camera
    glm::vec3 cameraUp;                              ///< The up vector of the camera
    glm::mat4 projection;                            ///< Projection matrix
    glm::mat4 view;                                  ///< View matrix
    glm::mat4 viewProjection;                        ///< View projection matrix
    int selectedShader;                              ///< Currently active shader for rendering
};