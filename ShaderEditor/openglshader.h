////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - openglshader.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "openglcommon.h"
#include <unordered_map>

/**
* Holds information for an opengl shader
*/
class GlShader
{
public:

    /**
    * Constructor
    * @param index The unique index of the shader
    * @param vs The filepath for the vertex shader
    * @param fs The filepath for the fragment shader
    */
    GlShader(int index, const std::string& vs, const std::string& fs);

    /**
    * Constructor
    * @param vs The filepath for the vertex shader
    * @param fs The filepath for the fragment shader
    * @param vsAsm The filepath to generate the vertex assembly file
    * @param fsAsm The filepath to generate the fragment assembly file
    */
    GlShader(const std::string& vs, const std::string& fs,
        const std::string& vsAsm, const std::string& fsAsm);

    /**
    * Destructor
    */
    ~GlShader();

    /**
    * Releases the shader object
    */
    void Release();

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
    * Sends a matrix to the shader
    * @param name Name of the matrix to send. This must match on the shader to be successful
    * @param matrix The matrix to send
    */
    void SendUniformMatrix(const std::string& name, const glm::mat4& matrix);

    /**
    * Sends the float to the shader
    * @param name Name of the float to send. This must match on the shader to be successful
    * @param value The pointer to the float array to send
    * @param size The number of floats to send
    */
    void SendUniformFloat(const std::string& name, const float* value, int size);

    /**
    * @return the unique index of the shader
    */
    int GetIndex() const;

    /**
    * Enables the vertex shader 'in' attributes for the shader
    * This is required after the shader is active and when a mesh buffer is bound
    */
    void EnableAttributes();

    /**
    * Determines whether the texture slot is available for the texture
    * @param slot The slot to query
    * @return whether the shader allows the slot to be bound to
    */
    bool HasTextureSlot(int slot);

private:

    /**
    * Updates the cached text for the shader.
    * @note done as the last step so the visible shader text in the 
    * GUI is not wiped or corrupted if the shader compilation fails.
    * @param vText Text for the vertex shader
    * @param fText Text for the fragment shader
    * @param vAsm Text for the vertex shader assembly
    * @param pAsm Text for the fragment shader assembly
    */
    void UpdateShaderText(const std::string& vText, const std::string& fText,
        const std::string& vAsm, const std::string& fAsm);

    /**
    * Generates the assembly instructions for the shader
    * @param vertexAsm The text container to fill the vertex assembly with
    * @param fragmentAsm The text container to fill the fragment assembly with
    * @return Error message if failed or empty if succeeded
    */
    std::string GenerateAssembly(std::string& vertexAsm, std::string& fragmentAsm);

    /**
    * Retrieves the generated assembly text from a file
    * @param path The path of the assembly text file
    * @param text The container to fill with the text
    * @return Error message if failed or empty if succeeded
    */
    std::string LoadAssemblyText(const std::string& path, std::string& text);

    /**
    * Loads the vertex and pixel shaders into strings
    * @param path The path of the shader text file
    * @param text The container to fill with the text
    * @return Error message if failed or empty if succeeded
    */
    std::string LoadShaderText(const std::string& path, std::string& text);

    /**
    * Determines the vertex shader input attributes and caches them
    * @param text Vertex shader text split into word components
    * @return Error message if failed or empty if succeeded
    * @note should only be called on the vertex shader
    */
    std::string BindVertexAttributes(const std::vector<std::string>& text);

    /**
    * Determines the shader non-attribute uniform variables
    * @note OpenGL will sometimes remove uniforms when compiling 
    * if they are not used within the body of the shader.
    * @param text Vertex or fragment text split into word components
    * @return Error message if failed or empty if succeeded
    */
    std::string FindShaderUniforms(const std::vector<std::string>& text);

    /**
    * Generates the shader for the engine
    * @param index The unique index of the shader (vs or fs) to compile
    * @param text Vertex or fragment shader text
    * @return Error message if failed or empty if succeeded
    */
    std::string CompileShader(GLint index, const std::string& text);

    /**
    * Links together all shaders within the shader program
    * @return Error message if failed or empty if succeeded
    */
    std::string LinkShaderProgram();

    /**
    * Validates the uniform that is requesting to be sent matches the cached type
    * @param expectedType Current type of uniform wanting to send
    * @param actualType Actual type of uniform in the shader
    * @param name Name of the uniform to send
    */
    bool CanSendUniform(const std::string& expectedType, 
        const std::string& actualType, const std::string& name) const;

    /**
    * Information for each vertex input attribute
    */
    struct AttributeData
    {
        int components;      ///< Number of float components in the type
        int location;        ///< The index location of the attribute
        std::string name;    ///< The name of the attribute
    };

    /**
    * Information for a non-attribute shader uniform
    */
    struct UniformData
    {
        std::string type;    ///< Type of data stored in the location
        int location;        ///< Unique location within the shader
    };

    typedef std::unordered_map<std::string, UniformData> UniformMap;

    UniformMap m_uniforms;                    ///< Vertex and fragment non-attribute uniform data
    std::vector<int> m_samplers;              ///< Fragment shader sampler locations
    std::vector<AttributeData> m_attributes;  ///< Vertex shader input attributes
    std::string m_vsFilepath;                 ///< Path to the vertex shader file
    std::string m_fsFilepath;                 ///< Path to the fragment shader file
    std::string m_vaFilepath;                 ///< Path to the vertex assembly file
    std::string m_faFilepath;                 ///< Path to the fragment assembly file
    std::string m_vertexText;                 ///< Text for the vertex shader
    std::string m_fragmentText;               ///< Text for the fragment shader
    std::string m_vertexAsm;                  ///< Assembly for the vertex shader
    std::string m_fragmentAsm;                ///< Assembly for the fragment shader
    GLint m_program;                          ///< Shader program
    GLint m_vs;                               ///< GLSL Vertex Shader
    GLint m_fs;                               ///< GLSL Fragment Shader
    GLsizei m_stride;                         ///< Stride required for vertex attributes
    int m_index;                              ///< Unique index of the shader
};