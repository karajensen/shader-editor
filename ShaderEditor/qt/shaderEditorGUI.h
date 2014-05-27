////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - shaderEditorGUI.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ui_shaderEditorGUI.h"

/**
* Temporary test widget for the GUI
*/
class ShaderEditorGUI : public QWidget
{
    Q_OBJECT

public:

    /**
    * Constructor
    * @param parent The parent widget of this one
    */
    ShaderEditorGUI(QWidget* parent = nullptr);

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

    Ui::ShaderEditorGUI ui; ///< Internal Qt UI for this widget
};
