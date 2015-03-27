////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directxshader.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "directxshader.h"
#include "directxtarget.h"
#include "boost/algorithm/string.hpp"
#include "boost/regex.hpp"
#include "boost/bimap.hpp"
#include "boost/lexical_cast.hpp"
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
    const std::string PIXEL_RETURN("Outputs");
    const std::string VERTEX_ENTRY("VShader");
    const std::string PIXEL_ENTRY("PShader");
    const std::string VERTEX_MODEL("vs_5_0");
    const std::string PIXEL_MODEL("ps_5_0");
    const std::string HLSL_IN_POSITION("POSITION");
}

DxShader::DxShader(const Shader& shader) :
    m_shader(shader),
    m_filepath(shader.HLSLShaderFile())
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
    m_textureSlots = 0;
    m_vertexAsm.clear();
    m_pixelAsm.clear();

    SafeRelease(&m_vsReflection);
    SafeRelease(&m_psReflection);
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

    std::string errorBuffer = LoadShaderText();
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

    errorBuffer = BindVertexAttributes(device);
    if(!errorBuffer.empty())
    {
        return VS + errorBuffer;
    }

    errorBuffer = CreateConstantBuffers(device, true);
    if(!errorBuffer.empty())
    {
        return VS + errorBuffer;
    }

    errorBuffer = CreateConstantBuffers(device, false);
    if(!errorBuffer.empty())
    {
        return PS + errorBuffer;
    }

    m_textureSlots = m_pixelDesc.BoundResources;
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

std::string DxShader::LoadShaderText()
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
    m_vertexText = std::string(text.begin() + vertexIndex, text.begin() + pixelIndex - 1);
    m_pixelText = std::string(text.begin() + pixelIndex, text.end());
    m_sharedText = std::string(text.begin(), text.begin() + vertexIndex - 1);

    return std::string();
}

std::string DxShader::GenerateAssembly()
{
    if(!m_vertexAsm.empty() && !m_pixelAsm.empty())
    {
        return std::string();
    }

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

    m_vertexAsm = static_cast<char*>(vertexAsmBlob->GetBufferPointer());
    m_pixelAsm = static_cast<char*>(pixelAsmBlob->GetBufferPointer());

    if(REMOVE_ASM_COMMENTS)
    {
        boost::regex reg("//.*?\n");
        m_vertexAsm = boost::regex_replace(m_vertexAsm, reg, "");
        m_pixelAsm = boost::regex_replace(m_pixelAsm, reg, "");
    }

    if(OUTPUT_ASM_FILE)
    {
        std::ofstream file(m_asmpath.c_str(), std::ios_base::out|std::ios_base::trunc);
        if(!file.is_open())
        {
            return "Could not open file " + m_asmpath;
        }    

        file << m_vertexAsm << std::endl << std::endl << m_pixelAsm;
        file.close();
    }

    pixelAsmBlob->Release();
    vertexAsmBlob->Release();

    return std::string();
}

int DxShader::GetAttributeCompononts(D3D11_SIGNATURE_PARAMETER_DESC& description) const
{
    enum ComponentMask
    {
        COMPONENTS_X = 1,  
        COMPONENTS_XY = 2,  
        COMPONENTS_XYZ = 4,  
        COMPONENTS_XYZW = 8
    };

    if(description.SemanticName == HLSL_IN_POSITION)
    {
        // Pass position as a vec3 into a vec4 slot to use the optimization
        // where the 'w' component is automatically set as 1.0
        return 3;
    }
    if((description.Mask & COMPONENTS_XYZW) == COMPONENTS_XYZW)
    {
        return 4;
    }
    else if((description.Mask & COMPONENTS_XYZ) == COMPONENTS_XYZ)
    {
        return 3;
    }
    else if((description.Mask & COMPONENTS_XY) == COMPONENTS_XY)
    {
        return 2;
    }
    else if((description.Mask & COMPONENTS_X) == COMPONENTS_X)
    {
        return 1;
    }
    return 0;
}

DXGI_FORMAT DxShader::GetAttributeFormat(D3D11_SIGNATURE_PARAMETER_DESC& description) const
{
    const int componentsUsed = GetAttributeCompononts(description);
    switch(componentsUsed)
    {
    case 1:
        switch(description.ComponentType)
        {
        case D3D_REGISTER_COMPONENT_UINT32:
            return DXGI_FORMAT_R32_UINT;
        case D3D_REGISTER_COMPONENT_SINT32:
            return DXGI_FORMAT_R32_SINT;
        case D3D_REGISTER_COMPONENT_FLOAT32:
            return DXGI_FORMAT_R32_FLOAT;
        }
        break;
    case 2:
        switch(description.ComponentType)
        {
        case D3D_REGISTER_COMPONENT_UINT32:
            return DXGI_FORMAT_R32G32_UINT;
        case D3D_REGISTER_COMPONENT_SINT32:
            return DXGI_FORMAT_R32G32_SINT;
        case D3D_REGISTER_COMPONENT_FLOAT32:
            return DXGI_FORMAT_R32G32_FLOAT;
        }
        break;
    case 3:
        switch(description.ComponentType)
        {
        case D3D_REGISTER_COMPONENT_UINT32:
            return DXGI_FORMAT_R32G32B32_UINT;
        case D3D_REGISTER_COMPONENT_SINT32:
            return DXGI_FORMAT_R32G32B32_SINT;
        case D3D_REGISTER_COMPONENT_FLOAT32:
            return DXGI_FORMAT_R32G32B32_FLOAT;
        }
        break;
    case 4:
        switch(description.ComponentType)
        {
        case D3D_REGISTER_COMPONENT_UINT32:
            return DXGI_FORMAT_R32G32B32A32_UINT;
        case D3D_REGISTER_COMPONENT_SINT32:
            return DXGI_FORMAT_R32G32B32A32_SINT;
        case D3D_REGISTER_COMPONENT_FLOAT32:
            return DXGI_FORMAT_R32G32B32A32_FLOAT;
        }
    }

    Logger::LogError(std::string("Could not find format for ") + description.SemanticName);
    return DXGI_FORMAT_UNKNOWN;
}

int DxShader::GetAttributeBytes(D3D11_SIGNATURE_PARAMETER_DESC& description) const
{
    switch(description.ComponentType)
    {
    case D3D_REGISTER_COMPONENT_UINT32:
        return GetAttributeCompononts(description) * sizeof(UINT);
    case D3D_REGISTER_COMPONENT_SINT32:
        return GetAttributeCompononts(description) * sizeof(INT);
    case D3D_REGISTER_COMPONENT_FLOAT32:
        return GetAttributeCompononts(description) * sizeof(FLOAT);
    default:
        Logger::LogError(std::string("Could not find size for ") + description.SemanticName);
        return 0;
    }
}

std::string DxShader::BindVertexAttributes(ID3D11Device* device)
{
    std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayout(m_vertexDesc.InputParameters);

    int byteOffset = 0;
    for(UINT i = 0; i < m_vertexDesc.InputParameters; ++i)
    {
        D3D11_SIGNATURE_PARAMETER_DESC inputDesc;
        if(FAILED(m_vsReflection->GetInputParameterDesc(i, &inputDesc)))
        {
            return "Could not get description for vertex attribute " + 
                boost::lexical_cast<std::string>(i);
        }

        inputLayout[i].AlignedByteOffset = byteOffset;
        inputLayout[i].InputSlot = 0;
        inputLayout[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        inputLayout[i].SemanticIndex = inputDesc.SemanticIndex;
        inputLayout[i].SemanticName = inputDesc.SemanticName;
        inputLayout[i].Format = GetAttributeFormat(inputDesc);
        inputLayout[i].InstanceDataStepRate = 0;

        byteOffset += GetAttributeBytes(inputDesc);
    }
    
    if(FAILED(device->CreateInputLayout(&inputLayout[0], inputLayout.size(), 
        m_vsBlob->GetBufferPointer(), m_vsBlob->GetBufferSize(), &m_layout)))
    {
        return "Could not create input layout";
    }

    return std::string();
}

std::string DxShader::CreateConstantBuffers(ID3D11Device* device, 
                                            bool isVertexShader)
{
    std::string errorBuffer;
    const auto& description = isVertexShader ? m_vertexDesc : m_pixelDesc;

    for(UINT i = 0; i < description.ConstantBuffers; ++i)
    {
        errorBuffer = CreateConstantBuffer(device, i, isVertexShader);
        if(!errorBuffer.empty())
        {
            return errorBuffer;
        }
    }
    return errorBuffer;
}

std::string DxShader::CreateConstantBuffer(ID3D11Device* device,
                                           int index, 
                                           bool isVertexBuffer)
{
    auto* reflection = isVertexBuffer ? m_vsReflection : m_psReflection;
    const auto& description = isVertexBuffer ? m_vertexDesc : m_pixelDesc;
    auto* cbuffer = reflection->GetConstantBufferByIndex(index);

    D3D11_SHADER_BUFFER_DESC bufferDesc;
    if(FAILED(cbuffer->GetDesc(&bufferDesc)))
    {
        Logger::LogInfo("CBuffer " + boost::lexical_cast<std::string>(index) + 
            " has been optimised out of shader " + m_shader.Name());
        return std::string();
    }

    m_cbuffers.push_back(std::unique_ptr<ConstantBuffer>(new ConstantBuffer()));
    auto& buffer = *m_cbuffers[m_cbuffers.size()-1];
    buffer.name = bufferDesc.Name;
    buffer.isVertexBuffer = isVertexBuffer;

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

        buffer.constants[valueDesc.Name].size = valueDesc.Size/sizeof(float);
        buffer.constants[valueDesc.Name].index = valueDesc.StartOffset / sizeof(float);
    }

    D3D11_SHADER_INPUT_BIND_DESC inputDesc;
    if(FAILED(reflection->GetResourceBindingDescByName(bufferDesc.Name, &inputDesc)))
    {
        return "Could not find input resource for " + buffer.name;
    }

    buffer.startSlot = inputDesc.BindPoint;
    buffer.scratch.resize(bufferDesc.Size/sizeof(float));
    buffer.scratch.assign(buffer.scratch.size(), 0.0f);

    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = bufferDesc.Size;
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    if(FAILED(device->CreateBuffer(&bd, 0, &buffer.buffer)))
    {
        return "Constant buffer " + buffer.name + " creation failed";
    }

    return std::string();
}

void DxShader::SetDebugNames()
{
    SetDebugName(m_vs, m_shader.Name() + "_Vertex");
    SetDebugName(m_ps, m_shader.Name() + "_Pixel");
    SetDebugName(m_layout, m_shader.Name() + "_Layout");

    for(const auto& constantBuffer : m_cbuffers)
    {
        SetDebugName(constantBuffer->buffer, 
            m_shader.Name() + "_" + constantBuffer->name);
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
}

void DxShader::SendTexture(ID3D11DeviceContext* context,
                           int slot,
                           ID3D11ShaderResourceView** view,
                           ID3D11SamplerState** state)
{
    context->PSSetShaderResources(slot, 1, view);
    context->PSSetSamplers(slot, 1, state);
}

void DxShader::SendTexture(ID3D11DeviceContext* context, 
                           int slot, 
                           const DxRenderTarget& target,
                           int ID)
{
    context->PSSetShaderResources(slot, 1, target.Get(ID));
    context->PSSetSamplers(slot, 1, target.State());
}

void DxShader::SendCopiedTexture(ID3D11DeviceContext* context, 
                                 int slot, 
                                 const DxRenderTarget& target,
                                 int ID)
{
    context->PSSetShaderResources(slot, 1, target.GetCopied(ID));
    context->PSSetSamplers(slot, 1, target.State());
}

void DxShader::ClearTexture(ID3D11DeviceContext* context,
                            int slot)
{
    ID3D11ShaderResourceView* nullView = nullptr;
    context->PSSetShaderResources(slot, 1, &nullView);
}

void DxShader::UpdateConstantFloat(const std::string& name, const float* value, int size, int offset)
{
    for(auto& buffer : m_cbuffers)
    {
        auto itr = buffer->constants.find(name);
        if(itr != buffer->constants.end())
        {
            if (offset == NO_INDEX ? itr->second.size != size : itr->second.size < size)
            {
                Logger::LogError("Size for constant " + name + " doesn't match");
                return;
            }

            offset = max(offset, 0);
            for(int i = offset, j = 0; j < size; ++i, ++j)
            {
                buffer->scratch[itr->second.index + i] = value[j];
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
        if(itr != buffer->constants.end())
        {
            const int matrixSize = 16;
            if (itr->second.size != matrixSize)
            {
                Logger::LogError("Size for constant " + name + " doesn't match");
                return;
            }

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
    return m_shader.ID();
}

bool DxShader::HasTextureSlot(int slot)
{
    return slot <= m_textureSlots;
}

std::string DxShader::GetText() const
{
    return m_sharedText + "\n" +  m_vertexText + "\n" + m_pixelText;
}

std::string DxShader::GetAssembly()
{
    const std::string errors = GenerateAssembly();
    if(!errors.empty())
    {
        Logger::LogError("DirectX: " + m_shader.Name() + " " + errors);
    }
    return m_vertexAsm + "\n" + m_pixelAsm;
}

const std::string& DxShader::GetName() const
{
    return m_shader.Name();
}