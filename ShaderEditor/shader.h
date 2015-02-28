////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - shader.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include "renderdata.h"

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
        FLAT,
        BUMP,
        SPECULAR,
        GLOW,
        MAX_COMPONENTS
    };

    /**
    * Vertex Shader Attribute ordering
    */
    enum VertexAttribute
    {
        POSITION,
        UVS,
        NORMAL,
        COLOUR,
        TANGENT,
        BITANGENT
    };

    /**
    * Constructor
    */
    Shader() = default;

    /**
    * Constructor
    * @param name The name of the shader
    * @param index The unique ID of the shader
    */
    Shader(const std::string& name, int index);

    /**
    * Sets the data for the shader
    * @param name The name of the shader
    * @param index The unique ID of the shader
    */
    void Set(const std::string& name, int index);

    /**
    * Gets a text description of the shader component
    * @param component The component to query for text
    * @return The text description of the component
    */
    static std::string ComponentAsString(unsigned int component);

    /**
    * Determines whether the shader has the component
    * @param component The component to query for text
    * @return whether The shader has the component
    */
    bool HasComponent(unsigned int component) const;

    /**
    * @param component The component to add to the shader
    */
    void AddComponent(Component component);

    /**
    * @return The name of the shader
    */
    const std::string& Name() const;

    /**
    * @return Unique index of the shader
    */
    int ID() const;

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

    int m_index = -1;                    ///< Unique index of the shader
    std::string m_name;                  ///< name of the shader
    std::string m_glslVertexFile;        ///< filename of the glsl shader
    std::string m_glslFragmentFile;      ///< filename of the glsl shader
    std::string m_hlslShaderFile;        ///< filename of the hlsl shader
    std::vector<Component> m_components; ///< Sections that make up this shader
};