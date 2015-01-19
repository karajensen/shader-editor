////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - openglshader.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "openglcommon.h"
#include <unordered_map>

struct Shader;

/**
* Holds information for an opengl shader
*/
class GlShader : boost::noncopyable
{
public:

    /**
    * Constructor
    * @param shader The shader data to create
    */
    GlShader(const Shader& shader);

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
    * @param name Name of the uniform to send. This must match on the shader to be successful
    * @param value The pointer to the float array to send
    * @param count The number of floats to send
    */
    void SendUniformFloat(const std::string& name, const float* value, int count);

    /**
    * Updates the cached array scratch buffer
    * @param name Name of the uniform to send. This must match on the shader to be successful
    * @param value The pointer to the float array to send
    * @param count The number of floats to send
    * @param offset The index offset into the array
    */
    void UpdateUniformArray(const std::string& name, const float* value, int count, int offset);

    /**
    * Sends the array buffers to the shader if they have been updated
    */
    void SendUniformArrays();

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

    /**
    * Sends a texture to the shader
    * @param slot The slot to put the texture in
    * @param id The unique id for the opengl texture
    * @param cubemap Whether this texture is a cubemap
    * @param multisample Whether this texture is to be multisampled
    */
    void SendTexture(int slot, GLuint id, bool cubemap, bool multisample);

    /**
    * Clears the current texture set
    * @param slot The slot to put the texture in
    * @param cubemap Whether this texture is a cubemap
    * @param multisample Whether this texture is to be multisampled
    */
    void ClearTexture(int slot, bool cubemap, bool multisample);

    /**
    * @return the text for the shader
    */
    std::string GetText() const;

    /**
    * @return the assembly for the shader
    */
    std::string GetAssembly();

    /**
    * @return the the name of the shader
    */
    const std::string& GetName() const;

    /**
    * Sends the lights to the shader
    */
    void SendLights(const std::vector<Light>& lights);

private:

    /**
    * Generates the assembly instructions for the shader if needed
    * @return Error message if failed or empty if succeeded
    */
    std::string GenerateAssembly();

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
    * Determines the input/output attributes and binds them
    * @return Error message if failed or empty if succeeded
    */
    std::string BindShaderAttributes();

    /**
    * Determines the input vertex attributes and binds them
    * @return Error message if failed or empty if succeeded
    */
    std::string BindVertexAttributes();

    /**
    * Determines the shader non-attribute uniform variables and samplers
    * @return Error message if failed or empty if succeeded
    */
    std::string FindShaderUniforms();

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
    * Sends the float to the shader
    * @param name Name of the uniform to send. This must match on the shader to be successful
    * @param value The pointer to the float array to send
    * @param location Unique location within the shader
    * @param size The number of elements in the array (1 if not an array)
    * @param type Whether a float, vec2, vec3, vec4 
    */
    void SendUniformFloat(const std::string& name, const float* value, 
        int location, int size, GLenum type);

    /**
    * Information for each vertex input attribute
    */
    struct AttributeData
    {
        int components = 0;  ///< Number of float components in the type
        int location = 0;    ///< The index location of the attribute
        std::string name;    ///< The name of the attribute
    };

    /**
    * Information for a non-attribute shader uniform
    * @note scratch is not allocated for matrices
    */
    struct UniformData
    {
        GLenum type = 0;            ///< Whether a float, vec2, vec3, vec4
        int location = 0;           ///< Unique location within the shader
        int size = 0;               ///< The number of elements in the array (1 if not an array)
        std::vector<float> scratch; ///< Buffer allocated with the total floats used
        bool updated = false;       ///< Whether the scratch buffer has been updated
    };

    typedef std::unordered_map<std::string, UniformData> UniformMap;

    const Shader& m_shader;                   ///< Shader data and paths
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
    GLint m_program = NO_INDEX;               ///< Shader program
    GLint m_vs = NO_INDEX;                    ///< GLSL Vertex Shader
    GLint m_fs = NO_INDEX;                    ///< GLSL Fragment Shader
    GLsizei m_stride = 0;                     ///< Stride required for vertex attributes
};