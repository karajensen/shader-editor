////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directxshader.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "directxshader.h"
#include "boost/algorithm/string.hpp"

namespace
{
    const std::string VS("Vertex Shader: ");    ///< Text for vertex shader diagnostics
    const std::string PS("Pixel Shader: ");     ///< Text for pixel shader diagnostics
}

DxShader::DxShader(int index, const std::string& path) :
    m_filepath(path),
    m_layout(nullptr),
    m_vs(nullptr),
    m_ps(nullptr),
    m_constant(nullptr),
    m_index(index)
{
}

DxShader::~DxShader()
{
    Release();
}

void DxShader::Release()
{
    m_attributes.clear();
    m_constants.clear();
    m_constantScratch.clear();

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
    if(m_constant)
    {
        m_constant->Release();
        m_constant = nullptr;
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
        const std::string error(static_cast<char*>(vsErrorBlob->GetBufferPointer()));
        vertexError = VS + (vsErrorBlob ? error : "Unknown Error");
    }

    std::string pixelError;
    if(FAILED(D3DX11CompileFromFile(m_filepath.c_str(), 0, 0,
        "PShader", "ps_5_0", 0, 0, 0, &psBlob, &psErrorBlob, 0)))
    {
        const std::string error(static_cast<char*>(psErrorBlob->GetBufferPointer()));
        pixelError = PS + (psErrorBlob ? error : "Unknown Error");
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
        return VS + "Could not create shader";
    }
    
    if(FAILED(device->CreatePixelShader(psBlob->GetBufferPointer(),
        psBlob->GetBufferSize(), 0, &m_ps)))
    {
        return PS + "Could not create shader";
    }

    std::string shadertext;
    const std::string errorBuffer = LoadShaderFile(shadertext);
    if(!errorBuffer.empty())
    {
        return errorBuffer;
    }

    std::string attributeError = BindVertexAttributes(device, vsBlob, shadertext);
    if(!attributeError.empty())
    {
        return attributeError;
    }

    return CreateConstantBuffer(device, shadertext);
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

std::string DxShader::BindVertexAttributes(ID3D11Device* device, 
                                           ID3D10Blob* vs,
                                           const std::string& shadertext)
{
    // Find a string of all text within the entry point brackets
    std::string attributeList;
    const std::string entryPoint("VShader(");
    int index = shadertext.find(entryPoint, 0) + entryPoint.size();
    while(shadertext[index] != ')')
    {
        attributeList += shadertext[index];
        ++index;
    }

    std::vector<std::string> components;
    boost::split(components, attributeList, 
        boost::is_any_of(",:;\n\r "), boost::token_compress_on);

    int byteOffset = 0;
    const std::string texCoord = "TEXCOORD";
    for(unsigned index = 0; index < components.size(); index += 3)
    {
        AttributeData data;
        data.slot = 0;
        data.name = components[index+2];

        // TEXCOORD0->9 requires input slot 0->9 to be set
        if(boost::icontains(data.name, texCoord))
        {            
            data.slot = boost::lexical_cast<int>(
                boost::erase_head_copy(data.name, texCoord.size()));
            data.name = texCoord;
        }

        // Pass position as a vec3 into a vec4 slot as an 
        // optimization to have the 'w' component set as 1.0
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
        else if(components[index] == "float2")
        {
            data.byteOffset = byteOffset;
            data.format = DXGI_FORMAT_R32G32_FLOAT;
            byteOffset += 8;
        }

        m_attributes.push_back(data);
    }
   
    D3D11_INPUT_ELEMENT_DESC* vertexDescription = new D3D11_INPUT_ELEMENT_DESC[m_attributes.size()];
    for(unsigned int i = 0; i < m_attributes.size(); ++i)
    {
        vertexDescription[i].AlignedByteOffset = m_attributes[i].byteOffset;
        vertexDescription[i].Format = m_attributes[i].format;
        vertexDescription[i].InputSlot = m_attributes[i].slot;
        vertexDescription[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        vertexDescription[i].InstanceDataStepRate = 0;
        vertexDescription[i].SemanticIndex = 0;
        vertexDescription[i].SemanticName = m_attributes[i].name.c_str();
    }
    
    if(FAILED(device->CreateInputLayout(vertexDescription, m_attributes.size(), 
        vs->GetBufferPointer(), vs->GetBufferSize(), &m_layout)))
    {
        return "Could not create input layout";
    }

    delete [] vertexDescription;
    return std::string();
}

std::string DxShader::CreateConstantBuffer(ID3D11Device* device, 
                                           const std::string& shadertext)
{
    // Generate a list of all variables in the constant buffer
    std::string constantList;
    const std::string bufferName("ConstantBuffer");
    int index = shadertext.find(bufferName, 0) + bufferName.size();
    while(shadertext[index] != '}')
    {
        constantList += shadertext[index];
        ++index;
    }

    std::vector<std::string> components;
    boost::split(components, constantList, 
        boost::is_any_of("{},:;\n\r "), boost::token_compress_on);

    int byteWidth = 0;
    int slotsRemaining = 0;
    for(unsigned index = 0; index < components.size(); ++index)
    {
        if(!components[index].empty())
        {
            const std::string& type = components[index];
            const std::string& name = components[index+1];
            m_constants[name].index = byteWidth/sizeof(float);

            // When determining the byte offset of constant buffer data
            // take into account optimizations that the buffer will perform where
            // float/float2/float3 are packed together in lots of float4 if possible
            // Each component of a float4 is considered a 'slot'
            if(type == "float4x4")
            {
                byteWidth += slotsRemaining * 4; // fill in empty slots
                byteWidth += 64;
                slotsRemaining = 4;
            }
            else if(type == "float4")
            {
                byteWidth += slotsRemaining * 4; // fill in empty slots
                byteWidth += 16;
                slotsRemaining = 4;
            }
            else if(type == "float3")
            {
                if(slotsRemaining <= 2)
                {
                    byteWidth += slotsRemaining * 4; // can only fit in slots of 3+
                    slotsRemaining = 4;
                }
                byteWidth += 12;
                slotsRemaining -= 3;
            }
            else if(type == "float2")
            {
                if(slotsRemaining <= 1)
                {
                    byteWidth += slotsRemaining * 4; // can only fit in slots of 2+
                    slotsRemaining = 4;
                }
                byteWidth += 8;
                slotsRemaining -= 2;
            }
            else if(type == "float")
            {
                byteWidth += 4;
                if(slotsRemaining == 0)
                {
                    slotsRemaining = 4;
                }
                --slotsRemaining;
            }
            
            m_constants[name].type = type;
            ++index;
        }
    }
    byteWidth += slotsRemaining * 4;

    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = byteWidth;
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    m_constantScratch.resize(byteWidth/sizeof(float));
    m_constantScratch.assign(m_constantScratch.size(), 0.0f);
    
    if(FAILED(device->CreateBuffer(&bd, 0, &m_constant)))
    {
        return "Constant buffer creation failed";
    }
    return std::string();
}

void DxShader::SetAsActive(ID3D11DeviceContext* context)
{
    context->VSSetShader(m_vs, 0, 0);
    context->PSSetShader(m_ps, 0, 0);
    context->IASetInputLayout(m_layout);
    context->VSSetConstantBuffers(0, 1, &m_constant);
}

void DxShader::UpdateConstantFloat(const std::string& name, const float* value, int size)
{
    std::string floatType = "float";
    if(size > 1)
    {
        floatType += boost::lexical_cast<std::string>(size);
    }

    auto itr = m_constants.find(name);
    if(itr != m_constants.end() && 
        CanSendConstant(floatType, itr->second.type, name))
    {
        for(int i = 0; i < size; ++i)
        {
            m_constantScratch[itr->second.index] = value[i];
        }
    }
}

void DxShader::UpdateConstantMatrix(const std::string& name, const D3DXMATRIX& matrix)
{
    auto itr = m_constants.find(name);
    if(itr != m_constants.end() && 
        CanSendConstant("float4x4", itr->second.type, name))
    {
        const int scratchIndex = itr->second.index;
        const FLOAT* matArray = matrix;
        for(int i = 0; i < 16; ++i) // 16 floats in a directx matrix
        {
            m_constantScratch[scratchIndex + i] = matArray[i];
        }
    }
}

bool DxShader::CanSendConstant(const std::string& expectedType, 
                               const std::string& actualType, 
                               const std::string& name) const
{
    if(actualType != expectedType)
    {
        Logger::LogError(name + " type mismatch. Attempting to send " + 
            actualType + " as a " + expectedType);
        return false;
    }
    return true;
}

void DxShader::SendConstants(ID3D11DeviceContext* context)
{
    context->UpdateSubresource(m_constant, 0, 0, &m_constantScratch[0], 0, 0);
}

int DxShader::GetIndex() const
{
    return m_index;
}