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
    m_signalCallback(nullptr),
    m_previousDialValue(0),
    m_box(nullptr)
{
}

void Tweakable::Initialise(double value, 
                           double step,
                           QDoubleSpinBox* box, 
                           QDial* dial,
                           std::function<void(float)> signalCallback)
{
    m_signalCallback = signalCallback;
    m_box = box;

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
    if(value != m_previousDialValue)
    {
        const bool clockwise = value > m_previousDialValue;
        clockwise ? m_box->stepUp() : m_box->stepDown();
        m_previousDialValue = value;
    }
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

void Tweaker::SetFramesPerSec(const std::string& fps)
{
    m_ui.fps_text->setText(QString(fps.c_str()));
    m_ui.fps_text->update();
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

void Tweaker::SetCameraPosition(const std::string& x, 
                                const std::string& y, 
                                const std::string& z)
{
    m_ui.cameraX_text->setText(QString(x.c_str()));
    m_ui.cameraX_text->update();

    m_ui.cameraY_text->setText(QString(y.c_str()));
    m_ui.cameraY_text->update();

    m_ui.cameraZ_text->setText(QString(z.c_str()));
    m_ui.cameraZ_text->update();
}

void Tweaker::SetMeshBackFaceCull(bool enable)
{
    m_ui.backfaceCull_text->setText(enable ? "True" : "False");
    m_ui.backfaceCull_text->update();
}

void Tweaker::SetMeshTransparency(bool enable)
{
    m_ui.transparency_text->setText(enable ? "True" : "False");
    m_ui.transparency_text->update();
}

void Tweaker::SetMeshDiffuseTexture(const std::string& name)
{
    m_ui.diffuseMap_text->setText(QString(name.c_str()));
    m_ui.diffuseMap_text->update();
}

void Tweaker::SetMeshSpecularTexture(const std::string& name)
{
    m_ui.specularMap_text->setText(QString(name.c_str()));
    m_ui.specularMap_text->update();
}

void Tweaker::SetMeshNormalTexture(const std::string& name)
{
    m_ui.normalMap_text->setText(QString(name.c_str()));
    m_ui.normalMap_text->update();
}

void Tweaker::SetMeshShaderName(const std::string& name)
{
    m_ui.shader_text->setText(QString(name.c_str()));
    m_ui.shader_text->update();
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

void Tweaker::SetLightAttenuation(float x, float y, float z)
{
    m_lightAttenuationX.Initialise(x, 0.1, m_ui.attenuationX_value,
        m_ui.attenuationX_dial, m_callbacks.SetLightAttX);

    m_lightAttenuationY.Initialise(y, 0.01, m_ui.attenuationY_value,
        m_ui.attenuationY_dial, m_callbacks.SetLightAttY);

    m_lightAttenuationZ.Initialise(z, 0.001, m_ui.attenuationZ_value,
        m_ui.attenuationZ_dial, m_callbacks.SetLightAttZ);
}

void Tweaker::SetLightDiffuse(float r, float g, float b)
{
    m_lightDiffuseR.Initialise(r, 0.01, m_ui.diffuseRed_value,
        m_ui.diffuseRed_dial, m_callbacks.SetLightDiffuseR);

    m_lightDiffuseG.Initialise(g, 0.01, m_ui.diffuseGreen_value,
        m_ui.diffuseGreen_dial, m_callbacks.SetLightDiffuseG);

    m_lightDiffuseB.Initialise(b, 0.01, m_ui.diffuseBlue_value,
        m_ui.diffuseBlue_dial, m_callbacks.SetLightDiffuseB);
}

void Tweaker::SetLightSpecular(float r, float g, float b)
{
    m_lightSpecularR.Initialise(r, 0.01, m_ui.specularRed_value,
        m_ui.specularRed_dial, m_callbacks.SetLightSpecularR);

    m_lightSpecularG.Initialise(g, 0.01, m_ui.specularGreen_value,
        m_ui.specularGreen_dial, m_callbacks.SetLightSpecularG);

    m_lightSpecularB.Initialise(b, 0.01, m_ui.specularBlue_value,
        m_ui.specularBlue_dial, m_callbacks.SetLightSpecularB);
}

void Tweaker::SetLightSpecularity(float size)
{
    m_lightSpecularity.Initialise(size, 0.1, m_ui.lightSpecularity_value,
        m_ui.lightSpecularity_dial, m_callbacks.SetLightSpecularity);
}

void Tweaker::SetMeshSpecularity(float size)
{
    m_meshSpecularity.Initialise(size, 0.1, m_ui.meshSpecularity_value,
        m_ui.meshSpecularity_dial, m_callbacks.SetMeshSpecularity);
}

bool Tweaker::MeshSpecularitySet() const
{
    return m_meshSpecularity.IsInitialised();
}

bool Tweaker::LightSpecularitySet() const
{
    return m_lightSpecularity.IsInitialised();
}

bool Tweaker::LightDiffuseSet() const
{
    return m_lightDiffuseR.IsInitialised() &&
        m_lightDiffuseG.IsInitialised() &&
        m_lightDiffuseB.IsInitialised();
}

bool Tweaker::LightSpecularSet() const
{
    return m_lightSpecularR.IsInitialised() &&
        m_lightSpecularG.IsInitialised() &&
        m_lightSpecularB.IsInitialised();
}

bool Tweaker::LightAttenuationSet() const
{
    return m_lightAttenuationX.IsInitialised() &&
        m_lightAttenuationY.IsInitialised() &&
        m_lightAttenuationZ.IsInitialised();
}

bool Tweaker::LightPositionSet() const
{
    return m_lightPositionX.IsInitialised() &&
        m_lightPositionY.IsInitialised() &&
        m_lightPositionZ.IsInitialised();
}
