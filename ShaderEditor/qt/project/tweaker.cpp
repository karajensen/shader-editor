////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - tweaker.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include <QtGui>
#include "tweaker.h"

Tweaker::Tweaker(QWidget *parent) :
    QWidget(parent)
{
    m_ui.setupUi(this);
}

Tweakable::Tweakable() :
    QWidget(nullptr),
    m_signalCallback(nullptr)
{
}

void Tweakable::Initialise(float value, 
                           float step,
                           QDoubleSpinBox* box, 
                           QDial* dial,
                           std::function<void(float)> signalCallback)
{
    m_signalCallback = signalCallback;

    box->setValue(value);
    box->setSingleStep(step);

    connect(box, SIGNAL(valueChanged(double)), 
        this, SLOT(UpdateValue(double)));  

    connect(dial, SIGNAL(sliderMoved(int)), 
        this, SLOT(DialValue(int)));  

    box->update();
    dial->update();
}

void Tweakable::UpdateValue(double value)
{
    m_signalCallback(value);
}

void Tweakable::DialValue(int value)
{
}

bool Tweakable::IsInitialised() const
{
    return m_signalCallback != nullptr;
}

void Tweaker::SetSignalCallbacks(SignalCallbacks& callbacks)
{
    m_callbacks = callbacks;
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
    m_lightPositionX.Initialise(x, 1.0, m_ui.positionX_value,
        m_ui.positionX_dial, m_callbacks.SetLightPositionX);

    m_lightPositionY.Initialise(y, 1.0, m_ui.positionY_value,
        m_ui.positionY_dial, m_callbacks.SetLightPositionY);

    m_lightPositionZ.Initialise(z, 1.0, m_ui.positionZ_value,
        m_ui.positionZ_dial, m_callbacks.SetLightPositionZ);
}

bool Tweaker::LightPositionSet() const
{
    return m_lightPositionX.IsInitialised() &&
        m_lightPositionY.IsInitialised() &&
        m_lightPositionZ.IsInitialised();
}

void Tweaker::SetLightAttenuation(float x, float y, float z)
{
    m_lightAttenuationX.Initialise(x, 0.1, m_ui.attenuationX_value,
        m_ui.attenuationX_dial, m_callbacks.SetLightAttX);

    m_lightAttenuationY.Initialise(y, 0.01, m_ui.attenuationY_value,
        m_ui.attenuationY_dial, m_callbacks.SetLightAttY);

    m_lightAttenuationZ.Initialise(z, 0.001, m_ui.attenuationZ_value,
        m_ui.attenuationZ_dial, m_callbacks.SetLightAttZ);
}

bool Tweaker::LightAttenuationSet() const
{
    return m_lightAttenuationX.IsInitialised() &&
        m_lightAttenuationY.IsInitialised() &&
        m_lightAttenuationZ.IsInitialised();
}