////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directshader.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "directxcommon.h"
#include "directx/include/D3D11Shader.h"
#include <unordered_map>

struct Shader;

/**
* Holds information for a directx shader
*/
class DxShader : boost::noncopyable
{
public:

    /**
    * Constructor
    * @param shader The shader data to create
    */
    DxShader(const Shader& shader);

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
    * @param offset The amount of floats to offset when writing
    */
    void UpdateConstantFloat(const std::string& name, 
        const float* value, int size, int offset = NO_INDEX);

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
    * Sets the internal debug names for the shader objects
    */
    void SetDebugNames();                  

    /**
    * Information for a shader (non-attribute uniform) constant
    */
    struct ConstantData
    {
        int size = 0;    ///< How many floats are apart of this data
        int index = 0;   ///< Offset from beginning of scratch buffer
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

    const Shader& m_shader;                           ///< Shader data and paths
    D3D11_SHADER_DESC m_vertexDesc;                   ///< Internal description of the vertex shader
    D3D11_SHADER_DESC m_pixelDesc;                    ///< Internal description of the pixel shader
    std::string m_filepath;                           ///< Path to the shader file
    std::string m_asmpath;                            ///< Path to the generated assembly file
    std::string m_sharedText;                         ///< Text to the shared shader components
    std::string m_vertexText;                         ///< Text for the vertex shader
    std::string m_pixelText;                          ///< Text for the pixel shader
    std::string m_vertexAsm;                          ///< Assembly for the vertex shader
    std::string m_pixelAsm;                           ///< Assembly for the pixel shader
    ID3D11InputLayout* m_layout = nullptr;            ///< Shader input layout
    ID3D11VertexShader* m_vs = nullptr;               ///< Vertex shader interface
    ID3D11PixelShader* m_ps = nullptr;                ///< Pixel shader interface
    ID3D11ShaderReflection* m_vsReflection = nullptr; ///< Interface for obtaining vertex shader details
    ID3D11ShaderReflection* m_psReflection = nullptr; ///< Interface for obtaining pixel shader details
    ID3D10Blob* m_vsBlob = nullptr;                   ///< Vertex shader data
    ID3D10Blob* m_psBlob = nullptr;                   ///< Pixel shader data
    ID3D11SamplerState* m_samplerState = nullptr;     ///< Texture Sampler state
    int m_textureSlots = 0;                           ///< Number of textures allowed for this mesh

    std::vector<std::unique_ptr<ConstantBuffer>> m_cbuffers;  ///< Constant buffers for the shader
};  