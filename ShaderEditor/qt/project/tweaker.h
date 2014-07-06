////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - tweaker.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <memory>

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
    * Constructor
    * @param parent The owner of this widget
    */
    Tweaker(QWidget* parent = nullptr);

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

private slots:


private:

    bool m_lightPositionSet; ///< Whether the light position has been set
    Ui::Tweaker m_ui;        ///< User interface object
};
