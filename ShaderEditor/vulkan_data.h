////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkan_data.h
////////////////////////////////////////////////////////////////////////////////////////

#include "vulkan.h"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include <vector>
#include <memory>

class VkShader;
class VkMesh;

/**
* Information on layers supported in Vulkan
*/
struct LayerProperties
{
    VkLayerProperties properties;
    std::vector<VkExtensionProperties> extensions;
};

/**
* Holds swap chain image and view data
*/
struct SwapChainBuffer 
{
    VkImage image = VK_NULL_HANDLE;
    VkImageView view = VK_NULL_HANDLE;
};

/**
* Holds depth buffer data
*/
struct Depth
{
    VkFormat format = VK_FORMAT_UNDEFINED;
    VkImage image = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
    VkImageView view = VK_NULL_HANDLE;
};

/**
* Data for the Vulkan Engine
*/
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

    /**
    * @return the currently selected shader
    */
    VkShader& SelectedShader();

    /**
    * @return the currently selected command buffer
    */
    VkCommandBuffer& SelectedCmd();

    HINSTANCE connection;                                          ///< Application instance
    HWND window;                                                   ///< Application window
    VkInstance instance;                                           ///< Vulkan instance
    VkDevice device;                                               ///< Vulkan device
    VkSurfaceKHR surface;                                          ///< Surface to present the swap chain to
    VkSwapchainKHR swapChain;                                      ///< Handle to the swap chain
    VkQueue graphicsQueue;                                         ///< Handle to the device queue that command buffers are submitted to
    VkQueue presentQueue;                                          ///< Handle to the device queue that is presented to the screen
    Depth depth;                                                   ///< Depth buffer
    VkPhysicalDeviceMemoryProperties memoryProperties;             ///< Stores available memory properties for the physical device
    VkPhysicalDeviceProperties gpuProps;                           ///< Properties for the physical gpu device
    VkSemaphore presentCompleteSemaphore;                          ///< For synchronising rendering frames
    VkSemaphore renderCompleteSemaphore;                           ///< For synchronising rendering frames
    uint32_t currentBuffer;                                        ///< Current selected frame and command buffer
    uint32_t queueFamilyCount;                                     ///< Number of supported queue families
    uint32_t graphicsQueueFamilyIndex;                             ///< Index of queue family that supports rendering
    uint32_t presentQueueFamilyIndex;                              ///< Index of queue family that supports presenting
    uint32_t swapchainImageCount;                                  ///< Number of swap chain images
    VkCommandPool cmdPool;                                         ///< Pool to allocate command buffers
    VkFormat format;                                               ///< Supported swap chain format
    VkRenderPass renderPass;                                       ///< Used for rendering to the frame buffers
    VkPipelineStageFlags pipeStageFlags;                           ///< Stages where the corresponding semaphore wait will occur
    VkClearValue clearValues[2];                                   ///< Buffer clear values/color                                                    
    VkDebugReportCallbackEXT debugCallback;                        ///< Debug report callback information
    PFN_vkCreateDebugReportCallbackEXT createDebugReportFn;        ///< Function pointer for creating the debug report
    PFN_vkDestroyDebugReportCallbackEXT destroyDebugReportFn;      ///< Function pointer for destroying the debug report
    PFN_vkDebugMarkerSetObjectNameEXT setDebugNameFn;              ///< Function pointer for setting the debug name

    std::vector<VkFence> fences;                                   ///< For synchronising queues
    std::vector<VkFramebuffer> framebuffers;                       ///< Collection of memory attachments that a render pass uses
    std::vector<VkCommandBuffer> cmd;                              ///< Command buffers to record render information
    std::vector<SwapChainBuffer> buffers;                          ///< Swap chain image and view data
    std::vector<VkQueueFamilyProperties> queueProps;               ///< Properties of the queues of the physical device
    std::vector<LayerProperties> instanceLayerProperties;          ///< Information on layers supported
    std::vector<const char*> instanceLayerNames;                   ///< Layer names supported
    std::vector<VkPhysicalDevice> gpus;                            ///< Handles to the physical gpu devices
    std::vector<const char*> instanceExtensionNames;               ///< Names of the extensions to use for the instance
    std::vector<const char*> deviceExtensionNames;                 ///< Names of the extensions to use for the device
    std::vector<std::unique_ptr<VkShader>> shaders;                ///< Shaders shared by all meshes
    std::vector<std::unique_ptr<VkMesh>> meshes;                   ///< Each mesh in the scene
                                                                   
    glm::vec3 cameraPosition;                                      ///< Position of the camera
    glm::vec3 cameraUp;                                            ///< The up vector of the camera
    glm::mat4 projection;                                          ///< Projection matrix
    glm::mat4 view;                                                ///< View matrix
    glm::mat4 viewProjection;                                      ///< View projection matrix
    int selectedShader;                                            ///< Currently active shader for rendering
};