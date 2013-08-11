////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com
////////////////////////////////////////////////////////////////////////////////////////

#include "postshader.h"
#include <boost/algorithm/string.hpp>

PostShader::PostShader(EnginePtr engine) :
    Shader(engine)
{
}

void PostShader::OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
{
    Shader::OnSetConstants(services, userData);

    services->setPixelShaderConstant("OutputTexture", &m_outputTextures[0], MAX_TEXTURES);
    services->setPixelShaderConstant("ComponentVisibility", &m_editableComponents[0], MAX_EDITABLE);
}

void PostShader::SetVisibleTexture(unsigned int texture)
{
    m_outputTextures.assign(0.0f);
    m_outputTextures[texture] = 1.0f;
}

void PostShader::SetComponentVisibility(unsigned int component, float value)
{
    m_editableComponents[component] = value;
}

stringw PostShader::GetComponentDescription(unsigned int component)
{
    switch(component)
    {
    case SSAO_VIS:
        return "AMBIENT OCC.";
    case FOG_VIS:
        return "FOG";
    case DOF_VIS:
        return "DEPTH OF FIELD";
    case GLOW_VIS:
        return "GLOW";
    default:
        return "NONE";
    };
}

stringw PostShader::GetTextureDescription(unsigned int texture)
{
    switch(texture)
    {
    case POST_MAP:
        return "POST MAP";
    case DIFFUSE_MAP:
        return "DIFFUSE MAP";
    case NORMAL_MAP:
        return "NORMAL MAP";
    case DEPTH_MAP:
        return "DEPTH MAP";
    default:
        return "NONE";
    };
}