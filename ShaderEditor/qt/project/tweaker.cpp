////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - tweaker.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include <QtGui>
#include "tweaker.h"

namespace
{
    enum Colour
    {
        RED,
        GREEN,
        BLUE
    };
}

Tweaker::Tweaker(QWidget* parent) :
    QWidget(parent)
{
    m_ui.setupUi(this);
}

Tweaker::Tweaker(const SignalCallbacks& callbacks, QWidget* parent) :
    QWidget(parent),
    m_callbacks(callbacks)
{
    m_ui.setupUi(this);
    m_ui.TabMenu->setCurrentIndex(0);

    m_depthNear.Initialise(1.0, m_ui.depthNear_value,
        m_ui.depthNear_dial, m_callbacks.SetDepthNear);

    m_depthFar.Initialise(10.0, m_ui.depthFar_value,
        m_ui.depthFar_dial, m_callbacks.SetDepthFar);

    m_lightPositionX.Initialise(1.0, m_ui.positionX_value,
        m_ui.positionX_dial, m_callbacks.SetLightPositionX);

    m_lightPositionY.Initialise(1.0, m_ui.positionY_value,
        m_ui.positionY_dial, m_callbacks.SetLightPositionY);

    m_lightPositionZ.Initialise(1.0, m_ui.positionZ_value,
        m_ui.positionZ_dial, m_callbacks.SetLightPositionZ);

    m_lightAttenuationX.Initialise(0.01, m_ui.attenuationX_value,
        m_ui.attenuationX_dial, m_callbacks.SetLightAttX);

    m_lightAttenuationY.Initialise(0.001, m_ui.attenuationY_value,
        m_ui.attenuationY_dial, m_callbacks.SetLightAttY);

    m_lightAttenuationZ.Initialise(0.0001, m_ui.attenuationZ_value,
        m_ui.attenuationZ_dial, m_callbacks.SetLightAttZ);

    m_lightDiffuseR.Initialise(0.01, m_ui.diffuseRed_value,
        m_ui.diffuseRed_dial, m_callbacks.SetLightDiffuseR);

    m_lightDiffuseG.Initialise(0.01, m_ui.diffuseGreen_value,
        m_ui.diffuseGreen_dial, m_callbacks.SetLightDiffuseG);

    m_lightDiffuseB.Initialise(0.01, m_ui.diffuseBlue_value,
        m_ui.diffuseBlue_dial, m_callbacks.SetLightDiffuseB);

    m_lightSpecularR.Initialise(0.01, m_ui.specularRed_value,
        m_ui.specularRed_dial, m_callbacks.SetLightSpecularR);

    m_lightSpecularG.Initialise(0.01, m_ui.specularGreen_value,
        m_ui.specularGreen_dial, m_callbacks.SetLightSpecularG);

    m_lightSpecularB.Initialise(0.01, m_ui.specularBlue_value,
        m_ui.specularBlue_dial, m_callbacks.SetLightSpecularB);

    m_lightSpecularity.Initialise(0.1, m_ui.lightSpecularity_value,
        m_ui.lightSpecularity_dial, m_callbacks.SetLightSpecularity);

    m_meshSpecularity.Initialise(0.01, m_ui.meshSpecularity_value,
        m_ui.meshSpecularity_dial, m_callbacks.SetMeshSpecularity);

    m_meshAmbience.Initialise(0.01, m_ui.meshAmbience_value,
        m_ui.meshAmbience_dial, m_callbacks.SetMeshAmbience);

    m_meshBump.Initialise(0.01, m_ui.meshBump_value,
        m_ui.meshBump_dial, m_callbacks.SetMeshBump);

    m_meshGlow.Initialise(0.05, m_ui.meshGlow_value,
        m_ui.meshGlow_dial, m_callbacks.SetMeshGlow);

    std::vector<ComboEntry> minimumColour;
    minimumColour.push_back(ComboEntry("Min Red", 0.01, m_callbacks.SetMinRed));
    minimumColour.push_back(ComboEntry("Min Green", 0.01, m_callbacks.SetMinGreen));
    minimumColour.push_back(ComboEntry("Min Blue", 0.01, m_callbacks.SetMinBlue));

    m_minColour.Initialise(m_ui.minRange_box,
        m_ui.minRange_value, m_ui.minRange_dial, minimumColour);

    std::vector<ComboEntry> maximumColour;
    maximumColour.push_back(ComboEntry("Max Red", 0.01, m_callbacks.SetMaxRed));
    maximumColour.push_back(ComboEntry("Max Green", 0.01, m_callbacks.SetMaxGreen));
    maximumColour.push_back(ComboEntry("Max Blue", 0.01, m_callbacks.SetMaxBlue));

    m_maxColour.Initialise(m_ui.maxRange_box,
        m_ui.maxRange_value, m_ui.maxRange_dial, maximumColour);
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

void Tweaker::SetMeshShaderName(const std::string& name)
{
    m_ui.shader_text->setText(QString(name.c_str()));
    m_ui.shader_text->update();
}

void Tweaker::SetLightPosition(float x, float y, float z)
{
    m_lightPositionX.Set(x);
    m_lightPositionY.Set(y);
    m_lightPositionZ.Set(z);
}

void Tweaker::SetLightAttenuation(float x, float y, float z)
{
    m_lightAttenuationX.Set(x);
    m_lightAttenuationY.Set(y);
    m_lightAttenuationZ.Set(z);
}

void Tweaker::SetLightDiffuse(float r, float g, float b)
{
    m_lightDiffuseR.Set(r);
    m_lightDiffuseG.Set(g);
    m_lightDiffuseB.Set(b);
}

void Tweaker::SetLightSpecular(float r, float g, float b)
{
    m_lightSpecularR.Set(r);
    m_lightSpecularG.Set(g);
    m_lightSpecularB.Set(b);
}

void Tweaker::SetLightSpecularity(float size)
{
    m_lightSpecularity.Set(size);
}

void Tweaker::SetMeshSpecularity(float size)
{
    m_meshSpecularity.Set(size);
}

void Tweaker::SetMeshAmbience(float value)
{
    m_meshAmbience.Set(value);
}

void Tweaker::SetMeshGlow(float value)
{
    m_meshGlow.Set(value);
}

void Tweaker::SetMeshBump(float value)
{
    m_meshBump.Set(value);
}

void Tweaker::SetDepthNear(float value)
{
    m_depthNear.Set(value);
}

void Tweaker::SetDepthFar(float value)
{
    m_depthFar.Set(value);
}

void Tweaker::InitialiseTextures(int selected,
                                 const std::vector<std::string>& textures)
{
    if (!textures.empty())
    {
        m_postTexture.Initialise(m_ui.postImage_box,
            selected, textures, m_callbacks.SetPostTexture);
    }
}

void Tweaker::SetSelectedEngine(int selected)
{
    m_renderEngine.SetSelected(selected);
}

void Tweaker::InitialiseEngines(int selected,
                                const std::vector<std::string>& engines)
{
    if(!engines.empty())
    {
        m_renderEngine.Initialise(m_ui.switchEngine_box, 
            selected, engines, m_callbacks.SetSelectedEngine);
    }
}

void Tweaker::InitialiseMeshes(int selected,
                               const std::vector<std::string>& meshes)
{
    if(!meshes.empty())
    {
        m_mesh.Initialise(m_ui.selectedMesh_box, 
            selected, meshes, m_callbacks.SetSelectedMesh);
    }
}

void Tweaker::InitialiseLights(int selected,
                               const std::vector<std::string>& lights)
{
    if(!lights.empty())
    {
        m_light.Initialise(m_ui.selectedLight_box, 
            selected, lights, m_callbacks.SetSelectedLight);
    }
}

bool Tweaker::HasPostTextures() const
{
    return m_postTexture.IsInitialised();
}

bool Tweaker::HasMeshes() const
{
    return m_mesh.IsInitialised();
}

bool Tweaker::HasLights() const
{
    return m_light.IsInitialised();
}

bool Tweaker::HasEngines() const
{
    return m_renderEngine.IsInitialised();
}

void Tweaker::SetMinimumColour(float r, float g, float b)
{
    m_minColour.SetValue(RED, r);
    m_minColour.SetValue(GREEN, g);
    m_minColour.SetValue(BLUE, b);
}

void Tweaker::SetMaximumColour(float r, float g, float b)
{
    m_maxColour.SetValue(RED, r);
    m_maxColour.SetValue(GREEN, g);
    m_maxColour.SetValue(BLUE, b);
}
