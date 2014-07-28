////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directxshader.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "directxshader.h"
#include "boost/algorithm/string.hpp"
#include "boost/regex.hpp"
#include "directx/include/D3Dcompiler.h"
#include <fstream>

namespace
{
    bool REMOVE_ASM_COMMENTS = true; ///< Whether to strip out any comments
    bool OUTPUT_ASM_FILE = true; ///< Whether to output assembly to a file

    const std::string VS("Vertex Shader: "); ///< Text for vertex shader diagnostics
    const std::string PS("Pixel Shader: "); ///< Text for pixel shader diagnostics

    // HLSL Keywords
    const std::string VERTEX_RETURN("Attributes");
    const std::string PIXEL_RETURN("float4");
    const std::string VERTEX_ENTRY("VShader");
    const std::string PIXEL_ENTRY("PShader");
    const std::string VERTEX_MODEL("vs_5_0");
    const std::string PIXEL_MODEL("ps_5_0");
    const std::string HLSL_IN_POSITION("POSITION");
    const std::string HLSL_FLT("float");
    const std::string HLSL_FLT2(HLSL_FLT + "2");
    const std::string HLSL_FLT3(HLSL_FLT + "3");
    const std::string HLSL_FLT4(HLSL_FLT + "4");
    const std::string HLSL_MAT4("float4x4");
    const std::string HLSL_CONSTANT_BUFFER("cbuffer");
    const std::string HLSL_VERTEX_BUFFER("VertexBuffer");
    const std::string HLSL_PIXEL_BUFFER("PixelBuffer");
    const std::string HLSL_TEXTURE2D("Texture2D");
    const std::string HLSL_TEXTURE2DMS("Texture2DMS<float4," + SAMPLES + ">");
    const std::string HLSL_SAMPLER("SamplerState");
}

DxShader::DxShader(int index, const std::string& filepath) :
    m_filepath(filepath),
    m_layout(nullptr),
    m_vs(nullptr),
    m_ps(nullptr),
    m_index(index),
    m_samplerState(nullptr),
    m_textureSlots(0)
{
    m_asmpath = boost::ireplace_last_copy(
        m_filepath, SHADER_EXTENSION, ASM_EXTENSION);
}

DxShader::~DxShader()
{
    Release();
}

void DxShader::Release()
{
    m_buffers.clear();
    m_attributes.clear();
    m_textureSlots = 0;

    SafeRelease(&m_samplerState);
    SafeRelease(&m_vs);
    SafeRelease(&m_ps);
    SafeRelease(&m_layout);
}

DxShader::ConstantBuffer::ConstantBuffer() :
    buffer(nullptr)
{
}

DxShader::ConstantBuffer::~ConstantBuffer()
{
    SafeRelease(&buffer);
}

std::string DxShader::CompileShader(ID3D11Device* device)
{
    ID3D10Blob* vsBlob = nullptr;
    ID3D10Blob* psBlob = nullptr;
    ID3D10Blob* vsErrorBlob = nullptr;
    ID3D10Blob* psErrorBlob = nullptr;

    std::string vertexError;
    if(FAILED(D3DX11CompileFromFile(m_filepath.c_str(), 0, 0,
        VERTEX_ENTRY.c_str(), VERTEX_MODEL.c_str(), 0, 0, 0, &vsBlob, &vsErrorBlob, 0)))
    {
        const std::string error(static_cast<char*>(vsErrorBlob->GetBufferPointer()));
        vertexError = VS + (vsErrorBlob ? error : "Unknown Error");
    }

    std::string pixelError;
    if(FAILED(D3DX11CompileFromFile(m_filepath.c_str(), 0, 0,
        PIXEL_ENTRY.c_str(), PIXEL_MODEL.c_str(), 0, 0, 0, &psBlob, &psErrorBlob, 0)))
    {
        const std::string error(static_cast<char*>(psErrorBlob->GetBufferPointer()));
        pixelError = PS + (psErrorBlob ? error : "Unknown Error");
    }

    if(!vertexError.empty() || !pixelError.empty())
    {
        return vertexError.empty() ? pixelError :
            vertexError + (pixelError.empty() ? "" : "\n"+pixelError);
    }

    // Only update the shader once compilation errors have been checked
    Release();

    std::string vertexText, pixelText, sharedText;
    std::string errorBuffer = LoadShaderText(vertexText, pixelText, sharedText);
    if(!errorBuffer.empty())
    {
        return errorBuffer;
    }

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

    errorBuffer = BindVertexAttributes(device, vsBlob, vertexText);
    if(!errorBuffer.empty())
    {
        return errorBuffer;
    }

    errorBuffer = CreateConstantBuffer(device, sharedText, HLSL_VERTEX_BUFFER);
    if(!errorBuffer.empty())
    {
        return errorBuffer;
    }

    errorBuffer = CreateConstantBuffer(device, sharedText, HLSL_PIXEL_BUFFER);
    if(!errorBuffer.empty())
    {
        return errorBuffer;
    }

    errorBuffer = CreateSamplerState(device, sharedText);
    if(!errorBuffer.empty())
    {
        return errorBuffer;
    }

    std::string vertexAsm, pixelAsm;
    errorBuffer = GenerateAssembly(vsBlob, psBlob, vertexAsm, pixelAsm);
    if(!errorBuffer.empty())
    {
        return errorBuffer;
    }

    UpdateShaderText(vertexText, pixelText, sharedText, vertexAsm, pixelAsm);
    SetDebugNames();

    return std::string();
}

void DxShader::SetDebugNames()
{
    SetDebugName(m_vs, m_filepath + "_Vertex");
    SetDebugName(m_ps, m_filepath + "_Pixel");
    SetDebugName(m_layout, m_filepath + "_Layout");
    SetDebugName(m_samplerState, m_filepath + "_Sampler");
    for(const auto& constantBuffer : m_buffers)
    {
        SetDebugName(constantBuffer.buffer, 
            m_filepath + "_" + constantBuffer.name);
    }
}

void DxShader::UpdateShaderText(const std::string& vText, 
                                const std::string& pText,
                                const std::string& shared, 
                                const std::string& vAsm, 
                                const std::string& pAsm)
{
    m_vertexText = vText;
    m_vertexAsm = vAsm;
    m_pixelText = pText;
    m_pixelAsm = pAsm;
    m_sharedText = shared;
}

std::string DxShader::LoadShaderText(std::string& vertexText, 
                                     std::string& pixelText, 
                                     std::string& sharedText)
{
    std::ifstream file(m_filepath.c_str(), std::ios::in|std::ios::ate|std::ios::_Nocreate);
    if(!file.is_open())
    {
        return "Could not open file " + m_filepath;
    }

    // Read all from file directly into a string
    const int size = static_cast<int>(file.tellg());
    file.seekg(0, std::ios::beg);
    std::string text(size, ' ');
    file.read(&text[0], text.size());
    if(text.empty())
    {
        return m_filepath + " is empty";
    }

    // Section into vertex and pixel shaders
    const int vertexIndex = text.find(VERTEX_RETURN + " " + VERTEX_ENTRY);
    const int pixelIndex = text.find(PIXEL_RETURN + " " + PIXEL_ENTRY);
    vertexText = std::string(text.begin() + vertexIndex, text.begin() + pixelIndex - 1);
    pixelText = std::string(text.begin() + pixelIndex, text.end());
    sharedText = std::string(text.begin(), text.begin() + vertexIndex - 1);

    return std::string();
}

std::string DxShader::GenerateAssembly(ID3D10Blob* vs, 
                                       ID3D10Blob* ps,
                                       std::string& vertexAsm, 
                                       std::string& pixelAsm)
{
    ID3DBlob* vertexAsmBlob = nullptr;
    D3DDisassemble(vs->GetBufferPointer(), vs->GetBufferSize(), 
        D3D_DISASM_DISABLE_DEBUG_INFO, 0, &vertexAsmBlob);

    if(!vertexAsmBlob)
    {
        return "Failed to generate vertex shader assembly";
    }

    ID3DBlob* pixelAsmBlob = nullptr;
    D3DDisassemble(ps->GetBufferPointer(), ps->GetBufferSize(), 
        D3D_DISASM_DISABLE_DEBUG_INFO, 0, &pixelAsmBlob);

    if(!pixelAsmBlob)
    {
        return "Failed to generate pixel shader assembly";
    }

    vertexAsm = static_cast<char*>(vertexAsmBlob->GetBufferPointer());
    pixelAsm = static_cast<char*>(pixelAsmBlob->GetBufferPointer());

    if(REMOVE_ASM_COMMENTS)
    {
        boost::regex reg("//.*?\n");
        vertexAsm = boost::regex_replace(vertexAsm, reg, "");
        pixelAsm = boost::regex_replace(pixelAsm, reg, "");
    }

    if(OUTPUT_ASM_FILE)
    {
        std::ofstream file(m_asmpath.c_str(), std::ios_base::out|std::ios_base::trunc);
        if(!file.is_open())
        {
            return "Could not open file " + m_asmpath;
        }    

        file << vertexAsm << std::endl << std::endl << pixelAsm;
        file.close();
    }

    pixelAsmBlob->Release();
    vertexAsmBlob->Release();

    return std::string();
}

std::string DxShader::BindVertexAttributes(ID3D11Device* device, 
                                           ID3D10Blob* vs,
                                           const std::string& text)
{
    // Find a string of all text within the entry point brackets main(...)
    std::string attributeList;
    const std::string entryPoint(VERTEX_ENTRY + "(");
    int index = text.find(entryPoint, 0) + entryPoint.size();
    while(text[index] != ')')
    {
        attributeList += text[index];
        ++index;
    }
    boost::trim(attributeList);

    // Section into words, removing any spaces, tokens etc.
    std::vector<std::string> components;
    boost::split(components, attributeList, 
        boost::is_any_of(",:;\n\r "), boost::token_compress_on);

    // Ordering of input attributes is assumed to be: 'type name semantic'
    int byteOffset = 0;
    for(unsigned index = 0; index < components.size(); index += 3)
    {
        AttributeData data;
        data.slot = 0;
        data.semantic = components[index+2];

        // SEMANTIC0->9 requires input slot 0->9 to be set
        char lastChar = data.semantic[data.semantic.size()-1];
        if(isdigit(lastChar))
        {            
            data.slot = boost::lexical_cast<int>(lastChar);
            data.semantic = std::string(data.semantic.begin(), data.semantic.end()-1);
        }

        // Pass position as a vec3 into a vec4 slot to use the optimization 
        // where the 'w' component is automatically set as 1.0
        if(components[index] == HLSL_FLT3 || data.semantic == HLSL_IN_POSITION)
        {
            data.byteOffset = byteOffset;
            data.format = DXGI_FORMAT_R32G32B32_FLOAT;
            byteOffset += 3 * sizeof(float);
        }
        else if(components[index] == HLSL_FLT4)
        {
            data.byteOffset = byteOffset;
            data.format = DXGI_FORMAT_R32G32B32A32_FLOAT;
            byteOffset += 4 * sizeof(float);
        }
        else if(components[index] == HLSL_FLT2)
        {
            data.byteOffset = byteOffset;
            data.format = DXGI_FORMAT_R32G32_FLOAT;
            byteOffset += 2 * sizeof(float);
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
        vertexDescription[i].SemanticName = m_attributes[i].semantic.c_str();
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
                                           const std::string& text,
                                           std::string bufferName)
{
    std::string errorBuffer;
    bufferName = HLSL_CONSTANT_BUFFER + " " + bufferName;
    int index = text.find(bufferName, 0);
    if(index == NO_INDEX)
    {
        return errorBuffer;
    }

    m_buffers.push_back(ConstantBuffer());
    auto& buffer = m_buffers[m_buffers.size()-1];
    buffer.name = bufferName;

    // Find a string of all text within the constant buffer brackets
    std::string constantList;
    index += bufferName.size();
    
    while(text[index] != '}')
    {
        constantList += text[index];
        ++index;
    }

    // Section into words, removing any spaces, tokens etc.
    std::vector<std::string> components;
    boost::split(components, constantList, 
        boost::is_any_of("{},:;\n\r "), boost::token_compress_on);

    // Ordering of constant buffer is assumed to be: 'type name'
    // Assumes all constants are used by desired vertex/pixel shader
    // Note this can affect the ordering which is not compenstated for here
    int byteWidth = 0;
    int slotsRemaining = 0;
    for(unsigned index = 0; index < components.size(); ++index)
    {
        if(!components[index].empty())
        {
            const std::string& type = components[index];
            const std::string& name = components[index+1];
            buffer.constants[name].index = byteWidth/sizeof(float);

            // When determining the byte offset of constant buffer data
            // take into account optimizations that the buffer will perform where
            // float/float2/float3 are packed together in lots of float4 if possible
            // This is similar to how a C++ struct/class is packed together
            // Each component of a float4 is considered a 'slot'
            if(type == HLSL_MAT4)
            {
                byteWidth += slotsRemaining * sizeof(float); // fill in empty slots
                byteWidth += 16 * sizeof(float);
                slotsRemaining = 4;
            }
            else if(type == HLSL_FLT4)
            {
                byteWidth += slotsRemaining * sizeof(float); // fill in empty slots
                byteWidth += 4 * sizeof(float);
                slotsRemaining = 4;
            }
            else if(type == HLSL_FLT3)
            {
                if(slotsRemaining <= 2)
                {
                    byteWidth += slotsRemaining * sizeof(float); // can only fit in slots of 3+
                    slotsRemaining = 4;
                }
                byteWidth += 3 * sizeof(float);
                slotsRemaining -= 3;
            }
            else if(type == HLSL_FLT2)
            {
                if(slotsRemaining <= 1)
                {
                    byteWidth += slotsRemaining * sizeof(float); // can only fit in slots of 2+
                    slotsRemaining = 4;
                }
                byteWidth += 2 * sizeof(float);
                slotsRemaining -= 2;
            }
            else if(type == HLSL_FLT)
            {
                byteWidth += sizeof(float);
                if(slotsRemaining == 0)
                {
                    slotsRemaining = 4;
                }
                --slotsRemaining;
            }
            
            buffer.constants[name].type = type;
            ++index;
        }
    }
    byteWidth += slotsRemaining * sizeof(float);

    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = byteWidth;
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    buffer.scratch.resize(byteWidth/sizeof(float));
    buffer.scratch.assign(buffer.scratch.size(), 0.0f);
    
    if(FAILED(device->CreateBuffer(&bd, 0, &buffer.buffer)))
    {
        errorBuffer = "Constant buffer " + buffer.name + " creation failed";
    }

    return errorBuffer;
}

void DxShader::SetActive(ID3D11DeviceContext* context)
{
    context->VSSetShader(m_vs, 0, 0);
    context->PSSetShader(m_ps, 0, 0);
    context->IASetInputLayout(m_layout);

    for(const auto& constantBuffer : m_buffers)
    {
        context->VSSetConstantBuffers(0, 1, &constantBuffer.buffer);
    }

    if(m_samplerState)
    {
        context->PSSetSamplers(0, 1, &m_samplerState);
    }
}

void DxShader::UpdateConstantFloat(const std::string& name, const float* value, int size)
{
    std::string floatType = HLSL_FLT;
    if(size > 1)
    {
        floatType += boost::lexical_cast<std::string>(size);
    }

    for(auto& buffer : m_buffers)
    {
        auto itr = buffer.constants.find(name);
        if(itr != buffer.constants.end() && 
            CanSendConstant(floatType, itr->second.type, name))
        {
            for(int i = 0; i < size; ++i)
            {
                buffer.scratch[itr->second.index + i] = value[i];
            }
            return;
        }
    }
}

void DxShader::UpdateConstantMatrix(const std::string& name, const D3DXMATRIX& matrix)
{
    for(auto& buffer : m_buffers)
    {
        auto itr = buffer.constants.find(name);
        if(itr != buffer.constants.end() && 
            CanSendConstant(HLSL_MAT4, itr->second.type, name))
        {
            const int scratchIndex = itr->second.index;
            const FLOAT* matArray = matrix;
            for(int i = 0; i < 16; ++i) // 16 floats in a directx matrix
            {
                buffer.scratch[scratchIndex + i] = matArray[i];
            }
            return;
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
    for(const auto& constantBuffer : m_buffers)
    {
        context->UpdateSubresource(
            constantBuffer.buffer, 0, 0,
            &constantBuffer.scratch[0], 0, 0);
    }
}

int DxShader::GetIndex() const
{
    return m_index;
}

std::string DxShader::CreateSamplerState(ID3D11Device* device, const std::string& text)
{
    if(boost::algorithm::icontains(text, HLSL_SAMPLER) ||
       boost::algorithm::icontains(text, HLSL_TEXTURE2D) ||
       boost::algorithm::icontains(text, HLSL_TEXTURE2DMS))
    {
        D3D11_SAMPLER_DESC samplerDesc;
        samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.MipLODBias = 0.0f;
        samplerDesc.MaxAnisotropy = 1;
        samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
        samplerDesc.BorderColor[0] = 0;
        samplerDesc.BorderColor[1] = 0;
        samplerDesc.BorderColor[2] = 0;
        samplerDesc.BorderColor[3] = 0;
        samplerDesc.MinLOD = 0;
        samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

        if(FAILED(device->CreateSamplerState(&samplerDesc, &m_samplerState)))
        {
            return "Failed to create texture sampler";
        }

        // Determine the number of allowed texture slots
        std::vector<std::string> components;
        boost::split(components, text, boost::is_any_of("\n "), boost::token_compress_on);

        m_textureSlots = 
            std::count(components.begin(), components.end(), HLSL_TEXTURE2D) +
            std::count(components.begin(), components.end(), HLSL_TEXTURE2DMS);
    }

    return std::string();
}

bool DxShader::HasTextureSlot(int slot)
{
    return slot <= m_textureSlots;
}

std::string DxShader::GetText() const
{
    return m_sharedText + "\n" +  m_vertexText + "\n" + m_pixelText;
}

std::string DxShader::GetAssembly() const
{
    return m_vertexAsm + "\n" + m_pixelAsm;
}