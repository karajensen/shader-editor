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
    */
    enum OutputTexture
    {
        POST_MAP,
        DIFFUSE_MAP,
        NORMAL_MAP,
        DEPTH_MAP,
        MAX_TEXTURES
    };

    /**
    * Shader Components avaliable for editing
    */
    enum ComponentVisibility
    {
        FOG_VIS,
        SSAO_VIS,
        GLOW_VIS,
        DOF_VIS,
        MAX_EDITABLE
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

    /**
    * Sets the visiblity of the editable component
    * @param the component
    * @param the level of visibilty of the component
    */
    static void SetComponentVisibility(unsigned int component, float value);

    /**
    * @return a string description of the editable component
    */
    static stringw GetComponentDescription(unsigned int component);

private:

    static std::array<float, MAX_EDITABLE> sm_editableComponents;  ///< which components are currently toggled on/off
    static std::array<float, MAX_TEXTURES> sm_outputTextures;  ///< which textures are currently toggled on/off

};