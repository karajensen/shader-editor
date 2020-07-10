////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - opengl_shader.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "opengl_common.h"
#include <unordered_map>

class Shader;
class GlRenderTarget;

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
    * Enables the vertex shader 'in' attributes for the shader
    * This is required after the shader is active and when a mesh buffer is bound
    */
    void EnableAttributes();

    /**
    * Sends a texture to the shader
    * @param sampler Name of the shader texture sampler to use
    * @param id The unique id for the opengl texture
    * @param cubemap Whether this texture is a cubemap
    */
    void SendTexture(const std::string& sampler, GLuint id, bool cubemap);

    /**
    * Sends the render target texture to the shader
    * @param sampler Name of the shader texture sampler to use
    * @param target The render target to send
    * @param ID the id of the target texture to send
    */
    void SendTexture(const std::string& sampler, const GlRenderTarget& target, int ID);

    /**
    * Clears the render target texture from the shader
    * @param sampler Name of the shader texture sampler to use
    * @param target The render target to clear
    */
    void ClearTexture(const std::string& sampler, const GlRenderTarget& target);

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
    * @return the header version keyword for GLSL shaders
    */
    static std::string GetShaderHeader();

private:

    /**
    * Sends a texture to the shader
    * @param sampler Name of the shader texture sampler to use
    * @param id The unique id for the opengl texture
    * @param multisample Whether this texture is to be multisampled
    * @param cubemap Whether this texture is a cubemap
    */
    void SendTexture(const std::string& sampler, GLuint id, bool multisample, bool cubemap);

    /**
    * Clears the current texture set
    * @param sampler Name of the shader texture sampler to use
    * @param multisample Whether this texture is to be multisampled
    * @param cubemap Whether this texture is a cubemap
    */
    void ClearTexture(const std::string& sampler, bool multisample, bool cubemap);

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
    void SendUniformFloat(const std::string& name, 
                          const float* value, 
                          int location, 
                          int size, 
                          GLenum type);

    /**
    * Determines the unique ID of the texture slot
    * @param slot The slot in the shader the texture will fill
    * @return the unique ID of the texture
    */
    static unsigned int GetTexture(int slot);

    /**
    * Determines the amount of float components from the OpenGL type
    * @param type The type of object (ie. float, vec2)
    * @return the number of floats in the object
    */
    static int GetComponents(GLenum type);

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

    /**
    * Information for a sampler
    */
    struct SamplerData
    {
        int slot = 0;                ///< Order of usage in shader
        int allocated = -1;            ///< Currently allocated texture to this slot
        int location = 0;            ///< Unique location within the shader
        GLenum type = 0;            ///< Whether a texture, cubemap or ms
    };

    typedef std::unordered_map<std::string, UniformData> UniformMap;
    typedef std::unordered_map<std::string, SamplerData> SamplerMap;

private:

    const Shader& m_shader;                   ///< Shader data and paths
    UniformMap m_uniforms;                    ///< Vertex and fragment non-attribute uniform data
    SamplerMap m_samplers;                    ///< Fragment shader sampler locations
    std::vector<AttributeData> m_attributes;  ///< Vertex shader input attributes
    std::string m_vsFilepath;                 ///< Path to the vertex shader file
    std::string m_fsFilepath;                 ///< Path to the fragment shader file
    std::string m_vaFilepath;                 ///< Path to the vertex assembly file
    std::string m_faFilepath;                 ///< Path to the fragment assembly file
    std::string m_vertexText;                 ///< Text for the vertex shader
    std::string m_fragmentText;               ///< Text for the fragment shader
    std::string m_vertexAsm;                  ///< Assembly for the vertex shader
    std::string m_fragmentAsm;                ///< Assembly for the fragment shader
    GLint m_program = -1;                     ///< Shader program
    GLint m_vs = -1;                          ///< GLSL Vertex Shader
    GLint m_fs = -1;                          ///< GLSL Fragment Shader
    GLsizei m_stride = 0;                     ///< Stride required for vertex attributes
};
