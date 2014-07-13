////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - tweakable_value.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <functional>
#include <memory>
#include <QObject>
#include <QDoubleSpinBox>
#include <QDial>

/**
* Tweakable widget that holds a double spin box and dial
* @note Any changes to slots requires building of moc
* @note Read by both VS and Qt Editor
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
    * Sets the value for the tweakable widge
    */
    void Set(double value);

    /**
    * Initialises the tweakable widget
    * @param value The default value to set
    * @param step The change if value to set
    * @param box The spin box for the tweakable widget
    * @param dial The dial for the tweakable widget
    * @param signalCallback The function to call when the value changes
    */
    void Initialise(double step,
                    QDoubleSpinBox* box, 
                    QDial* dial,
                    std::function<void(float)> signalCallback);

private slots:

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
