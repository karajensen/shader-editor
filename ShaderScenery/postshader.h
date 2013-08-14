////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - postshader.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <array>
#include "shader.h"

/**
* Final Post processing shader
*/
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
    * Constructor
    * @param engine The Irrlicht engine
    */
    explicit PostShader(EnginePtr engine);

    /**
    * Sets the constants for the shader 
    * @param services Interface providing methods for communicating with shaders
    * @param userData User defined int specific on shader creation
    */
    virtual void OnSetConstants(IMaterialRendererServices* services, s32 userData) override;

    /**
    * Sets which output texture is shown
    * @param texture The texture to make visible
    */
    void SetVisibleTexture(unsigned int texture);

    /**
    * @return a string description of the output texture
    */
    stringw GetTextureDescription(unsigned int texture);

    /**
    * Sets the visiblity of the editable component
    * @param component The component to set the visibility
    * @param value The level of visibilty of the component
    */
    void SetComponentVisibility(unsigned int component, float value);

    /**
    * @return a string description of the editable component
    */
    stringw GetComponentDescription(unsigned int component);

private:

    std::array<float, MAX_EDITABLE> m_editableComponents; ///< which components are currently toggled
    std::array<float, MAX_TEXTURES> m_outputTextures;     ///< which textures are currently toggled
};
