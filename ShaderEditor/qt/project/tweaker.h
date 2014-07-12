////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - tweaker.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <memory>
#include "signals.h"

#ifdef _VS
#include "generated/ui_tweaker.h"
#else
#include "ui_tweaker.h"
#endif

/**
* Tweakable widget that holds a double spin box and dial
* @note Any changes to slots requires building of moc
* @note Read by both VS and Qt Editor
* @note cannot subclass inside Tweaker or be in seperate file
*/
class TweakableValue : public QWidget
{
    Q_OBJECT

public:

    /**
    * Constructor
    */
    TweakableValue();

    /**
    * Initialises the tweakable widget
    * @param value The default value to set
    * @param step The change if value to set
    * @param box The spin box for the tweakable widget
    * @param dial The dial for the tweakable widget
    * @param signalCallback The function to call when the value changes
    */
    void Initialise(double value, double step,
        QDoubleSpinBox* box, QDial* dial,
        std::function<void(float)> signalCallback);

    /**
    * @return whether this tweakable is initialised or not
    */
    bool IsInitialised() const;

public slots:

    /**
    * Sends an update for the tweakable value
    * @param value The value changed to
    */
    void UpdateValue(double value);

    /**
    * Sends an update when the dial value is changed
    * @param value The value the dial has changed to
    */
    void DialValue(int value);

private:

    int m_previousDialValue;  ///< The previous value of the dial
    QDoubleSpinBox* m_box;    ///< The spin box used to edit/display the value
    std::function<void(float)> m_signalCallback; ///< Function to call when value is changed
};

/**
* Tweakable widget that holds a combo box
* @note Any changes to slots requires building of moc
* @note Read by both VS and Qt Editor
* @note cannot subclass inside Tweaker or be in seperate file
*/
class TweakableBox : public QWidget
{
    Q_OBJECT

public:

    /**
    * Constructor
    */
    TweakableBox();

    /**
    * Initialises the tweakable box
    * @param comboBox The combo box widget
    * @param selectedItem The initial selected item
    * @param items The items to add to the box
    * @param signalCallback Function to call when index is changed
    */
    void Initialise(QComboBox* comboBox, 
                    int selectedItem,
                    const std::vector<std::string>& items,
                    std::function<void(int)> signalCallback);

    /**
    * @return whether this tweakable is initialised or not
    */
    bool IsInitialised() const;

public slots:

    /**
    * Sends an update for the selected index
    * @param index The new selected index from the combo box
    */
    void UpdateSelected(int index);

private:

    QComboBox* m_comboBox;                     ///< Combo box widget
    std::function<void(int)> m_signalCallback; ///< Function to call when index is changed
};

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
    * @param parent The owner of this widget
    */
    explicit Tweaker(QWidget* parent = nullptr);
    
    /**
    * Sets the functions called when a tweakable value emits a signal
    */
    void SetSignalCallbacks(SignalCallbacks& callbacks);

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
    * Sets the readonly selected mesh back face culling
    * @param enable Whether the mesh uses back face culling
    */
    void SetMeshBackFaceCull(bool enable);

    /**
    * Sets the readonly selected mesh transparency
    * @param enable Whether the mesh uses transparency
    */
    void SetMeshTransparency(bool enable);

    /**
    * Sets the readonly selected mesh diffuse texture
    * @param name The name of the diffuse texture
    */
    void SetMeshDiffuseTexture(const std::string& name);

    /**
    * Sets the readonly selected mesh specular texture
    * @param name The name of the specular texture
    */
    void SetMeshSpecularTexture(const std::string& name);

    /**
    * Sets the readonly selected mesh normal texture
    * @param name The name of the normal texture
    */
    void SetMeshNormalTexture(const std::string& name);

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
    * @param x The x component of the light diffuse colour
    * @param y The y component of the light diffuse colour
    * @param z The z component of the light diffuse colour
    */
    void SetLightDiffuse(float r, float g, float b);

    /**
    * Sets the light specular color
    * @param x The x component of the light specular colour
    * @param y The y component of the light specular colour
    * @param z The z component of the light specular colour
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
    * Sets the available render engines for the combo box
    * @param selected The initially selected engine
    * @param engines The render engine names to set
    */
    void SetRenderEngines(int selected, const std::vector<std::string>& engines);

    /**
    * Sets the available meshes for the combo box
    * @param selected The initially selected mesh
    * @param meshes The mesh names to set
    */
    void SetMeshes(int selected, const std::vector<std::string>& meshes);

    /**
    * Sets the available lights for the combo box
    * @param selected The initially selected light
    * @param lights The light names to set
    */
    void SetLights(int selected, const std::vector<std::string>& lights);

    /**
    * @return whether the light diffuse has been previously set
    */
    bool LightDiffuseSet() const;

    /**
    * @return whether the light diffuse has been previously set
    */
    bool LightSpecularSet() const;

    /**
    * @return whether the light attenuation has been previously set
    */
    bool LightAttenuationSet() const;

    /**
    * @return whether the light position has been previously set
    */
    bool LightPositionSet() const;

    /**
    * @return whether the light specularity has been previously set
    */
    bool LightSpecularitySet() const;

    /**
    * @return whether the mesh specularity has been previously set
    */
    bool MeshSpecularitySet() const;

    /**
    * @return whether the render engine combo box is filled int
    */
    bool HasRenderEngines() const;

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

    TweakableBox m_renderEngine;         ///< Combo box for selecting the render engine
    TweakableBox m_light;                ///< Combo box for selecting the light
    TweakableBox m_mesh;                 ///< Combo box for selecting the mesh

    Ui::Tweaker m_ui;               ///< User interface object
    SignalCallbacks m_callbacks;    ///< Callbacks to update the cache
};
