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
    enum EditableComponent
    {
        FOG_VISIBILITY,
        SSAO_VISIBILITY,
        GLOW_VISIBILITY,
        DOF_VISIBILITY,
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
    * Sets which output texture is shown by the post shader
    * @param texture The texture to make visible
    */
    void SetVisibleTexture(OutputTexture texture);

    /**
    * @param texture The output texture to get a description for
    * @return a string description of the output texture
    */
    std::string GetTextureDescription(OutputTexture texture);

    /**
    * Sets the visiblity of the editable component
    * @param component The component to set the visibility
    * @param value The level of visibilty of the component
    */
    void SetComponentVisibility(EditableComponent component, float value);

    /**
    * @param texture The output texture to get the visibility for
    * @return the visibility of the given texture
    */
    float GetTextureVisibility(OutputTexture texture);

    /**
    * @return a string description of the editable component
    */
    std::string GetComponentDescription(EditableComponent component);

    /**
    * Allows diagnostics to use the editable components in a tweak bar
    * @param component The component to access
    * @return a the address of the editable component
    */
    void* GetComponentAddress(EditableComponent component);

private:

    std::array<float, MAX_EDITABLE> m_editableComponents; ///< which components are currently toggled
    std::array<float, MAX_TEXTURES> m_outputTextures;     ///< which textures are currently toggled
};
