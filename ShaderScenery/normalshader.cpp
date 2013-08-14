////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - normalshader.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "normalshader.h"
#include <boost/algorithm/string.hpp>

NormalShader::NormalShader(EnginePtr engine) :
    Shader(engine)
{
}

void NormalShader::OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
{
    Shader::OnSetConstants(services, userData);
    ISceneManager* scene = m_engine->scene;

    float near = scene->getActiveCamera()->getNearValue();
    services->setPixelShaderConstant("FrustumNear", &near, 1);

    float far = scene->getActiveCamera()->getFarValue();
    services->setPixelShaderConstant("FrustumFar", &far, 1);
}