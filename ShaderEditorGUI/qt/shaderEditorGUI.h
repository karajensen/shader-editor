////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - shaderEditorGUI.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ui_shaderEditorGUI.h"

class ShaderEditorGUI : public QWidget
{
    Q_OBJECT

public:
    ShaderEditorGUI(QWidget *parent = 0);

private slots:
    void on_inputSpinBox1_valueChanged(int value);
    void on_inputSpinBox2_valueChanged(int value);

private:
    Ui::ShaderEditorGUI ui;
};
