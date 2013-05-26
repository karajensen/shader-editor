#include "post_shader.h"
#include <boost/algorithm/string.hpp>

std::array<float, PostShader::MAX_TEXTURES> PostShader::sm_outputTextures;
std::array<float, PostShader::MAX_EDITABLE> PostShader::sm_editableComponents;

PostShader::PostShader(PostShaderType type) :
    m_postShaderType(type)
{
}

void PostShader::OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
{
    Shader::OnSetConstants(services, userData);

    switch(m_postShaderType)
    {
        case NORMAL_DEPTH_MAPPING:
        {
            ISceneManager* scene = Scene();

            float near = scene->getActiveCamera()->getNearValue();
            services->setPixelShaderConstant("FrustumNear", &near, 1);

            float far = scene->getActiveCamera()->getFarValue();
            services->setPixelShaderConstant("FrustumFar", &far, 1);

            break;
        }
        case POST_PROCESSING:
        {
            services->setPixelShaderConstant("OutputTexture", &sm_outputTextures[0], MAX_TEXTURES);
            services->setPixelShaderConstant("ComponentVisibility", &sm_editableComponents[0], MAX_EDITABLE);
        }
    }
}

void PostShader::SetVisibleTexture(unsigned int texture)
{
    sm_outputTextures.assign(0.0f);
    sm_outputTextures[texture] = 1.0f;
}

void PostShader::SetComponentVisibility(unsigned int component, float value)
{
    sm_editableComponents[component] = value;
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