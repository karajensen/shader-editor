////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directshader.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "directxcommon.h"

/**
* Holds information for an individual directx shader
*/
class DxShader
{
public:

    /**
    * Constructor
    * @param path The filepath for the shader
    */
    DxShader(const std::string& path);

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

private:

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
    * @return error message if failed or empty if succeeded
    */
    std::string BindShaderAttributes(ID3D11Device* device, ID3D10Blob* vs);

    /**
    * Information for each vertex input attribute
    */
    struct AttributeData
    {
        int byteOffset;      ///< Offset between current and previous attribute
        std::string name;    ///< The name of the attribute
        DXGI_FORMAT format;  ///< Layout of the attribute
    };

    std::vector<AttributeData> m_attributes;     ///< Vertex shader attributes
    std::string m_filepath;                      ///< Path to the shader file
    ID3D11InputLayout* m_layout;                 ///< Shader input layout
    ID3D11VertexShader* m_vs;                    ///< HLSL vertex shader
    ID3D11PixelShader* m_ps;                     ///< HLSL pixel shader
};  