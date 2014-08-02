////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directxshader.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "directxshader.h"
#include "boost/algorithm/string.hpp"
#include "boost/regex.hpp"
#include "boost/assign.hpp"
#include "boost/bimap.hpp"
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
    const std::string HLSL_CONSTANT_REGISTER("register(b");
    const std::string HLSL_TEXTURE2D("Texture2D");
    const std::string HLSL_TEXTURE2DMS("Texture2DMS<float4," + SAMPLES + ">");
    const std::string HLSL_SAMPLER("SamplerState");

    // How many float components to the HLSL float structure
    boost::bimap<int, std::string> FLOAT_COMPONENTS = 
        boost::assign::list_of<boost::bimap<int, std::string>::relation>
        (1, HLSL_FLT)
        (2, HLSL_FLT2)
        (3, HLSL_FLT3)
        (4, HLSL_FLT4)
        (16, HLSL_MAT4);
}

DxShader::DxShader(int index, const std::string& name, const std::string& filepath) :
    m_filepath(filepath),
    m_layout(nullptr),
    m_vs(nullptr),
    m_ps(nullptr),
    m_psBlob(nullptr),
    m_vsBlob(nullptr),
    m_vsReflection(nullptr),
    m_psReflection(nullptr),
    m_index(index),
    m_samplerState(nullptr),
    m_textureSlots(0),
    m_name(name)
{
    m_asmpath = boost::ireplace_last_copy(
        m_filepath, SHADER_EXTENSION, ASM_EXTENSION);
}

DxShader::~DxShader()
{
    SafeRelease(&m_vsBlob);
    SafeRelease(&m_psBlob);
    Release();
}

void DxShader::Release()
{
    m_cbuffers.clear();
    m_attributes.clear();
    m_textureSlots = 0;

    SafeRelease(&m_vsReflection);
    SafeRelease(&m_psReflection);
    SafeRelease(&m_samplerState);
    SafeRelease(&m_vs);
    SafeRelease(&m_ps);
    SafeRelease(&m_layout);
}

DxShader::ConstantBuffer::ConstantBuffer() :
    buffer(nullptr),
    isVertexBuffer(false),
    updated(false),
    startSlot(NO_INDEX)
{
}

DxShader::ConstantBuffer::~ConstantBuffer()
{
    SafeRelease(&buffer);
}

std::string DxShader::CompileShader(ID3D11Device* device)
{
    std::string vertexError = CompileShader(&m_vsBlob, true);
    std::string pixelError = CompileShader(&m_psBlob, false);

    if(!vertexError.empty() || !pixelError.empty())
    {
        return vertexError.empty() ? pixelError :
            vertexError + (pixelError.empty() ? "" : "\n"+pixelError);
    }

    Release();

    std::string vertexText, pixelText, sharedText;
    std::string errorBuffer = LoadShaderText(vertexText, pixelText, sharedText);
    if(!errorBuffer.empty())
    {
        return errorBuffer;
    }

    errorBuffer = CreateShaders(device);
    if(!errorBuffer.empty())
    {
        return errorBuffer;
    }

    errorBuffer = FindShaderDescription(m_vsBlob, &m_vsReflection, m_vertexDesc);
    if(!errorBuffer.empty())
    {
        return VS + errorBuffer;
    }

    errorBuffer = FindShaderDescription(m_psBlob, &m_psReflection, m_pixelDesc);
    if(!errorBuffer.empty())
    {
        return PS + errorBuffer;
    }

    errorBuffer = BindVertexAttributes(device, vertexText);
    if(!errorBuffer.empty())
    {
        return VS + errorBuffer;
    }

    errorBuffer = CreateConstantBuffers(device, sharedText);
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
    errorBuffer = GenerateAssembly(vertexAsm, pixelAsm);
    if(!errorBuffer.empty())
    {
        return errorBuffer;
    }

    m_vertexText = vertexText;
    m_vertexAsm = vertexAsm;
    m_pixelText = pixelText;
    m_pixelAsm = pixelAsm;
    m_sharedText = sharedText;

    SetDebugNames();
    return std::string();
}

std::string DxShader::CompileShader(ID3D10Blob** shader, bool isVertex)
{
    *shader = nullptr;
    ID3D10Blob* errorBlob = nullptr;

    const std::string entry = isVertex ? VERTEX_ENTRY : PIXEL_ENTRY;
    const std::string model = isVertex ? VERTEX_MODEL : PIXEL_MODEL;

    if(FAILED(D3DX11CompileFromFile(m_filepath.c_str(), 0, 0,
        entry.c_str(), model.c_str(), 0, 0, 0, shader, &errorBlob, 0)) || !(*shader))
    {
        return (isVertex ? VS : PS) + (!errorBlob ? "Unknown Error" :
            std::string(static_cast<char*>(errorBlob->GetBufferPointer())));
    }

    SafeRelease(&errorBlob);
    return std::string();
}

std::string DxShader::CreateShaders(ID3D11Device* device)
{
    if(FAILED(device->CreateVertexShader(m_vsBlob->GetBufferPointer(),
        m_vsBlob->GetBufferSize(), 0, &m_vs)))
    {
        return VS + "Could not create shader";
    }

    if(FAILED(device->CreatePixelShader(m_psBlob->GetBufferPointer(),
        m_psBlob->GetBufferSize(), 0, &m_ps)))
    {
        return PS + "Could not create shader";
    }

    return std::string();
}

std::string DxShader::FindShaderDescription(ID3D10Blob* shader, 
                                            ID3D11ShaderReflection** reflection,
                                            D3D11_SHADER_DESC& desc)
{
    if(FAILED(D3DReflect(shader->GetBufferPointer(), shader->GetBufferSize(), 
        IID_ID3D11ShaderReflection, (void**)reflection)) || !(*reflection))
    {
        return "Could not reflect shader details";
    }

    if(FAILED((*reflection)->GetDesc(&desc)))
    {
        return "Could not query shader description";
    }

    return std::string();
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

std::string DxShader::GenerateAssembly(std::string& vertexAsm, 
                                       std::string& pixelAsm)
{
    ID3DBlob* vertexAsmBlob = nullptr;
    D3DDisassemble(m_vsBlob->GetBufferPointer(), m_vsBlob->GetBufferSize(), 
        D3D_DISASM_DISABLE_DEBUG_INFO, 0, &vertexAsmBlob);

    if(!vertexAsmBlob)
    {
        return "Failed to generate vertex shader assembly";
    }

    ID3DBlob* pixelAsmBlob = nullptr;
    D3DDisassemble(m_psBlob->GetBufferPointer(), m_psBlob->GetBufferSize(), 
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

std::vector<std::string> DxShader::GetVertexAttributes(const std::string& text) const
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
    return components;
}

std::string DxShader::BindVertexAttributes(ID3D11Device* device, 
                                           const std::string& text)
{
    std::vector<std::string> components = GetVertexAttributes(text);

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

    if(m_attributes.size() != m_vertexDesc.InputParameters)
    {
        return static_cast<sstream&>(sstream() << " Created " << m_attributes.size() 
            << ", expected " << m_vertexDesc.InputParameters << " attributes").str();
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
        m_vsBlob->GetBufferPointer(), m_vsBlob->GetBufferSize(), &m_layout)))
    {
        return "Could not create input layout";
    }

    delete [] vertexDescription;
    return std::string();
}

std::string DxShader::CreateConstantBuffers(ID3D11Device* device, 
                                            const std::string& text)
{
    std::string errorBuffer = CreateConstantBuffers(device, text, true);
    if(!errorBuffer.empty())
    {
        return VS + errorBuffer;
    }

    errorBuffer = CreateConstantBuffers(device, text, false);
    if(!errorBuffer.empty())
    {
        return PS + errorBuffer;
    }

    std::vector<std::string> components;
    boost::split(components, text, boost::is_any_of("\n "), boost::token_compress_on);

    const int bufferCount = std::count(components.begin(), 
        components.end(), HLSL_CONSTANT_BUFFER);

    if(bufferCount != static_cast<int>(m_cbuffers.size()))
    {
        Logger::LogInfo(static_cast<sstream&>(sstream() 
            << m_name << " cbuffers: Created " << m_cbuffers.size() 
            << ", expected " << bufferCount).str());
    }

    return errorBuffer;
}

std::string DxShader::CreateConstantBuffers(ID3D11Device* device, 
                                            const std::string& text,
                                            bool isVertexShader)
{
    std::string errorBuffer;
    const auto& description = isVertexShader ? m_vertexDesc : m_pixelDesc;

    for(UINT i = 0; i < description.ConstantBuffers; ++i)
    {
        errorBuffer = CreateConstantBuffer(device, text, i, isVertexShader);
        if(!errorBuffer.empty())
        {
            return errorBuffer;
        }
    }
    return errorBuffer;
}

std::string DxShader::CreateConstantBuffer(ID3D11Device* device,
                                           const std::string& text,
                                           int index, 
                                           bool isVertexBuffer)
{
    ID3D11ShaderReflection* reflection = 
        isVertexBuffer ? m_vsReflection : m_psReflection;

    ID3D11ShaderReflectionConstantBuffer* cbuffer = 
        reflection->GetConstantBufferByIndex(index);

    D3D11_SHADER_BUFFER_DESC bufferDesc;
    if(!cbuffer || FAILED(cbuffer->GetDesc(&bufferDesc)))
    {
        return std::string();
    }

    m_cbuffers.push_back(std::unique_ptr<ConstantBuffer>(new ConstantBuffer()));
    auto& buffer = *m_cbuffers[m_cbuffers.size()-1];
    buffer.name = bufferDesc.Name;
    buffer.isVertexBuffer = isVertexBuffer;
    buffer.startSlot = 0;

    const int bufferIndex = text.find(HLSL_CONSTANT_BUFFER + " " + buffer.name);
    const int registerIndex = text.find(HLSL_CONSTANT_REGISTER, bufferIndex);
    if(registerIndex == NO_INDEX || bufferIndex == NO_INDEX)
    {
        return "Could not find buffer " + buffer.name + " register";
    }

    // Assumes registers 0->9 are only used
    buffer.startSlot = boost::lexical_cast<int>(
        text[registerIndex + HLSL_CONSTANT_REGISTER.size()]);

    for(UINT i = 0; i < bufferDesc.Variables; ++i)
    {
        ID3D11ShaderReflectionVariable* value = cbuffer->GetVariableByIndex(i);

        if(!value)
        {
            return "Could not get buffer " + buffer.name + 
                " variable " + boost::lexical_cast<std::string>(i);
        }

        D3D11_SHADER_VARIABLE_DESC valueDesc;
        if(FAILED(value->GetDesc(&valueDesc)))
        {
            return "Could not get buffer " + buffer.name + " variable " + 
                boost::lexical_cast<std::string>(i) + " description";
        }

        const int count = valueDesc.Size/sizeof(float);
        auto itr = FLOAT_COMPONENTS.left.find(count);
        if(itr == FLOAT_COMPONENTS.left.end())
        {
            return "Buffer " + buffer.name + " variable " + valueDesc.Name + 
                " has float components (" + boost::lexical_cast<std::string>(count) + 
                ") that does not match up to known structure ";
        }

        buffer.constants[valueDesc.Name].type = itr->second;
        buffer.constants[valueDesc.Name].index = valueDesc.StartOffset / sizeof(float);
    }

    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = bufferDesc.Size;
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    buffer.scratch.resize(bufferDesc.Size/sizeof(float));
    buffer.scratch.assign(buffer.scratch.size(), 0.0f);
    
    if(FAILED(device->CreateBuffer(&bd, 0, &buffer.buffer)))
    {
        return "Constant buffer " + buffer.name + " creation failed";
    }

    return std::string();
}

void DxShader::SetDebugNames()
{
    SetDebugName(m_vs, m_name + "_Vertex");
    SetDebugName(m_ps, m_name + "_Pixel");
    SetDebugName(m_layout, m_name + "_Layout");
    SetDebugName(m_samplerState, m_name + "_Sampler");

    for(const auto& constantBuffer : m_cbuffers)
    {
        SetDebugName(constantBuffer->buffer, 
            m_name + "_" + constantBuffer->name);
    }
}

void DxShader::SetActive(ID3D11DeviceContext* context)
{
    context->VSSetShader(m_vs, 0, 0);
    context->PSSetShader(m_ps, 0, 0);
    context->IASetInputLayout(m_layout);

    for(auto& cbuffer : m_cbuffers)
    {
        if(cbuffer->isVertexBuffer)
        {
            context->VSSetConstantBuffers(cbuffer->startSlot, 1, &cbuffer->buffer);
        }
        else
        {
            context->PSSetConstantBuffers(cbuffer->startSlot, 1, &cbuffer->buffer);
        }
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

    for(auto& buffer : m_cbuffers)
    {
        auto itr = buffer->constants.find(name);
        if(itr != buffer->constants.end() && 
            CanSendConstant(floatType, itr->second.type, name))
        {
            for(int i = 0; i < size; ++i)
            {
                buffer->scratch[itr->second.index + i] = value[i];
            }

            buffer->updated = true;
            return;
        }
    }
}

void DxShader::UpdateConstantMatrix(const std::string& name, const D3DXMATRIX& matrix)
{
    for(auto& buffer : m_cbuffers)
    {
        auto itr = buffer->constants.find(name);
        if(itr != buffer->constants.end() && 
            CanSendConstant(HLSL_MAT4, itr->second.type, name))
        {
            const int scratchIndex = itr->second.index;
            const FLOAT* matArray = matrix;
            for(int i = 0; i < 16; ++i) // 16 floats in a directx matrix
            {
                buffer->scratch[scratchIndex + i] = matArray[i];
            }

            buffer->updated = true;
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
    for(auto& cbuffer : m_cbuffers)
    {
        if(cbuffer->updated)
        {
            cbuffer->updated = false;
            context->UpdateSubresource(cbuffer->buffer, 
                0, 0, &cbuffer->scratch[0], 0, 0);
        }
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