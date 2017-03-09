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
    * Binds the shader for rendering
    */
    void Bind(VkCommandBuffer cmd);

    /**
    * @return the text for the shader
    */
    std::string GetText() const;

    /**
    * @return the assembly for the shader
    */
    std::string GetAssembly();

    /**
    * Sends the uniform buffer to the shader
    */
    void SendUniformBuffer();

    /**
    * TODO: Make this generic
    */
    void UpdateWorldMatrix(const glm::mat4& world);

private:

    /**
    * Parse a GLSL shader into SPRIV
    */
    std::string Parse(const VkShaderStageFlagBits shader_type,  
                      const char* pshader, 
                      std::vector<unsigned int>& spirv);

    //-----------------------------------------
    // TODO: Make this generic for scene
    struct SceneData
    {
        glm::mat4 viewProj;
    };
    struct ObjData
    {
        glm::mat4 world;
    };
    ObjData m_objData;
    SceneData m_sceneData;
    //-----------------------------------------

    void ReleaseShader();
    void Reset();
    bool InitUniformBuffer();
    bool InitPipeline();

    const Shader& m_shader;        ///< Shader data and paths
    VulkanData& m_info;            ///< Data for the Vulkan Engine
    std::string m_vertexText;      ///< Text for the vertex shader
    std::string m_fragmentText;    ///< Text for the fragment shader
    std::string m_vertexAsm;       ///< Assembly for the vertex shader
    std::string m_fragmentAsm;     ///< Assembly for the fragment shader
    
    struct UniformBuffer
    {
        VkBuffer buffer = VK_NULL_HANDLE;
        VkDeviceMemory memory = VK_NULL_HANDLE;
        VkDescriptorBufferInfo bufferInfo = {};
    };

    UniformBuffer m_scene;
    std::vector<UniformBuffer> m_objs;

    VkPipeline m_pipeline;
    std::vector<VkPipelineShaderStageCreateInfo> m_stages;
};