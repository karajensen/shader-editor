////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - shaderEditorGUI.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include <QtGui>
#include "shaderEditorGUI.h"

ShaderEditorGUI::ShaderEditorGUI(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
}

void ShaderEditorGUI::on_inputSpinBox1_valueChanged(int value)
{
    ui.outputWidget->setText(QString::number(value + ui.inputSpinBox2->value()));
}

void ShaderEditorGUI::on_inputSpinBox2_valueChanged(int value)
{
    ui.outputWidget->setText(QString::number(value + ui.inputSpinBox1->value()));
}