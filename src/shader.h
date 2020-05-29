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
        None = 0,
        Flat = 1,
        Bump = 2,
        Specular = 4,
        Caustics = 8
    };

    /**
    * Constructor
    */
    Shader() = default;

    /**
    * Constructor
    * @param name The name of the shader
    * @param components The components this shader supports
    * @param fromFragments Whether this shader is generated from the base shader
    */
    Shader(const std::string& name, 
           unsigned int components,
           bool fromFragments);

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
    * @return whether this shader is to be generated from fragments
    */
    bool GenerateFromFragments() const;

    /**
    * @return the components of the shader
    */
    unsigned int GetComponents() const;

    /**
    * @return The name of the shader
    */
    const std::string& Name() const;

    /**
    * @return the location of the base shader to generate from
    */
    std::string GLSLVertexBase() const;

    /**
    * @return the location of the base shader to generate from
    */
    std::string GLSLFragmentBase() const;

    /**
    * @return the location of the base shader to generate from
    */
    std::string HLSLShaderBase() const;

    /**
    * @return The full path of the generated GLSL vertex shader
    */
    std::string GLSLVertexFile() const;

    /**
    * @return The full path of the generated GLSL fragment shader
    */
    std::string GLSLFragmentFile() const;

    /**
    * @return The full path of the generated HLSL shader
    */
    std::string HLSLShaderFile() const;

    /**
    * @return The full path of the GLSL vertex assembly
    */
    std::string GLSLVertexAsmFile() const;

    /**
    * @return The full path of the GLSL fragment assembly
    */
    std::string GLSLFragmentAsmFile() const;

    /**
    * @return The full path of the HLSL assembly
    */
    std::string HLSLShaderAsmFile() const;

private:

    const unsigned int m_components;     ///< Sections that make up this shader
    int m_index = -1;                    ///< Unique index of the shader
    std::string m_name;                  ///< name of the shader
    bool m_fromFragments = false;        ///< whether this shader is generated from the base shader
};