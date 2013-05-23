#include "post_shader.h"
#include <boost/algorithm/string.hpp>

std::array<float, PostShader::MAX_TEXTURES> PostShader::sm_outputTextures;

void PostShader::OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
{
    Shader::OnSetConstants(services, userData);

    if(boost::icontains(m_name,"normalshader"))
    {
        ISceneManager* scene = Scene();

        float near = scene->getActiveCamera()->getNearValue();
        services->setPixelShaderConstant("FrustumNear", &near, 1);

        float far = scene->getActiveCamera()->getFarValue();
        services->setPixelShaderConstant("FrustumFar", &far, 1);
    }
    else if(boost::icontains(m_name,"postshader"))
    {
        services->setPixelShaderConstant("OutputTexture", &sm_outputTextures[0], MAX_TEXTURES);
    }
}

void PostShader::SetTextureVisibility(unsigned int texture, bool visible)
{
    sm_outputTextures.assign(0.0f);
    sm_outputTextures[texture] = (visible ? 1.0f : 0.0f);
}

stringw PostShader::GetTextureDescription(unsigned int texture)
{
    switch(texture)
    {
    case DIFFUSE_MAP:
        return "Diffuse Map";
    case NORMAL_MAP:
        return "Normal Map";
    case DEPTH_MAP:
        return "Depth Map";
    default:
        return "None";
    };
}