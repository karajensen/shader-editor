////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - tweaker.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <memory>
#include <unordered_map>
#include "tweakable.h"
#include "tweakable_value.h"
#include "tweakable_box.h"
#include "tweakable_button.h"
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
    * @param timer The time passed in seconds from start
    */
    void SetTimer(const std::string& timer);

    /**
    * Sets the readonly tweak entry
    * @param fps The frames per second for the application
    */
    void SetFramesPerSec(const std::string& fps);

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
    * Sets the distance depth of field will be active
    * @param value The distance depth of field will be active
    */
    void SetDOFDistance(float value);

    /**
    * Sets how quickly depth of field blends into the scene
    * @param value How quickly depth of field blends into the scene
    */
    void SetDOFFade(float value);

    /**
    * Sets the amount of blurring on the scene
    * @param value The amount of blurring on the scene
    */
    void SetBlurAmount(float value);

    /**
    * Sets the step between samples for blurring
    * @param value The amount of step between samples
    */
    void SetBlurStep(float value);

    /**
    * Sets the amount of overall glow for the scene
    * @param value The amount of glow on the scene
    */
    void SetGlowAmount(float value);

    /**
    * Sets the amount contrast in the final scene
    * @param value The amount contrast in the final scene
    */
    void SetContrast(float value);

    /**
    * Sets the amount saturation in the final scene
    * @param value The amount saturation in the final scene
    */
    void SetSaturation(float value);

    /**
    * Sets the value for an attribute of fog
    * @param attribute The type of attribute to set
    * @param value The value to set for the attribute
    */
    void SetFog(FogAttribute attribute, float value);

    /**
    * Sets the value for an attribute of a light
    * @param attribute The type of attribute to set
    * @param value The value to set for the attribute
    */
    void SetLight(LightAttribute attribute, float value);

    /**
    * Sets the value for an attribute of the camera
    * @param attribute The type of attribute to set
    * @param value The value to set for the attribute
    */
    void SetCamera(CameraAttribute attribute, float value);

    /**
    * Sets the value for an attribute of a mesh
    * @param attribute The type of attribute to set
    * @param value The value to set for the attribute
    */
    void SetMesh(MeshAttribute attribute, float value);

    /**
    * Sets the value for an attribute of a wave
    * @param attribute The type of attribute to set
    * @param value The value to set for the attribute
    */
    void SetWave(WaveAttribute attribute, float value);

    /**
    * Sets the value for an attribute of an emitter
    * @param attribute The type of attribute to set
    * @param value The value to set for the attribute
    */
    void SetEmitter(EmitterAttribute attribute, float value);

    /**
    * Sets the value for an attribute of a the maximum colour rante
    * @param attribute The type of attribute to set
    * @param value The value to set for the attribute
    */
    void SetMaximumColour(ColourAttribute attribute, float value);

    /**
    * Sets the value for an attribute of a the minimum colour rante
    * @param attribute The type of attribute to set
    * @param value The value to set for the attribute
    */
    void SetMinimumColour(ColourAttribute attribute, float value);

    /**
    * Sets the value for an attribute of water
    * @param attribute The type of attribute to set
    * @param value The value to set for the attribute
    */
    void SetWater(WaterAttribute attribute, float value);

    /**
    * Sets the readonly selected mesh shader name
    * @param name The name of the shader
    */
    void SetMeshShaderName(const std::string& name);

    /**
    * Sets the selected engine
    * @param selected The selected engine
    */
    void SetSelectedEngine(int selected);

    /**
    * Sets the selected post map
    * @param selected The selected post map
    */
    void SetSelectedPostMap(int selected);

    /**
    * Sets the selected emitter
    * @param selected The selected emitter
    */
    void SetSelectedEmitter(int selected);

    /**
    * Sets the available post maps for the combo box
    * @param selected The initially selected post map
    * @param engines The post map names to set
    */
    void InitialisePostMaps(int selected, const std::vector<std::string>& maps);

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
    * Sets the available water meshes for the combo box
    * @param selected The initially selected mesh
    * @param meshes The mesh names to set
    */
    void InitialiseWater(int selected, const std::vector<std::string>& water);

    /**
    * Sets the available lights for the combo box
    * @param selected The initially selected light
    * @param lights The light names to set
    */
    void InitialiseLights(int selected, const std::vector<std::string>& lights);

    /**
    * Sets the available emitters for the combo box
    * @param selected The initially selected emitter
    * @param emitters The emitter names to set
    */
    void InitialiseEmitters(int selected, const std::vector<std::string>& emitters);

    /**
    * Sets the available waves to be tweaked
    * @param amount The amount of waves to tweak
    */
    void SetWaveAmount(int amount);

    /**
    * Sets how many particles will spawn on the selected emitter
    * @param amount The amount of particles to spawn
    */
    void SetParticleAmount(int amount);

    /**
    * @return whether the render engine combo box is filled int
    */
    bool HasEngines() const;

    /**
    * @return whether the post map combo box is filled in
    */
    bool HasPostMaps() const;

    /**
    * @return whether the mesh combo box is filled in
    */
    bool HasMeshes() const;

    /**
    * @return whether the lights combo box is filled in
    */
    bool HasLights() const;

    /**
    * @return whether the emitters combo box is filled in
    */
    bool HasEmitters() const;

    /**
    * @return whether the water combo box is filled in
    */
    bool HasWater() const;

    /**
    * @return the name of the selected tab page
    */
    std::string GetSelectedPage() const;

private:

    TweakableValue m_DOFFade;            ///< How quickly depth of field blurs into scene
    TweakableValue m_DOFDistance;        ///< Distance depth of field will start
    TweakableValue m_contrast;           ///< Contrast controller for the final scene
    TweakableValue m_saturation;         ///< Saturation controller for the final scene
    TweakableValue m_glowAmount;         ///< Amount to glow the scene by
    TweakableValue m_blurAmount;         ///< Amount to blur the scene by
    TweakableValue m_blurStep;           ///< Step between samples for blurring
    TweakableValue m_depthNear;          ///< Tweakable depth near value
    TweakableValue m_depthFar;           ///< Tweakable depth far value
    TweakableValue m_particleAmount;     ///< The amount of particles to spawn
    TweakableValue m_selectedWave;       ///< Dial for selecting the wave number

    TweakableButton m_lightDiag;         ///< Button to toggle light diagnostics
    TweakableButton m_reloadScene;       ///< Button to reload the application
    TweakableButton m_saveScene;         ///< Button to save all scene to xml
    TweakableButton m_savePost;          ///< Button to save all post processing to xml
    TweakableButton m_pauseEmission;     ///< Button to pause the selected emitter

    TweakableBox m_postMap;              ///< Combo box for selecting the post map
    TweakableBox m_renderEngine;         ///< Combo box for selecting the render engine
    TweakableBox m_selectedLight;        ///< Combo box for selecting the light
    TweakableBox m_selectedMesh;         ///< Combo box for selecting the mesh
    TweakableBox m_selectedWater;        ///< Combo box for selecting the water
    TweakableBox m_selectedEmitter;      ///< Combo box for selecting the emitter

    TweakableBoxedValue m_minColour;     ///< Colour ranges for RGB
    TweakableBoxedValue m_maxColour;     ///< Colour ranges for RGB
    TweakableBoxedValue m_fog;           ///< Tweakable values for fog
    TweakableBoxedValue m_mesh;          ///< Tweakable values for mesh attributes
    TweakableBoxedValue m_water;         ///< Tweakable values for water attributes
    TweakableBoxedValue m_wave;          ///< Tweakable values for wave attributes
    TweakableBoxedValue m_emitter;       ///< Tweakable values for emitter attributes
    TweakableBoxedValue m_emitterColour; ///< Tweakable values for emitter colour attributes
    TweakableBoxedValue m_emitterMinMax; ///< Tweakable values for emitter min/max attributes
    TweakableBoxedValue m_lightDiffuse;  ///< Tweakable values for light diffuse colour
    TweakableBoxedValue m_lightSpecular; ///< Tweakable values for light specular colour

    std::unordered_map<CameraAttribute, TweakableValue> m_camera; ///< Camera Tweakable values
    std::unordered_map<LightAttribute, TweakableValue> m_light;   ///< Light tweakable values;

    Ui::Tweaker m_ui;                    ///< User interface object
    SignalCallbacks m_callbacks;         ///< Callbacks to update the cache
};
