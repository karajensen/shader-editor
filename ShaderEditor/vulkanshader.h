////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkanshader.h
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

private:

    // TODO: Make this generic for scene
    struct UniformData
    {
        glm::mat4 mvp;
    };

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

    VkBuffer m_buffer;
    VkDeviceMemory m_memory;
    VkDescriptorBufferInfo m_bufferInfo;
    UniformData m_data;
    VkPipeline m_pipeline;
    std::vector<VkPipelineShaderStageCreateInfo> m_stages;
};