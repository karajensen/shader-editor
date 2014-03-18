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

std::string DxShader::LoadShaderFile(std::string& text)
{
    std::string shaderText;
    std::ifstream file(m_filepath, std::ios::in|std::ios::binary|std::ios::ate);
    if(!file.is_open())
    {
        return "Could not open file " + m_filepath;
    }

    int size = static_cast<int>(file.tellg());
    file.seekg(0, std::ios::beg);
    text.resize(size);
    file.read(&text[0], text.size());
    assert(!text.empty());
    file.close();
    return std::string();
}

std::string DxShader::BindShaderAttributes(ID3D11Device* device, ID3D10Blob* vs)
{
    std::string text;
    const std::string errorBuffer = LoadShaderFile(text);
    if(!errorBuffer.empty())
    {
        return errorBuffer;
    }

    std::string attributeList;
    const std::string entryPoint("VShader(");
    int index = text.find(entryPoint, 0) + entryPoint.size();
    while(text[index] != ')')
    {
        attributeList += text[index];
        ++index;
    }

    m_attributes.clear();
    std::vector<std::string> components;
    boost::split(components, attributeList, boost::is_any_of(",:;\n\r "), boost::token_compress_on);

    int byteOffset = 0;
    for(unsigned index = 0; index < components.size(); index += 3)
    {
        AttributeData data;
        data.name = components[index+2];

        if(components[index] == "float3" || data.name == "POSITION")
        {
            data.byteOffset = byteOffset;
            data.format = DXGI_FORMAT_R32G32B32_FLOAT;
            byteOffset += 12;
        }
        else if(components[index] == "float4")
        {
            data.byteOffset = byteOffset;
            data.format = DXGI_FORMAT_R32G32B32A32_FLOAT;
            byteOffset += 16;
        }

        m_attributes.push_back(data);
    }
   
    D3D11_INPUT_ELEMENT_DESC* vertexDescription = new D3D11_INPUT_ELEMENT_DESC[m_attributes.size()];
    for(unsigned int i = 0; i < m_attributes.size(); ++i)
    {
        vertexDescription[i].AlignedByteOffset = m_attributes[i].byteOffset;
        vertexDescription[i].Format = m_attributes[i].format;
        vertexDescription[i].InputSlot = 0;
        vertexDescription[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        vertexDescription[i].InstanceDataStepRate = 0;
        vertexDescription[i].SemanticIndex= 0;
        vertexDescription[i].SemanticName = m_attributes[i].name.c_str();
    }
    
    if(FAILED(device->CreateInputLayout(vertexDescription, 2, 
        vs->GetBufferPointer(), vs->GetBufferSize(), &m_layout)))
    {
        return "Could not create input layout";
    }

    delete [] vertexDescription;
    return std::string();
}

void DxShader::SetAsActive(ID3D11DeviceContext* context)
{
    context->VSSetShader(m_vs, 0, 0);
    context->PSSetShader(m_ps, 0, 0);
    context->IASetInputLayout(m_layout);
}