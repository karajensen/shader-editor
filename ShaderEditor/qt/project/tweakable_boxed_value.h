////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - tweakable_boxed_value.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <functional>
#include <memory>
#include <QObject>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QDial>

/**
* Entry for the combo box
*/
struct ComboEntry
{
    /**
    * Constructor
    * @param Name Name of the entry
    * @param Step How much to change the value by
    * @param Signal The callback for when value changes
    */
    ComboEntry(const std::string& Name,
               double Step,
               std::function<void(float)> Signal);

    std::string name;                  ///< Name of the entry
    double step;                       ///< How much to change the value by
    double value;                      ///< Current value of the entry
    std::function<void(float)> signal; ///< callback for when the value is changed
};

/**
* Tweakable widget that holds a combo box with a value
* @note Any changes to slots requires building of moc
* @note Read by both VS and Qt Editor
*/
class TweakableBoxedValue : public QWidget
{
    Q_OBJECT

public:

    /**
    * Constructor
    */
    TweakableBoxedValue();

    /**
    * Initialises the tweakable box
    * @param comboBox The combo box widget
    * @param box The spin box for the tweakable widget
    * @param dial The dial for the tweakable widget
    * @param entries The items to add to the box
    */
    void Initialise(QComboBox* comboBox,
                    QDoubleSpinBox* box,
                    QDial* dial,
                    const std::vector<ComboEntry>& entries);

    /**
    * Sets the value for the index
    * @param index The index to set the value at
    * @param value The value to set
    */
    void SetValue(int index, double value);

private slots:

    /**
    * Sends an update for the selected index
    * @param index The new selected index from the combo box
    */
    void UpdateSelected(int index);

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

    int m_selectedIndex;               ///< The current selected entry
    int m_previousDialValue;           ///< The previous value of the dial
    QDoubleSpinBox* m_box;             ///< The spin box used to edit/display the value
    QDial* m_dial;                     ///< The dial used to edit the value
    QComboBox* m_comboBox;             ///< Combo box widget
    std::vector<ComboEntry> m_entries; ///< Entries for the combo box
};
