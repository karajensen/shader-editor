/****************************************************************
* Kara Jensen (KaraPeaceJensen@gmail.com) 2012
* Post processing shaders
*****************************************************************/
#pragma once

#include <array>
#include "shader.h"

class PostShader : public Shader
{
public:

    /**
    * Avaliable output textures
    * Toggling all off will result in post processed scene
    */
    enum OutputTexture
    {
        DIFFUSE_MAP,
        NORMAL_MAP,
        DEPTH_MAP,
        MAX_TEXTURES
    };

    /**
    * Sets the constants for the shader
    */
    virtual void OnSetConstants(IMaterialRendererServices* services, s32 userData) override;

    /**
    * Sets the visiblity of the output texture
    * @param the texture
    * @param whether the texture is visible or not
    */
    static void SetTextureVisibility(unsigned int texture, bool visible);

    /**
    * @return a string description of the output texture
    */
    static stringw GetTextureDescription(unsigned int texture);

private:

    static std::array<float, MAX_TEXTURES> sm_outputTextures;  ///< which textures are currently toggled on/off

};