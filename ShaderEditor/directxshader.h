////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directshader.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "directxcommon.h"
#include <unordered_map>

/**
* Holds information for an individual directx shader
*/
class DxShader
{
public:

    /**
    * Constructor
    * @param the unique index of the shader
    * @param path The filepath for the shader
    */
    DxShader(int index, const std::string& path);

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
    * @return error message if failed or empty if succeeded
    */
    std::string CompileShader(ID3D11Device* device);

    /**
    * Sets the shader as activated for rendering
    * @param context Direct3D device context
    */
    void SetAsActive(ID3D11DeviceContext* context);

    /**
    * Sends the matrix to the shader
    * @param name Name of the matrix to send
    * @param matrix The matrix to send
    */
    void UpdateConstantMatrix(const std::string& name, const D3DXMATRIX& matrix);

    /**
    * Sends a float or float array to the shader
    * @param name Name of the float to send
    * @param value Pointer to the float array to send
    * @param size The size of the float array
    */
    void UpdateConstantFloat(const std::string& name, const float* value, int size);

    /**
    * This bulk sends all constant data saved in the scratch buffer to the shader
    * @note Done this way as currently not possible to partially update a cbuffer
    * @param context Direct3D device context
    */
    void SendConstants(ID3D11DeviceContext* context);

    /**
    * @return the unique index of the shader
    */
    int GetIndex() const;

private:

    /**
    * Ouputs the assembly instructions to a text file
    * @param vs The vertex compiled shader to output
    * @param ps The pixel compiled shader to output
    * @return error message if failed or empty if succeeded
    */
    std::string OutputAssembly(ID3D10Blob* vs, ID3D10Blob* ps);

    /**
    * Loads the shader from the path
    * @param text The text within the file
    * @return error message if failed or empty if succeeded
    */
    std::string LoadShaderFile(std::string& text);

    /**
    * Determines the vertex shader 'in' attributes and caches them
    * @param device The directX device
    * @param vs The vertex shader object
    * @param shadertext The text of the hlsl shader
    * @return error message if failed or empty if succeeded
    */
    std::string BindVertexAttributes(ID3D11Device* device, 
        ID3D10Blob* vs, const std::string& shadertext);

    /**
    * Generates the constant buffer
    * @param device The directX device
    * @param shadertext The text of the hlsl shader
    * @return error message if failed or empty if succeeded
    */
    std::string CreateConstantBuffer(ID3D11Device* device,
        const std::string& shadertext);

    /**
    * Validates the uniform that is requesting to be sent
    * @param expectedType Type of uniform wanting to send
    * @param actualType Tyep of uniform being sent
    * @param name Name of the uniform
    */
    bool CanSendConstant(const std::string& expectedType, 
        const std::string& actualType, const std::string& name) const;

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
    * Information for a shader constant
    */
    struct ConstantData
    {
        std::string type;    ///< Type of data stored in the location
        int index;           ///< Offset from beginning of scratch buffer
    };

    typedef std::unordered_map<std::string, ConstantData> ConstantMap;

    ConstantMap m_constants;                    ///< Shader constant variables
    std::vector<AttributeData> m_attributes;    ///< Vertex shader attributes
    std::string m_filepath;                     ///< Path to the shader file
    ID3D11InputLayout* m_layout;                ///< Shader input layout
    ID3D11VertexShader* m_vs;                   ///< HLSL vertex shader
    ID3D11PixelShader* m_ps;                    ///< HLSL pixel shader
    ID3D11Buffer* m_constant;                   ///< Constant buffer
    std::vector<float> m_constantScratch;       ///< Holds temporary constant values
    int m_index;                                ///< Unique index of the shader
};  