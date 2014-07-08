////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - tweaker.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <memory>
#include <functional>

#ifdef _VS
#include "generated/ui_tweaker.h"
#else
#include "ui_tweaker.h"
#endif

/**
* Holds functions called when a tweakable value emits a signal
*/
struct SignalCallbacks
{
    std::function<void(float)> SetLightPositionX;   ///< Sets the selected light position
    std::function<void(float)> SetLightPositionY;   ///< Sets the selected light position
    std::function<void(float)> SetLightPositionZ;   ///< Sets the selected light position
    std::function<void(float)> SetLightAttX;        ///< Sets the selected light attenuation
    std::function<void(float)> SetLightAttY;        ///< Sets the selected light attenuation
    std::function<void(float)> SetLightAttZ;        ///< Sets the selected light attenuation
    std::function<void(float)> SetLightDiffuseR;    ///< Sets the selected light diffuse color
    std::function<void(float)> SetLightDiffuseG;    ///< Sets the selected light diffuse color
    std::function<void(float)> SetLightDiffuseB;    ///< Sets the selected light diffuse color
    std::function<void(float)> SetLightDiffuseA;    ///< Sets the selected light diffuse color
    std::function<void(float)> SetLightSpecularR;   ///< Sets the selected light specular color
    std::function<void(float)> SetLightSpecularG;   ///< Sets the selected light specular color
    std::function<void(float)> SetLightSpecularB;   ///< Sets the selected light specular color
    std::function<void(float)> SetLightSpecularA;   ///< Sets the selected light specular color
    std::function<void(float)> SetLightSpecularity; ///< Sets the selected light specular size
};

/**
* Tweakable widget that holds a double spin box and dial
* @note Any changes to slots requires building of moc
*/
class Tweakable : public QWidget
{
    Q_OBJECT

public:

    /**
    * Constructor
    */
    Tweakable();

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
* Allows run time editing of the scene
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
    * @return the name of the selected tab page
    */
    std::string GetSelectedPage() const;

    /**
    * Sets the readonly tweak entry 'deltaTime'
    * @param dt The time passed in seconds between ticks
    */
    void SetDeltaTime(const std::string& dt);

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
    * Sets the light position
    * @param x The x component of the light position
    * @param y The y component of the light position
    * @param z The z component of the light position
    */
    void SetLightPosition(float x, float y, float z);

    /**
    * @return whether the light position has been previously set
    */
    bool LightPositionSet() const;

    /**
    * Sets the light attenuation
    * @param x The x component of the light attenuation
    * @param y The y component of the light attenuation
    * @param z The z component of the light attenuation
    */
    void SetLightAttenuation(float x, float y, float z);

    /**
    * @return whether the light attenuation has been previously set
    */
    bool LightAttenuationSet() const;

private:

    Tweakable m_lightPositionX;     ///< Tweakable x component of the light position
    Tweakable m_lightPositionY;     ///< Tweakable y component of the light position
    Tweakable m_lightPositionZ;     ///< Tweakable z component of the light position
    Tweakable m_lightAttenuationX;  ///< Tweakable x component of the light attenuation
    Tweakable m_lightAttenuationY;  ///< Tweakable y component of the light attenuation
    Tweakable m_lightAttenuationZ;  ///< Tweakable z component of the light attenuation
    Ui::Tweaker m_ui;               ///< User interface object
    SignalCallbacks m_callbacks;    ///< Callbacks to update the cache
};
