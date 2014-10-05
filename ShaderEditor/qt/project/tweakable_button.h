////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - tweakable_button.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <functional>
#include <memory>
#include <QObject>
#include <QPushbutton.h>

/**
* Tweakable widget that holds a pressable button
* @note Any changes to slots requires building of moc
* @note Read by both VS and Qt Editor
*/
class TweakableButton : public QWidget
{
    Q_OBJECT

public:

    /**
    * Constructor
    */
    TweakableButton();

    /**
    * Initialises the tweakable widget
    * @param button The qt button object
    * @param signalCallback The function to call when the button is pushed
    */
    void Initialise(QPushButton* button,
                    std::function<void(void)> signalCallback);

private slots:

    /**
    * Sends an update for the tweakable value
    */
    void OnButtonPress();

private:

    std::function<void(void)> m_signalCallback; ///< Function to call when button is pressed
};
