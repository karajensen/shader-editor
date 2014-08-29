////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - openglshader.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "openglcommon.h"
#include <unordered_map>

/**
* Holds information for an opengl shader
*/
class GlShader : boost::noncopyable
{
public:

    /**
    * Constructor
    * @param name The name of the shader
    * @param vs The filepath for the vertex shader
    * @param fs The filepath for the fragment shader
    * @param index The unique index of the shader
    */
    GlShader(const std::string& name, const std::string& vs, 
        const std::string& fs, int index = NO_INDEX);

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

    /**
    * Sends a texture to the shader
    * @param slot The slot to put the texture in
    * @param id The unique id for the opengl texture
    * @param multisample Whether this texture is to be multisampled
    */
    void SendTexture(int slot, GLuint id, bool multisample = false);

    /**
    * Clears the current texture set
    * @param slot The slot to put the texture in
    * @param multisample Whether this texture is to be multisampled
    */
    void ClearTexture(int slot, bool multisample = false);

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
    * Validates the uniform that is requesting to be sent matches the cached type
    * @param expectedType Current type of uniform wanting to send
    * @param actualType Actual type of uniform in the shader
    * @param name Name of the uniform to send
    */
    bool CanSendUniform(GLenum expectedType,
        GLenum actualType, const std::string& name) const;

    /**
    * Finds the float type from the number of components used
    * @param components The number of components used
    * @return the type of float structure in GLSL
    */
    GLenum GetTypeFromComponents(int components) const;

    /**
    * Finds the number of components used from the float type
    * @param type The type of float structure in GLSL
    * @return the number of components used
    */
    int GetComponentsFromType(GLenum type) const;

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
        GLenum type;    ///< Type of data stored in the location
        int location;   ///< Unique location within the shader
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
    std::string m_name;                       ///< The name of the shader
    GLint m_program = NO_INDEX;               ///< Shader program
    GLint m_vs = NO_INDEX;                    ///< GLSL Vertex Shader
    GLint m_fs = NO_INDEX;                    ///< GLSL Fragment Shader
    GLsizei m_stride = 0;                     ///< Stride required for vertex attributes
    int m_index;                              ///< Unique index of the shader
};