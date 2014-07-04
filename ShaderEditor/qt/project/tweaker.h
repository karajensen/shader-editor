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
    * Sets the coordinates for the mouse
    * @param positionX The x screen position of the mouse
    * @param positionY The y screen position of the mouse
    * @param directionX The x direction normalized of the mouse
    * @param directionY The y direction normalized of the mouse
    */
    void SetMouse(const std::string& positionX, 
                  const std::string& positionY,
                  const std::string& directionX,
                  const std::string& directionY);

private slots:


private:

    Ui::Tweaker m_ui; ///< User interface object
};
