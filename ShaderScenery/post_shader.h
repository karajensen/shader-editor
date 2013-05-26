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
    * Avaliable post shaders
    */
    enum PostShaderType
    {
        POST_PROCESSING,
        NORMAL_DEPTH_MAPPING
    };

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
    * Constructor.
    * @param the type of post shader
    */
    PostShader(PostShaderType type);

    /**
    * Sets the constants for the shader
    */
    virtual void OnSetConstants(IMaterialRendererServices* services, s32 userData) override;

    /**
    * Sets which output texture is shown
    * @param the texture to make visible
    */
    static void SetVisibleTexture(unsigned int texture);

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

    PostShaderType m_postShaderType; ///< Type of post shader

    static std::array<float, MAX_EDITABLE> sm_editableComponents; ///< which components are currently toggled on/off
    static std::array<float, MAX_TEXTURES> sm_outputTextures;     ///< which textures are currently toggled on/off

};