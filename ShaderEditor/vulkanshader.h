////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkanshader.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "vulkan.h"
#include <string>

struct VulkanData;

class VkShader
{
public:

    /**
    * Constructor
    * @param info Data for the Vulkan Engine
    */
    VkShader(VulkanData& info);

    /**
    * Destructor
    */
    ~VkShader();

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
    * Initialises Vulkan shader framework
    */
    static bool InitFramework();

    /**
    * Releases Vulkan shader framework
    */
    static void ReleaseFramework();

private:

    VulkanData& m_info;             ///< data for the Vulkan Engine
    std::string m_vertexText;       ///< Text for the vertex shader
    std::string m_fragmentText;     ///< Text for the fragment shader

    std::string m_tempName = "SHADER";

};