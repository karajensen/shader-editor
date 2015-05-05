////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - shader.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <vector>
#include <algorithm>

/**
* Shader used to render a mesh
*/
class Shader
{
public:

    /**
    * All shader components
    */
    enum Component
    {
        NONE = 0,
        FLAT = 1,
        BUMP = 2,
        SPECULAR = 4,
        CAUSTICS = 8
    };

    /**
    * Constructor
    */
    Shader() = default;

    /**
    * Constructor
    * @param name The name of the shader
    * @param components The components this shader supports
    */
    Shader(const std::string& name, 
           const std::string& path,
           unsigned int components);

    /**
    * Gets a text description of the shader component
    * @param component The component to query for text
    * @return The text description of the component
    */
    static std::string ComponentAsString(unsigned int component);

    /**
    * Gets the component from a text description
    * @param component The string description
    * @return the component ID
    */
    static Component StringAsComponent(const std::string& component);

    /**
    * Determines whether the shader has the component
    * @param component The component to query for text
    * @return whether The shader has the component
    */
    bool HasComponent(Component component) const;

    /**
    * @return the components of the shader
    */
    unsigned int GetComponents() const;

    /**
    * @return The name of the shader
    */
    const std::string& Name() const;

    /**
    * @param file The filename of the GLSL vertex shader
    */
    void GLSLVertexFile(const std::string& file);

    /**
    * @param file The filename of the GLSL fragment shader
    */
    void GLSLFragmentFile(const std::string& file);

    /**
    * @param file The filename of the HLSL shader
    */
    void HLSLShaderFile(const std::string& file);

    /**
    * @return The filename of the GLSL vertex shader
    */
    const std::string& GLSLVertexFile() const;

    /**
    * @return The filename of the GLSL fragment shader
    */
    const std::string& GLSLFragmentFile() const;

    /**
    * @return The filename of the HLSL shader
    */
    const std::string& HLSLShaderFile() const;

private:

    const unsigned int m_components;     ///< Sections that make up this shader
    int m_index = -1;                    ///< Unique index of the shader
    std::string m_name;                  ///< name of the shader
    std::string m_glslVertexFile;        ///< filename of the glsl shader
    std::string m_glslFragmentFile;      ///< filename of the glsl shader
    std::string m_hlslShaderFile;        ///< filename of the hlsl shader
};