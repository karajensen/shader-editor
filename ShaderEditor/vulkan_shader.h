////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkan_shader.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "vulkan.h"
#include <string>
#include <vector>
#include "glm/mat4x4.hpp"

struct VulkanData;
class Shader;

/**
* Holds information for an vulkan shader
*/
class VkShader
{
public:

    /**
    * Constructor
    * @param info Data for the Vulkan Engine
    * @param shader The shader data to create
    */
    VkShader(VulkanData& info, const Shader& shader);

    /**
    * Destructor
    */
    ~VkShader();

    /**
    * Initialises Vulkan shader framework
    */
    static bool InitFramework();

    /**
    * Releases Vulkan shader framework
    */
    static void ReleaseFramework();

    /**
    * Initialises the shader object
    * @return Error message if failed or empty if succeeded
    */
    std::string Initialise();

    /**
    * Releases the shader object
    */
    void Release();

    /**
    * @return the the name of the shader
    */
    const std::string& GetName() const;

    /**
    * Generates the shader for the engine
    * @return Error message if failed or empty if succeeded
    */
    std::string CompileShader();

    /**
    * Sets the shader as activated for rendering
    */
    void SetActive();

    /**
    * @return the text for the shader
    */
    std::string GetText() const;

    /**
    * @return the assembly for the shader
    */
    std::string GetAssembly();

    /**
    * Bind the shader to the command buffer
    */
    void Bind();

    /**
    * Sends the uniform buffer to the shader
    */
    void SendUniformBuffer();

private:

    /**
    * Initialises a descripter pool to allocate descripter sets
    * @param info Data for the Vulkan Engine
    */
    bool InitDescriptorPool(VulkanData& info);

    /**
    * Specifies the types of resources that are going to be accessed by the pipeline
    * @param info Data for the Vulkan Engine
    */
    bool InitDescriptorAndPipelineLayouts(VulkanData &info);

    /**
    * Specifies the actual buffer or image resources that will be bound
    * @param info Data for the Vulkan Engine
    */
    bool InitDescriptorSet(VulkanData& info);

    /**
    * Initialises the shader uniform buffers
    * @param info Data for the Vulkan Engine
    */
    bool InitUniformBuffer(VulkanData& info);

    /**
    * Initialises the shader pipeline which describes the shader stages
    * @param info Data for the Vulkan Engine
    */
    bool InitPipeline(VulkanData& info);

    /**
    * Parse a GLSL shader into SPRIV
    */
    std::string Parse(const VkShaderStageFlagBits shader_type,  
                      const char* pshader, 
                      std::vector<unsigned int>& spirv);

    /**
    * Releases the shader modules
    */
    void ReleaseShaderModules();

    /**
    * Resets the shader resource values to default
    */
    void Reset();

    /**
    * TODO: Make this generic
    */
    struct SceneData
    {
        glm::mat4 viewProj;
    };
    
    /**
    * Data for a uniform buffer
    */
    struct UniformBuffer
    {
        VkBuffer buffer = VK_NULL_HANDLE;
        VkDeviceMemory memory = VK_NULL_HANDLE;
        VkDescriptorBufferInfo bufferInfo = {};
    };

    const Shader& m_shader;                                 ///< Shader data and paths
    VulkanData& m_info;                                     ///< Data for the Vulkan Engine
    std::string m_vertexText;                               ///< Text for the vertex shader
    std::string m_fragmentText;                             ///< Text for the fragment shader
    std::string m_vertexAsm;                                ///< Assembly for the vertex shader
    std::string m_fragmentAsm;                              ///< Assembly for the fragment shader
    VkPipeline m_pipeline;                                  ///< Pipeline which describes the shader stages
    std::vector<VkPipelineShaderStageCreateInfo> m_stages;  ///< Vertex and fragment shaders
    UniformBuffer m_scene;                                  ///< Uniform buffer for the scene
    SceneData m_sceneData;                                  ///< TODO: Make this generic
    VkPipelineLayout m_pipelineLayout;                      ///< Specifies types of resources to be accessed
    VkDescriptorSetLayout m_descLayout;                     ///< Specifies types of resources to be accessed
    VkDescriptorSet m_descSet;                              ///< Specifies actual buffer/image resources to be bound
    VkDescriptorPool m_descPool;                            ///< Used to allocate descripter sets
};