////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directxshader.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "directxshader.h"
#include "boost/algorithm/string.hpp"
#include "boost/regex.hpp"
#include "directx/include/D3Dcompiler.h"

namespace
{
    bool OUTPUT_ASM_FILE = false; ///< Whether to output assembly to a file
    const std::string VS("Vertex Shader: "); ///< Text for vertex shader diagnostics
    const std::string PS("Pixel Shader: "); ///< Text for pixel shader diagnostics

    // HLSL Keywords
    const std::string VERTEX_RETURN("VertexOutput");
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
    const std::string HLSL_CONSTANT_BUFFER("ConstantBuffer");
}

DxShader::DxShader(int index, const std::string& filepath) :
    m_filepath(filepath),
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

    std::string errorBuffer = LoaderShaderText();
    if(!errorBuffer.empty())
    {
        return errorBuffer;
    }

    errorBuffer = BindVertexAttributes(device, vsBlob);
    if(!errorBuffer.empty())
    {
        return errorBuffer;
    }

    errorBuffer = CreateConstantBuffer(device);
    if(!errorBuffer.empty())
    {
        return errorBuffer;
    }

    errorBuffer = OutputAssembly(vsBlob, psBlob);
    if(!errorBuffer.empty())
    {
        return errorBuffer;
    }

    return std::string();
}

std::string DxShader::LoaderShaderText()
{
    std::ifstream file(m_filepath.c_str(), std::ios::in|std::ios::ate|std::ios::_Nocreate);
    if(!file.is_open())
    {
        return "Could not open file " + m_filepath;
    }

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

std::string DxShader::OutputAssembly(ID3D10Blob* vs, ID3D10Blob* ps)
{
    ID3DBlob* vertexAsm = nullptr;
    D3DDisassemble(vs->GetBufferPointer(), vs->GetBufferSize(), 
        D3D_DISASM_DISABLE_DEBUG_INFO, 0, &vertexAsm);

    if(!vertexAsm)
    {
        return "Failed to generate vertex shader assembly";
    }

    ID3DBlob* pixelAsm = nullptr;
    D3DDisassemble(ps->GetBufferPointer(), ps->GetBufferSize(), 
        D3D_DISASM_DISABLE_DEBUG_INFO, 0, &pixelAsm);

    if(!pixelAsm)
    {
        return "Failed to generate pixel shader assembly";
    }

    m_vertexAsm = static_cast<char*>(vertexAsm->GetBufferPointer());
    m_pixelAsm = static_cast<char*>(pixelAsm->GetBufferPointer());

    // Strip out any comments to just get the asm
    boost::regex reg("//.*?\n");
    m_vertexAsm = boost::regex_replace(m_vertexAsm, reg, "");
    m_pixelAsm = boost::regex_replace(m_pixelAsm, reg, "");

    if(OUTPUT_ASM_FILE)
    {
        const std::string path(boost::ireplace_last_copy(m_filepath, SHADER_EXTENSION, ASM_EXTENSION));
        std::ofstream file(path.c_str(), std::ios_base::out|std::ios_base::trunc);
        if(!file.is_open())
        {
            return "Could not open file " + path;
        }    

        file << m_vertexAsm << std::endl << std::endl << m_pixelAsm;
        file.close();
    }

    pixelAsm->Release();
    vertexAsm->Release();

    return std::string();
}

std::string DxShader::BindVertexAttributes(ID3D11Device* device, 
                                           ID3D10Blob* vs)
{
    // Find a string of all text within the entry point brackets
    std::string attributeList;
    const std::string entryPoint(VERTEX_ENTRY + "(");
    int index = m_vertexText.find(entryPoint, 0) + entryPoint.size();
    while(m_vertexText[index] != ')')
    {
        attributeList += m_vertexText[index];
        ++index;
    }

    std::vector<std::string> components;
    boost::split(components, attributeList, 
        boost::is_any_of(",:;\n\r "), boost::token_compress_on);

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
            byteOffset += 12;
        }
        else if(components[index] == HLSL_FLT4)
        {
            data.byteOffset = byteOffset;
            data.format = DXGI_FORMAT_R32G32B32A32_FLOAT;
            byteOffset += 16;
        }
        else if(components[index] == HLSL_FLT2)
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

std::string DxShader::CreateConstantBuffer(ID3D11Device* device)
{
    // Generate a list of all variables in the constant buffer
    std::string constantList;
    int index = m_sharedText.find(HLSL_CONSTANT_BUFFER, 0) + HLSL_CONSTANT_BUFFER.size();
    while(m_sharedText[index] != '}')
    {
        constantList += m_sharedText[index];
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
            if(type == HLSL_MAT4)
            {
                byteWidth += slotsRemaining * 4; // fill in empty slots
                byteWidth += 64;
                slotsRemaining = 4;
            }
            else if(type == HLSL_FLT4)
            {
                byteWidth += slotsRemaining * 4; // fill in empty slots
                byteWidth += 16;
                slotsRemaining = 4;
            }
            else if(type == HLSL_FLT3)
            {
                if(slotsRemaining <= 2)
                {
                    byteWidth += slotsRemaining * 4; // can only fit in slots of 3+
                    slotsRemaining = 4;
                }
                byteWidth += 12;
                slotsRemaining -= 3;
            }
            else if(type == HLSL_FLT2)
            {
                if(slotsRemaining <= 1)
                {
                    byteWidth += slotsRemaining * 4; // can only fit in slots of 2+
                    slotsRemaining = 4;
                }
                byteWidth += 8;
                slotsRemaining -= 2;
            }
            else if(type == HLSL_FLT)
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
    std::string floatType = HLSL_FLT;
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
        CanSendConstant(HLSL_MAT4, itr->second.type, name))
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