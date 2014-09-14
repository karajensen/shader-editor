////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - tweaker.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <memory>
#include "signals.h"
#include "tweakable_value.h"
#include "tweakable_box.h"
#include "tweakable_boxed_value.h"

#ifdef _VS
#include "generated/ui_tweaker.h"
#else
#include "ui_tweaker.h"
#endif

/**
* Allows run time editing of the scene
* @note Any changes to slots requires building of moc
* @note Read by both VS and Qt Editor
*/
class Tweaker : public QWidget
{
    Q_OBJECT

public:

    /**
    * Constructor
    * @note called by qt designer
    * @param parent The owner of this widget
    */
    explicit Tweaker(QWidget* parent = nullptr);

    /**
    * Constructor
    * @note called by shader editor application
    * @param callbacks Functions called when a tweakable value emits a signal
    * @param parent The owner of this widget
    */
    explicit Tweaker(const SignalCallbacks& callbacks, QWidget* parent = nullptr);

    /**
    * Sets the readonly tweak entry
    * @param dt The time passed in seconds between ticks
    */
    void SetDeltaTime(const std::string& dt);

    /**
    * Sets the readonly tweak entry
    * @param fps The frames per second for the application
    */
    void SetFramesPerSec(const std::string& fps);

    /**
    * Sets the position for the mouse
    * @param x The x screen position of the mouse
    * @param y The y screen position of the mouse
    */
    void SetMousePosition(const std::string& x, const std::string& y);

    /**
    * Sets the direction for the mouse
    * @param x The x direction normalized of the mouse
    * @param y The y direction normalized of the mouse
    */
    void SetMouseDirection(const std::string& x, const std::string& y);

    /**
    * Sets the position for the camera
    * @param x The x world position of the camera
    * @param y The y world position of the camera
    * @param z The z world position of the camera
    */
    void SetCameraPosition(const std::string& x, 
                           const std::string& y, 
                           const std::string& z);

    /**
    * Sets the near value for the camera
    * @param value The near value for the camera
    */
    void SetDepthNear(float value);

    /**
    * Sets the far value for the camera
    * @param value The far value for the camera
    */
    void SetDepthFar(float value);

    /**
    * Sets the readonly selected mesh shader name
    * @param name The name of the shader
    */
    void SetMeshShaderName(const std::string& name);

    /**
    * Sets the light position
    * @param x The x component of the light position
    * @param y The y component of the light position
    * @param z The z component of the light position
    */
    void SetLightPosition(float x, float y, float z);

    /**
    * Sets the light attenuation
    * @param x The x component of the light attenuation
    * @param y The y component of the light attenuation
    * @param z The z component of the light attenuation
    */
    void SetLightAttenuation(float x, float y, float z);

    /**
    * Sets the light diffuse color
    * @param r The r component of the light diffuse colour
    * @param g The g component of the light diffuse colour
    * @param b The b component of the light diffuse colour
    */
    void SetLightDiffuse(float r, float g, float b);

    /**
    * Sets the light specular color
    * @param r The r component of the light specular colour
    * @param g The g component of the light specular colour
    * @param b The b component of the light specular colour
    */
    void SetLightSpecular(float r, float g, float b);

    /**
    * Sets the light specular size
    * @param size The size of the specular highlights
    */
    void SetLightSpecularity(float size);

    /**
    * Sets the mesh specular size
    * @param size The size of the specular highlights
    */
    void SetMeshSpecularity(float size);

    /**
    * Sets the mesh ambience
    * @param value The ambient value of the mesh
    */
    void SetMeshAmbience(float value);

    /**
    * Sets the mesh bump saturation
    * @param value The bump value of the mesh
    */
    void SetMeshBump(float value);

    /**
    * Sets the mesh glow intensity
    * @param value The intensity of the glow of the mesh
    */
    void SetMeshGlow(float value);

    /**
    * Sets the selected engine
    * @param selected The selected engine
    */
    void SetSelectedEngine(int selected);

    /**
    * Sets the values used for the minimum colour range
    * @param rgb The red, green and blue mininum colour range
    */
    void SetMinimumColour(float r, float g, float b);

    /**
    * Sets the values used for the maximum colour range
    * @param rgb The red, green and blue maximum colour range
    */
    void SetMaximumColour(float r, float g, float b);

    /**
    * Sets the available post textures for the combo box
    * @param selected The initially selected post texture
    * @param engines The post texture names to set
    */
    void InitialiseTextures(int selected, const std::vector<std::string>& textures);

    /**
    * Sets the available render engines for the combo box
    * @param selected The initially selected engine
    * @param engines The render engine names to set
    */
    void InitialiseEngines(int selected, const std::vector<std::string>& engines);

    /**
    * Sets the available meshes for the combo box
    * @param selected The initially selected mesh
    * @param meshes The mesh names to set
    */
    void InitialiseMeshes(int selected, const std::vector<std::string>& meshes);

    /**
    * Sets the available lights for the combo box
    * @param selected The initially selected light
    * @param lights The light names to set
    */
    void InitialiseLights(int selected, const std::vector<std::string>& lights);

    /**
    * @return whether the render engine combo box is filled int
    */
    bool HasEngines() const;

    /**
    * @return whether the post texture combo box is filled int
    */
    bool HasPostTextures() const;

    /**
    * @return whether the mesh combo box is filled int
    */
    bool HasMeshes() const;

    /**
    * @return whether the mesh combo box is filled int
    */
    bool HasLights() const;

    /**
    * @return the name of the selected tab page
    */
    std::string GetSelectedPage() const;

private:

    TweakableValue m_depthNear;          ///< Tweakable depth near value
    TweakableValue m_depthFar;           ///< Tweakable depth far value
    TweakableValue m_lightPositionX;     ///< Tweakable x of the light position
    TweakableValue m_lightPositionY;     ///< Tweakable y of the light position
    TweakableValue m_lightPositionZ;     ///< Tweakable z of the light position
    TweakableValue m_lightAttenuationX;  ///< Tweakable x of the light attenuation
    TweakableValue m_lightAttenuationY;  ///< Tweakable y of the light attenuation
    TweakableValue m_lightAttenuationZ;  ///< Tweakable z of the light attenuation
    TweakableValue m_lightDiffuseR;      ///< Tweakable r of the light diffuse colour
    TweakableValue m_lightDiffuseG;      ///< Tweakable g of the light diffuse colour
    TweakableValue m_lightDiffuseB;      ///< Tweakable b of the light diffuse colour
    TweakableValue m_lightSpecularR;     ///< Tweakable r of the light specular colour
    TweakableValue m_lightSpecularG;     ///< Tweakable g of the light specular colour
    TweakableValue m_lightSpecularB;     ///< Tweakable b of the light specular colour
    TweakableValue m_lightSpecularity;   ///< Tweakable specular size of the light
    TweakableValue m_meshSpecularity;    ///< Tweakable specular size of the mesh
    TweakableValue m_meshAmbience;       ///< Tweakable ambience of the mesh
    TweakableValue m_meshBump;           ///< Tweakable bump saturation of the mesh
    TweakableValue m_meshGlow;           ///< Tweakable glow intensity of the mesh

    TweakableBox m_postTexture;          ///< Combo box for selecting the post texture
    TweakableBox m_renderEngine;         ///< Combo box for selecting the render engine
    TweakableBox m_light;                ///< Combo box for selecting the light
    TweakableBox m_mesh;                 ///< Combo box for selecting the mesh

    TweakableBoxedValue m_minColour;     ///< Colour ranges for RGB
    TweakableBoxedValue m_maxColour;     ///< Colour ranges for RGB

    Ui::Tweaker m_ui;               ///< User interface object
    SignalCallbacks m_callbacks;    ///< Callbacks to update the cache
};
