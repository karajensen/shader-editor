////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - normalshader.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "shader.h"

/**
* Normal-depth mapping shader
*/
class NormalShader : public Shader
{
public:

    /**
    * Constructor
    * @param engine The Irrlicht engine
    */
    explicit NormalShader(EnginePtr engine);

    /**
    * Sets the constants for the shader 
    * @param services Interface providing methods for communicating with shaders
    * @param userData User defined int specific on shader creation
    */
    virtual void OnSetConstants(IMaterialRendererServices* services, s32 userData) override;
};