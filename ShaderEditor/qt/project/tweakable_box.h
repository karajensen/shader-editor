////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - tweakable_box.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <functional>
#include <memory>
#include <QObject>
#include <QComboBox>

/**
* Tweakable widget that holds a combo box
* @note Any changes to slots requires building of moc
* @note Read by both VS and Qt Editor
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

    /**
    * Sets the currently selected index
    * @param selected The index to select
    */
    void SetSelected(int selected);

private slots:

    /**
    * Sends an update for the selected index
    * @param index The new selected index from the combo box
    */
    void UpdateSelected(int index);

private:

    QComboBox* m_comboBox;                     ///< Combo box widget
    std::function<void(int)> m_signalCallback; ///< Function to call when index is changed
};
