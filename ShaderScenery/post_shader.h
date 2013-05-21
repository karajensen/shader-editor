/****************************************************************
* Kara Jensen (KaraPeaceJensen@gmail.com) 2012
* Post processing shaders
*****************************************************************/
#pragma once

#include "shader.h"

class PostShader : public Shader
{
public:

    ~PostShader();
    PostShader();

    /**
    * Sets the constants for the shader
    */
    virtual void OnSetConstants(IMaterialRendererServices* services, s32 userData) override;
};