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
* Allows run time editing of the scene
* @note Read by both VS and Qt Editor
*/
class Tweaker : public QWidget
{
    Q_OBJECT

public:

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

private slots: // Any changes to slots requires building of moc

    /**
    * Sends an update for the light position x component
    * @param value The x component
    */
    void UpdateLightPositionX(double value);

    /**
    * Sends an update for the light position y component
    * @param value The y component
    */
    void UpdateLightPositionY(double value);

    /**
    * Sends an update for the light position z component
    * @param value The zs component
    */
    void UpdateLightPositionZ(double value);

private:

    bool m_lightPositionSet;       ///< Whether the light position has been set
    Ui::Tweaker m_ui;              ///< User interface object
    SignalCallbacks m_callbacks;   ///< Callbacks to update the cache
};
