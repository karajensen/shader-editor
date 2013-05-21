#include "post_shader.h"
#include <boost/algorithm/string.hpp>

PostShader::PostShader()
{
}

PostShader::~PostShader()
{
}

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
}