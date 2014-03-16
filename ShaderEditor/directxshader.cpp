////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directxshader.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "directxshader.h"
#include "boost/algorithm/string.hpp"

DxShader::DxShader(const std::string& path) :
    m_filepath(path),
    m_layout(nullptr),
    m_vs(nullptr),
    m_ps(nullptr)
{
}

DxShader::~DxShader()
{
    Release();
}

void DxShader::Release()
{
    if(m_vs)
    {
        m_vs->Release();
        m_vs = nullptr;
    }
    if(m_ps)
    {
        m_ps->Release();
        m_ps = nullptr;
    }
    if(m_layout)
    {
        m_layout->Release();
        m_layout = nullptr;
    }
}

std::string DxShader::CompileShader(ID3D11Device* device)
{
    ID3D10Blob* vsBlob = nullptr;
    ID3D10Blob* psBlob = nullptr;
    ID3D10Blob* vsErrorBlob = nullptr;
    ID3D10Blob* psErrorBlob = nullptr;

    std::string vertexError;
    if(FAILED(D3DX11CompileFromFile(m_filepath.c_str(), 0, 0,
        "VShader", "vs_5_0", 0, 0, 0, &vsBlob, &vsErrorBlob, 0)))
    {
        vertexError = std::string("Vertex Shader: ") + (vsErrorBlob ? 
            static_cast<char*>(vsErrorBlob->GetBufferPointer()) : "Unknown Error");
    }

    std::string pixelError;
    if(FAILED(D3DX11CompileFromFile(m_filepath.c_str(), 0, 0,
        "PShader", "ps_5_0", 0, 0, 0, &psBlob, &psErrorBlob, 0)))
    {
        pixelError = std::string("Pixel Shader: ") + (psErrorBlob ? 
            static_cast<char*>(psErrorBlob->GetBufferPointer()) : "Unknown Error");
    }

    if(!vertexError.empty() || !pixelError.empty())
    {
        return vertexError.empty() ? pixelError :
            vertexError + (pixelError.empty() ? "" : "\n"+pixelError);
    }

    Release();

    if(FAILED(device->CreateVertexShader(vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(), 0, &m_vs)))
    {
        return "Could not create vertex shader";
    }
    
    if(FAILED(device->CreatePixelShader(psBlob->GetBufferPointer(),
        psBlob->GetBufferSize(), 0, &m_ps)))
    {
        return "Could not create pixel shader";
    }

    return BindShaderAttributes(device, vsBlob);
}

std::string DxShader::BindShaderAttributes(ID3D11Device* device, ID3D10Blob* vs)
{
    std::string shaderText;
    std::ifstream file(m_filepath, std::ios::in|std::ios::binary|std::ios::ate);
    if(file.is_open())
    {
        int size = static_cast<int>(file.tellg());
        file.seekg(0, std::ios::beg);
        shaderText.resize(size);
        file.read(&shaderText[0], shaderText.size());
        assert(!shaderText.empty());
        file.close();
    }
    else
    {
       return "Could not open file " + m_filepath;
    }

    m_attributes.clear();
    std::vector<std::string> components;
    boost::split(components, shaderText, 
        boost::is_any_of(";\n\r "), boost::token_compress_on);








    D3D11_INPUT_ELEMENT_DESC vertexDescription[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };
    
    if(FAILED(device->CreateInputLayout(vertexDescription, 2, 
        vs->GetBufferPointer(), vs->GetBufferSize(), &m_layout)))
    {
        return "Could not create input layout";
    }
    return std::string();
}

void DxShader::SetAsActive(ID3D11DeviceContext* context)
{
    context->VSSetShader(m_vs, 0, 0);
    context->PSSetShader(m_ps, 0, 0);
    context->IASetInputLayout(m_layout);
}