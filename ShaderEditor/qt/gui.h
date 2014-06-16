////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - gui.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ui_gui.h"

/**
* Temporary test widget for the GUI
*/
class Gui : public QWidget
{
    Q_OBJECT

public:

    /**
    * Constructor
    * @param parent The parent widget of this one
    */
    Gui(QWidget* parent = nullptr);

private slots:

    /**
    * On Input Spin Box1 value changed
    * @param value The new value to change to
    */
    void on_inputSpinBox1_valueChanged(int value);

    /**
    * On Input Spin Box1 value changed
    * @param value The new value to change to
    */
    void on_inputSpinBox2_valueChanged(int value);

private:

    Ui::Gui ui; ///< Internal Qt UI for this widget
};
