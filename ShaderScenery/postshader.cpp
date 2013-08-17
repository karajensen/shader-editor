////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - postshader.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "postshader.h"
#include <boost/algorithm/string.hpp>

PostShader::PostShader(EnginePtr engine) :
    Shader(engine)
{
    m_editableComponents.assign(1.0f);
    SetVisibleTexture(POST_MAP);
}

void PostShader::OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
{
    Shader::OnSetConstants(services, userData);

    services->setPixelShaderConstant("OutputTexture", &m_outputTextures[0], MAX_TEXTURES);
    services->setPixelShaderConstant("ComponentVisibility", &m_editableComponents[0], MAX_EDITABLE);
}

void PostShader::SetVisibleTexture(PostShader::OutputTexture texture)
{
    m_outputTextures.assign(0.0f);
    m_outputTextures[texture] = 1.0f;
}

float PostShader::GetTextureVisibility(PostShader::OutputTexture texture)
{
    return m_outputTextures[texture];
}

void PostShader::SetComponentVisibility(PostShader::EditableComponent component, float value)
{
    m_editableComponents[component] = value;
}

void* PostShader::GetComponentAddress(PostShader::EditableComponent component)
{
    return &m_editableComponents[component];
}

const char* PostShader::GetComponentDescription(PostShader::EditableComponent component) const
{
    switch(component)
    {
    case SSAO_VISIBILITY:
        return "SSAO";
    case FOG_VISIBILITY:
        return "Fog";
    case DOF_VISIBILITY:
        return "DOF";
    case GLOW_VISIBILITY:
        return "Glow";
    default:
        return "NONE";
    };
}

std::string PostShader::GetTextureDescription(PostShader::OutputTexture texture)
{
    switch(texture)
    {
    case POST_MAP:
        return "Post Texture";
    case DIFFUSE_MAP:
        return "Diffuse Texture";
    case NORMAL_MAP:
        return "Normal Texture";
    case DEPTH_MAP:
        return "Depth Texture";
    default:
        return "NONE";
    };
}