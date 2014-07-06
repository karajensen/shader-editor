////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - tweaker.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include <QtGui>
#include "tweaker.h"

Tweaker::Tweaker(QWidget *parent) :
    QWidget(parent),
    m_lightPositionSet(false)
{
    m_ui.setupUi(this);
}

void Tweaker::SetSignalCallbacks(SignalCallbacks& callbacks)
{
    m_callbacks = callbacks;

    connect(m_ui.positionX_value, 
        SIGNAL(valueChanged(double)), this,
        SLOT(UpdateLightPositionX(double)));  

    connect(m_ui.positionY_value, 
        SIGNAL(valueChanged(double)), this,
        SLOT(UpdateLightPositionY(double)));  

    connect(m_ui.positionZ_value, 
        SIGNAL(valueChanged(double)), this,
        SLOT(UpdateLightPositionZ(double)));  
}

std::string Tweaker::GetSelectedPage() const
{
    return m_ui.TabMenu->currentWidget()->objectName().toStdString();
}

void Tweaker::SetDeltaTime(const std::string& dt)
{
    m_ui.deltaTime_text->setText(QString(dt.c_str()));
    m_ui.deltaTime_text->update();
}

void Tweaker::SetMousePosition(const std::string& x, const std::string& y)
{
    m_ui.mousePosX_text->setText(QString(x.c_str()));
    m_ui.mousePosX_text->update();

    m_ui.mousePosY_text->setText(QString(y.c_str()));
    m_ui.mousePosY_text->update();
}

void Tweaker::SetMouseDirection(const std::string& x, const std::string& y)
{
    m_ui.mouseDirX_text->setText(QString(x.c_str()));
    m_ui.mouseDirX_text->update();

    m_ui.mouseDirY_text->setText(QString(y.c_str()));
    m_ui.mouseDirY_text->update();
}

void Tweaker::SetLightPosition(float x, float y, float z)
{
    m_ui.positionX_value->setValue(x);
    m_ui.positionX_value->update();

    m_ui.positionY_value->setValue(y);
    m_ui.positionY_value->update();

    m_ui.positionZ_value->setValue(z);
    m_ui.positionZ_value->update();

    m_lightPositionSet = true;
}

bool Tweaker::LightPositionSet() const
{
    return m_lightPositionSet;
}

void Tweaker::UpdateLightPositionX(double value)
{
    m_callbacks.SetLightPositionX(value);
}

void Tweaker::UpdateLightPositionY(double value)
{
    m_callbacks.SetLightPositionY(value);
}

void Tweaker::UpdateLightPositionZ(double value)
{
    m_callbacks.SetLightPositionZ(value);
}