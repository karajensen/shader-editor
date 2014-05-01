////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - openglshader.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "openglcommon.h"
#include <unordered_map>

/**
* Holds information for an individual opengl shader
*/
class GlShader
{
public:

    /**
    * Constructor
    * @param the unique index of the shader
    * @param vs The filepath for the vertex shader
    * @param fs The filepath for the fragment shader
    */
    GlShader(int index, const std::string& vs, const std::string& fs);

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
    * @param scratchVS A vertex shader to test changes on
    * @param scratchFS A fragment shader to test changes on
    * @return error message if failed or empty if succeeded
    */
    std::string CompileShader(GLint scratchVS, GLint scratchFS);

    /**
    * Sets the shader as activated for rendering
    */
    void SetAsActive();

    /**
    * Sends the matrix to the shader
    * @param name Name of the matrix to send
    * @param matrix The matrix to send
    */
    void SendUniformMatrix(const std::string& name, const glm::mat4& matrix);

    /**
    * Sends the float to the shader
    * @param name Name of the float to send
    * @param value The float to send
    */
    void SendUniformFloat(const std::string& name, float value);

    /**
    * @return the unique index of the shader
    */
    int GetIndex() const;

    /**
    * Enables the vertex shader 'in' attributes
    */
    void EnableAttributes();

private:

    /**
    * Determines the vertex shader 'in' attributes and caches them
    * @param vText Vertex shader text split into components
    * @return error message if failed or empty if succeeded
    */
    std::string BindVertexAttributes(const std::vector<std::string>& vText);

    /**
    * Determines the vertex and fragment shader uniform variables
    * @param text Shader text split into components
    * @return error message if failed or empty if succeeded
    */
    std::string FindShaderUniforms(const std::vector<std::string>& text);

    /**
    * Generates the shader for the engine
    * @param index The unique index of the shader (vs or fs) to compile
    * @param source The text to compile
    * @param size The amount of text to compile
    * @return error message if failed or empty if succeeded
    */
    std::string CompileShader(GLint index, const char* source, int size);

    /**
    * Loads the shader from the path
    * @param path The absolute path to the shader file
    * @param size The size of the file
    * @param text The text within the file
    * @return error message if failed or empty if succeeded
    */
    std::string LoadShaderFile(const std::string& path, int& size, std::string& text);

    /**
    * Links together all shaders within the shader program
    * @return error message if failed or empty if succeeded
    */
    std::string LinkShaderProgram();

    /**
    * Validates the uniform that is requesting to be sent
    * @param expectedType Type of uniform wanting to send
    * @param actualType Tyep of uniform being sent
    * @param name Name of the uniform
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
    * Information for a shader uniform
    */
    struct UniformData
    {
        std::string type;    ///< Type of data stored in the location
        int location;        ///< Unique location within the shader
    };

    typedef std::unordered_map<std::string, UniformData> UniformMap;

    UniformMap m_uniforms;                       ///< Vertex and fragment shader uniform data
    std::vector<AttributeData> m_attributes;     ///< Vertex shader attributes
    std::string m_vsFilepath;                    ///< Path to the vertex shader file
    std::string m_fsFilepath;                    ///< Path to the fragment shader file
	GLint m_program;                             ///< Shader program
	GLint m_vs;                                  ///< GLSL Vertex Shader
	GLint m_fs;                                  ///< GLSL Fragment Shader
    GLsizei m_stride;                            ///< Stride required for vertex attributes
    int m_index;                                 ///< Unique index of the shader
};