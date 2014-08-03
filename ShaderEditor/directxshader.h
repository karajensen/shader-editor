////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directshader.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "directxcommon.h"
#include "directx/include/D3D11Shader.h"
#include <unordered_map>

/**
* Holds information for a directx shader
*/
class DxShader : boost::noncopyable
{
public:

    /**
    * Constructor
    * @param index The unique index of the shader
    * @param name The name of the shader
    * @param filepath The path for the shader
    */
    DxShader(int index, const std::string& name, const std::string& filepath);

    /**
    * Destructor
    */
    ~DxShader();

    /**
    * Releases the shader object
    */
    void Release();

    /**
    * Generates the shader for the engine
    * @param device The DirectX device interface
    * @return Error message if failed or empty if succeeded
    */
    std::string CompileShader(ID3D11Device* device);

    /**
    * Sets the shader as activated for rendering
    * @param context Direct3D device context
    */
    void SetActive(ID3D11DeviceContext* context);

    /**
    * Sends a matrix to the shader
    * @param name Name of the matrix to send. This must match on the shader to be successful
    * @param matrix The matrix to send
    */
    void UpdateConstantMatrix(const std::string& name, const D3DXMATRIX& matrix);

    /**
    * Sends a float or float array to the shader
    * @param name Name of the float to send. This must match on the shader to be successful
    * @param value Pointer to the float array to send
    * @param size The size of the float array
    */
    void UpdateConstantFloat(const std::string& name, const float* value, int size);

    /**
    * Bulk-sends all constant data saved in the constant scratch buffer to the shader
    * @note Done this way as currently not possible to partially update a cbuffer
    * @note Will only send buffers updated last tick
    * @param context Direct3D device context
    */
    void SendConstants(ID3D11DeviceContext* context);

    /**
    * @return the unique index of the shader
    */
    int GetIndex() const;

    /**
    * Determines whether the texture slot is available for the texture
    * @param slot The slot to query
    * @return whether the shader allows the slot to be bound to
    */
    bool HasTextureSlot(int slot);

    /**
    * Gets the text for the shader
    * @return the text for the shader
    */
    std::string GetText() const;

    /**
    * Gets the assembly for the shader
    * @return the assembly for the shader
    */
    std::string GetAssembly();

private:

    /**
    * Creates the vertex and pixel shaders
    * @param device The DirectX device interface interface
    * @return Error message if failed or empty if succeeded
    */
    std::string CreateShaders(ID3D11Device* device);

    /**
    * Creates DirectX reflection to obtain information about the compiled shader
    * @param shader The shader blob object
    * @param reflection The reflection interface to obtain
    * @param desc The description to fill in
    * @return Error message if failed or empty if succeeded
    */
    std::string FindShaderDescription(ID3D10Blob* shader,
        ID3D11ShaderReflection** reflection, D3D11_SHADER_DESC& desc); 

    /**
    * Compiles the shader internally in DirectX
    * @param shader The shader blob to compile into
    * @param isVertex Whether this shader is the vertex or pixel shader
    * @return Error message if failed or empty if succeeded
    */
    std::string CompileShader(ID3D10Blob** shader, bool isVertex);

    /**
    * Generates the assembly instructions for the shader if needed
    * @return Error message if failed or empty if succeeded
    */
    std::string GenerateAssembly();

    /**
    * Loads the vertex and pixel shaders into cached strings
    * @return Error message if failed or empty if succeeded
    */
    std::string LoadShaderText();

    /**
    * Determines the vertex shader input attributes and caches them
    * @param device The DirectX device interface
    * @return Error message if failed or empty if succeeded
    */
    std::string BindVertexAttributes(ID3D11Device* device);

    /**
    * Gets the format of the vertex shader input attribute
    * @param description The description of the input attribute
    * @return The format of the input attribute
    */
    DXGI_FORMAT GetAttributeFormat(D3D11_SIGNATURE_PARAMETER_DESC& description) const;

    /**
    * Gets how many bytes the attribute uses from the format
    * @param description The description of the input attribute
    * @return the number of bytes used
    */
    int GetAttributeBytes(D3D11_SIGNATURE_PARAMETER_DESC& description) const;

    /**
    * Gets how many components are used by the vertex shader input attribute
    * @param description The description of the input attribute
    * @return how many components 
    */
    int GetAttributeCompononts(D3D11_SIGNATURE_PARAMETER_DESC& description) const;

    /**
    * Creates the state of the sampler
    * @param device The DirectX device interface
    * @return Error message if failed or empty if succeeded
    */
    std::string CreateSamplerState(ID3D11Device* device);

    /**
    * Generates the constant buffer which holds all non-attribute uniforms
    * @param device The DirectX device interface
    * @param index The index that the constant buffer is stored in for the shader
    * @param isVertexBuffer Whether this buffer is read by the vertex or pixel shader
    * @return Error message if failed or empty if succeeded
    */
    std::string CreateConstantBuffer(ID3D11Device* device, 
        int index, bool isVertexBuffer);

    /**
    * Generates the constant buffers which holds all non-attribute uniforms
    * @param device The DirectX device interface
    * @param isVertex Whether currently creating for the vertex or pixel shader
    * @return Error message if failed or empty if succeeded
    */
    std::string CreateConstantBuffers(ID3D11Device* device, bool isVertexShader);

    /**
    * Validates the non-attribute constant that is requesting to be sent
    * @param expectedType Current type of constant wanting to send
    * @param actualType Actual type of constant in the shader
    * @param name Name of the constant
    */
    bool CanSendConstant(
        const std::string& expectedType, 
        const std::string& actualType, 
        const std::string& name) const;

    /**
    * Sets the internal debug names for the shader objects
    */
    void SetDebugNames();                  

    /**
    * Information for a shader (non-attribute uniform) constant
    */
    struct ConstantData
    {
        std::string type;    ///< Type of data stored in the location
        int index;           ///< Offset from beginning of scratch buffer
    };

    /**
    * Information for a constant buffer read from either the vertex or pixel shader
    */
    struct ConstantBuffer : boost::noncopyable
    {
        /**
        * Constructor
        */
        ConstantBuffer();

        /**
        * Destructor
        */
        ~ConstantBuffer();

        typedef std::unordered_map<std::string, ConstantData> ConstantMap;

        std::string name;            ///< Name of the constant buffer
        ConstantMap constants;       ///< Shader constant variables for the buffer
        std::vector<float> scratch;  ///< Holds temporary constant values
        ID3D11Buffer* buffer;        ///< Buffer object
        int startSlot;               ///< The register number of the buffer
        bool isVertexBuffer;         ///< Whether this buffer is used by the vertex or pixel shader 
        bool updated;                ///< Whether this buffer was updated last tick
    };

    D3D11_SHADER_DESC m_vertexDesc;            ///< Internal description of the vertex shader
    D3D11_SHADER_DESC m_pixelDesc;             ///< Internal description of the pixel shader
    std::string m_filepath;                    ///< Path to the shader file
    std::string m_name;                        ///< Name of the shader
    std::string m_asmpath;                     ///< Path to the generated assembly file
    std::string m_sharedText;                  ///< Text to the shared shader components
    std::string m_vertexText;                  ///< Text for the vertex shader
    std::string m_pixelText;                   ///< Text for the pixel shader
    std::string m_vertexAsm;                   ///< Assembly for the vertex shader
    std::string m_pixelAsm;                    ///< Assembly for the pixel shader
    ID3D11InputLayout* m_layout;               ///< Shader input layout
    ID3D11VertexShader* m_vs;                  ///< Vertex shader interface
    ID3D11PixelShader* m_ps;                   ///< Pixel shader interface
    ID3D11ShaderReflection* m_vsReflection;    ///< Interface for obtaining vertex shader details
    ID3D11ShaderReflection* m_psReflection;    ///< Interface for obtaining pixel shader details
    ID3D10Blob* m_vsBlob;                      ///< Vertex shader data
    ID3D10Blob* m_psBlob;                      ///< Pixel shader data
    ID3D11SamplerState* m_samplerState;        ///< Texture Sampler state
    int m_index;                               ///< Unique index of the shader
    int m_textureSlots;                        ///< Number of textures allowed for this mesh

    std::vector<std::unique_ptr<ConstantBuffer>> m_cbuffers;  ///< Constant buffers for the shader
};  