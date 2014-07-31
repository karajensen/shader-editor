////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directshader.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "directxcommon.h"
#include <unordered_map>

/**
* Holds information for a directx shader
*/
class DxShader
{
public:

    /**
    * Constructor
    * @param index The unique index of the shader
    * @param filepath The path for the shader
    */
    DxShader(int index, const std::string& filepath);

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
    * @param device The directX device
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
    std::string GetAssembly() const;

private:

    /**
    * Updates the cached text for the shader.
    * @note done as the last step so the visible shader text in the 
    * GUI is not wiped or corrupted if the shader compilation fails.
    * @param vText Text for the vertex shader
    * @param pText Text for the pixel shader
    * @param shared Text for the shader shared components
    * @param vAsm Text for the vertex shader assembly
    * @param pAsm Text for the pixel shader assembly
    */
    void UpdateShaderText(const std::string& vText, const std::string& pText,
        const std::string& shared, const std::string& vAsm, const std::string& pAsm);

    /**
    * Generates the assembly instructions for the shader
    * @param vs The vertex shader to generate assembly for
    * @param ps The pixel shader to generate assembly for
    * @param vertexAsm The text container to fill the vertex assembly with
    * @param pixelAsm The text container to fill the fragment assembly with
    * @return Error message if failed or empty if succeeded
    */
    std::string GenerateAssembly(ID3D10Blob* vs, ID3D10Blob* ps,
        std::string& vertexAsm, std::string& pixelAsm);

    /**
    * Loads the vertex and pixel shaders into strings
    * @param vertexText Container to fill with the text for the vertex shader
    * @param fragmentText Container to fill with the text for the fragment shader
    * @param sharedText Container to fill with the text for shared components
    * @return Error message if failed or empty if succeeded
    */
    std::string LoadShaderText(std::string& vertexText, 
        std::string& pixelText, std::string& sharedText);

    /**
    * @return the vertex shader input attributes where the 
    * type, name and semantic are seperated as components
    */
    std::vector<std::string> GetVertexAttributes(const std::string& text) const;

    /**
    * Determines the vertex shader input attributes and caches them
    * @param device The directX device
    * @param vs The vertex shader object
    * @param text The text for the vertex shader
    * @return Error message if failed or empty if succeeded
    * @note should only be called on the vertex shader
    */
    std::string BindVertexAttributes(ID3D11Device* device, 
        ID3D10Blob* vs, const std::string& text);

    /**
    * Creates the state of the sampler
    * @param device The directX device
    * @param text The shared components of the shader
    * @return Error message if failed or empty if succeeded
    */
    std::string CreateSamplerState(ID3D11Device* device, const std::string& text);

    /**
    * Finds the constant buffer inside the given shader text
    * @param text The text of the shader
    * @param name The name of the constant buffer
    * @return The index in the shader text for the buffer or NO_INDEX if not found
    */
    int FindConstantBuffer(const std::string& text, const std::string& name) const;

    /**
    * Finds the register number of the constant buffer
    * @param text The text of the shader
    * @param index The index of the constant buffer within the shader text
    * @return the register number of the buffer
    */
    int FindConstantBufferRegister(const std::string& text, int index) const;

    /**
    * Finds all members of the constant buffer
    * @param text The text of the shader
    * @param index The index of the constant buffer within the shader text
    * @return a list of all members where the type preceeds the name as components
    */
    std::vector<std::string> GetConstants(const std::string& text, int index) const;

    /**
    * Generates the constant buffer which holds all non-attribute uniforms
    * @param device The directX device
    * @param text The text for the shared shader components
    * @param name The name of the buffer to create
    * @param isVertexBuffer Whether this buffer is read by the vertex or pixel shader
    * @return Error message if failed or empty if succeeded
    */
    std::string CreateConstantBuffer(
        ID3D11Device* device, const std::string& text,
        std::string name, bool isVertexBuffer);

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
    * Information for each vertex input attribute
    */
    struct AttributeData
    {
        int byteOffset;       ///< Offset between current and previous attribute
        std::string semantic; ///< The HLSL description of the attribute
        DXGI_FORMAT format;   ///< Layout of the attribute
        int slot;             ///< Slot for the attribute semantic
    };                       

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
    struct ConstantBuffer
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
        int registerID;              ///< Register this buffer is stored in
        bool isVertexBuffer;         ///< Whether this buffer is used by the vertex or pixel shader 
        bool updated;                ///< Whether this buffer was updated last tick
    };

    std::vector<ConstantBuffer> m_buffers;     ///< Constant buffers for the shader
    std::vector<AttributeData> m_attributes;   ///< Vertex shader input attributes
    std::string m_filepath;                    ///< Path to the shader file
    std::string m_asmpath;                     ///< Path to the generated assembly file
    std::string m_sharedText;                  ///< Text to the shared shader components
    std::string m_vertexText;                  ///< Text for the vertex shader
    std::string m_pixelText;                   ///< Text for the pixel shader
    std::string m_vertexAsm;                   ///< Assembly for the vertex shader
    std::string m_pixelAsm;                    ///< Assembly for the pixel shader
    ID3D11InputLayout* m_layout;               ///< Shader input layout
    ID3D11VertexShader* m_vs;                  ///< HLSL vertex shader
    ID3D11PixelShader* m_ps;                   ///< HLSL pixel shader
    ID3D11SamplerState* m_samplerState;        ///< Texture Sampler state
    int m_index;                               ///< Unique index of the shader
    int m_textureSlots;                        ///< Number of textures allowed for this mesh
};  