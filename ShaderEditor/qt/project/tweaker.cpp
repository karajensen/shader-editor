////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - tweaker.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "tweaker.h"
#include <QtGui>
#include <algorithm>

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

    m_lightDiag.Initialise(m_ui.lightDiagnostics_btn, m_callbacks.LightDiagnostics);
    m_reloadScene.Initialise(m_ui.reloadScene_btn, m_callbacks.ReloadScene);
    m_saveScene.Initialise(m_ui.saveScene_btn, m_callbacks.SaveScene);
    m_savePost.Initialise(m_ui.savePost_btn, m_callbacks.SavePost);
    m_pauseEmission.Initialise(m_ui.pauseEmission_btn, m_callbacks.PauseEmission);
    m_lightsOnly.Initialise(m_ui.renderLightsOnly_btn, m_callbacks.RenderLightsOnly);

    m_glowAmount.Initialise(1.0, m_ui.glowIntensity_value,
        m_ui.glowIntensity_dial, m_callbacks.SetGlowAmount);

    m_contrast.Initialise(0.01, m_ui.contrast_value,
        m_ui.contrast_dial, m_callbacks.SetContrast);

    m_saturation.Initialise(0.01, m_ui.saturation_value,
        m_ui.saturation_dial, m_callbacks.SetSaturation);

    m_blurAmount.Initialise(0.1, m_ui.blurring_value,
        m_ui.blurring_dial, m_callbacks.SetBlurAmount);

    m_DOFDistance.Initialise(0.01, m_ui.dofDistance_value,
        m_ui.dofDistance_dial, m_callbacks.SetDOFDistance);

    m_DOFFade.Initialise(0.01, m_ui.dofFade_value,
        m_ui.dofFade_dial, m_callbacks.SetDOFFade);

    m_blurStep.Initialise(0.5, m_ui.blurStep_value,
        m_ui.blurStep_dial, m_callbacks.SetBlurStep);

    m_depthNear.Initialise(1.0, m_ui.depthNear_value,
        m_ui.depthNear_dial, m_callbacks.SetDepthNear);

    m_depthFar.Initialise(10.0, m_ui.depthFar_value,
        m_ui.depthFar_dial, m_callbacks.SetDepthFar);

    m_ui.particleAmount_value->setMinimum(0.0);
    m_ui.particleAmount_value->setMaximum(500.0);

    m_particleAmount.Initialise(1.0, m_ui.particleAmount_value,
        m_ui.particleAmount_dial, m_callbacks.SetParticleAmount);

    m_ui.waveNumber_value->setMinimum(0.0);
    m_selectedWave.Initialise(1.0, m_ui.waveNumber_value,
        m_ui.waveNumber_dial, m_callbacks.SetSelectedWave);

    m_camera[CAMERA_POSITION_X].Initialise(1.0, m_ui.cameraX_value,
        m_ui.cameraX_dial, m_callbacks.SetCamera[CAMERA_POSITION_X]);

    m_camera[CAMERA_POSITION_Y].Initialise(1.0, m_ui.cameraY_value,
        m_ui.cameraY_dial, m_callbacks.SetCamera[CAMERA_POSITION_Y]);

    m_camera[CAMERA_POSITION_Z].Initialise(1.0, m_ui.cameraZ_value,
        m_ui.cameraZ_dial, m_callbacks.SetCamera[CAMERA_POSITION_Z]);

    m_camera[CAMERA_PITCH].Initialise(0.01, m_ui.cameraPitch_value,
        m_ui.cameraPitch_dial, m_callbacks.SetCamera[CAMERA_PITCH]);

    m_camera[CAMERA_YAW].Initialise(0.01, m_ui.cameraYaw_value,
        m_ui.cameraYaw_dial, m_callbacks.SetCamera[CAMERA_YAW]);

    m_camera[CAMERA_ROLL].Initialise(0.01, m_ui.cameraRoll_value,
        m_ui.cameraRoll_dial, m_callbacks.SetCamera[CAMERA_ROLL]);

    m_light[LIGHT_POSITION_X].Initialise(0.1, m_ui.positionX_value,
        m_ui.positionX_dial, m_callbacks.SetLight[LIGHT_POSITION_X]);
    
    m_light[LIGHT_POSITION_Y].Initialise(0.1, m_ui.positionY_value,
        m_ui.positionY_dial, m_callbacks.SetLight[LIGHT_POSITION_Y]);
    
    m_light[LIGHT_POSITION_Z].Initialise(0.1, m_ui.positionZ_value,
        m_ui.positionZ_dial, m_callbacks.SetLight[LIGHT_POSITION_Z]);
    
    m_light[LIGHT_ATTENUATION_X].Initialise(0.01, m_ui.attenuationX_value,
        m_ui.attenuationX_dial, m_callbacks.SetLight[LIGHT_ATTENUATION_X]);
    
    m_light[LIGHT_ATTENUATION_Y].Initialise(0.001, m_ui.attenuationY_value,
        m_ui.attenuationY_dial, m_callbacks.SetLight[LIGHT_ATTENUATION_Y]);
    
    m_light[LIGHT_ATTENUATION_Z].Initialise(0.0001, m_ui.attenuationZ_value,
        m_ui.attenuationZ_dial, m_callbacks.SetLight[LIGHT_ATTENUATION_Z]);

    m_light[LIGHT_SPECULARITY].Initialise(0.1, m_ui.specularity_value,
        m_ui.specularity_dial, m_callbacks.SetLight[LIGHT_SPECULARITY]);

    m_light[LIGHT_ACTIVE].Initialise(0.1, m_ui.lightActive_value,
        m_ui.lightActive_dial, m_callbacks.SetLight[LIGHT_ACTIVE]);

    std::vector<ComboEntry> diffuse;
    diffuse.emplace_back(LIGHT_DIFFUSE_R, "Diffuse R", 0.01, m_callbacks.SetLight[LIGHT_DIFFUSE_R]);
    diffuse.emplace_back(LIGHT_DIFFUSE_G, "Diffuse G", 0.01, m_callbacks.SetLight[LIGHT_DIFFUSE_G]);
    diffuse.emplace_back(LIGHT_DIFFUSE_B, "Diffuse B", 0.01, m_callbacks.SetLight[LIGHT_DIFFUSE_B]);
    m_lightDiffuse.Initialise(m_ui.light_diffuse_box, m_ui.light_diffuse_value, m_ui.light_diffuse_dial, diffuse);

    std::vector<ComboEntry> specular;
    specular.emplace_back(LIGHT_SPECULAR_R, "Specular R", 0.01, m_callbacks.SetLight[LIGHT_SPECULAR_R]);
    specular.emplace_back(LIGHT_SPECULAR_G, "Specular G", 0.01, m_callbacks.SetLight[LIGHT_SPECULAR_G]);
    specular.emplace_back(LIGHT_SPECULAR_B, "Specular B", 0.01, m_callbacks.SetLight[LIGHT_SPECULAR_B]);
    m_lightSpecular.Initialise(m_ui.light_specular_box, m_ui.light_specular_value, m_ui.light_specular_dial, specular);

    std::vector<ComboEntry> mesh;
    mesh.emplace_back(MESH_AMBIENCE, "Ambience", 0.01, m_callbacks.SetMesh[MESH_AMBIENCE]);
    mesh.emplace_back(MESH_BUMP, "Bump", 0.01, m_callbacks.SetMesh[MESH_BUMP]);
    mesh.emplace_back(MESH_GLOW, "Glow", 0.01, m_callbacks.SetMesh[MESH_GLOW]);
    mesh.emplace_back(MESH_SPECULARITY, "Specularity", 0.01, m_callbacks.SetMesh[MESH_SPECULARITY]);
    m_mesh.Initialise(m_ui.meshAttributes_box, m_ui.meshAttributes_value, m_ui.meshAttributes_dial, mesh);

    std::vector<ComboEntry> minimumColour;
    minimumColour.emplace_back(RED, "Min Red", 0.01, m_callbacks.SetMinColour[RED]);
    minimumColour.emplace_back(GREEN, "Min Green", 0.01, m_callbacks.SetMinColour[GREEN]);
    minimumColour.emplace_back(BLUE, "Min Blue", 0.01, m_callbacks.SetMinColour[BLUE]);
    m_minColour.Initialise(m_ui.minRange_box, m_ui.minRange_value, m_ui.minRange_dial, minimumColour);

    std::vector<ComboEntry> maximumColour;
    maximumColour.emplace_back(RED, "Max Red", 0.01, m_callbacks.SetMaxColour[RED]);
    maximumColour.emplace_back(GREEN, "Max Green", 0.01, m_callbacks.SetMaxColour[GREEN]);
    maximumColour.emplace_back(BLUE, "Max Blue", 0.01, m_callbacks.SetMaxColour[BLUE]);
    m_maxColour.Initialise(m_ui.maxRange_box, m_ui.maxRange_value, m_ui.maxRange_dial, maximumColour);

    std::vector<ComboEntry> fog;
    fog.emplace_back(FOG_FADE, "Fog Fade", 0.01, m_callbacks.SetFog[FOG_FADE]);
    fog.emplace_back(FOG_DISTANCE, "Fog Start", 0.01, m_callbacks.SetFog[FOG_DISTANCE]);
    fog.emplace_back(FOG_RED, "Fog Red", 0.01, m_callbacks.SetFog[FOG_RED]);
    fog.emplace_back(FOG_GREEN, "Fog Green", 0.01, m_callbacks.SetFog[FOG_GREEN]);
    fog.emplace_back(FOG_BLUE, "Fog Blue", 0.01, m_callbacks.SetFog[FOG_BLUE]);
    m_fog.Initialise(m_ui.fog_box, m_ui.fog_value, m_ui.fog_dial, fog);

    std::vector<ComboEntry> emitter;
    emitter.emplace_back(EMITTER_LIFETIME, "Lifetime", 0.01, m_callbacks.SetEmitter[EMITTER_LIFETIME]);
    emitter.emplace_back(EMITTER_LIFEFADE, "LifeFade", 0.01, m_callbacks.SetEmitter[EMITTER_LIFEFADE]);
    emitter.emplace_back(EMITTER_LENGTH, "Length", 0.01, m_callbacks.SetEmitter[EMITTER_LENGTH]);
    emitter.emplace_back(EMITTER_WIDTH, "Width", 0.01, m_callbacks.SetEmitter[EMITTER_WIDTH]);
    emitter.emplace_back(EMITTER_DIR_X, "Direction X", 0.01, m_callbacks.SetEmitter[EMITTER_DIR_X]);
    emitter.emplace_back(EMITTER_DIR_Y, "Direction Y", 0.01, m_callbacks.SetEmitter[EMITTER_DIR_Y]);
    emitter.emplace_back(EMITTER_DIR_Z, "Direction Z", 0.01, m_callbacks.SetEmitter[EMITTER_DIR_Z]);
    emitter.emplace_back(EMITTER_POS_X, "Position X", 0.1, m_callbacks.SetEmitter[EMITTER_POS_X]);
    emitter.emplace_back(EMITTER_POS_Y, "Position Y", 0.1, m_callbacks.SetEmitter[EMITTER_POS_Y]);
    emitter.emplace_back(EMITTER_POS_Z, "Position Z", 0.1, m_callbacks.SetEmitter[EMITTER_POS_Z]);
    m_emitter.Initialise(m_ui.emitter_box, m_ui.emitter_value, m_ui.emitter_dial, emitter);

    std::vector<ComboEntry> emitterColor;
    emitterColor.emplace_back(EMITTER_TINT_R, "Tint R", 0.01, m_callbacks.SetEmitter[EMITTER_TINT_R]);
    emitterColor.emplace_back(EMITTER_TINT_G, "Tint G", 0.01, m_callbacks.SetEmitter[EMITTER_TINT_G]);
    emitterColor.emplace_back(EMITTER_TINT_B, "Tint B", 0.01, m_callbacks.SetEmitter[EMITTER_TINT_B]);
    m_emitterColour.Initialise(m_ui.emitterColour_box, m_ui.emitterColour_value, m_ui.emitterColour_dial, emitterColor);

    std::vector<ComboEntry> emitterMinMax;
    emitterMinMax.emplace_back(EMITTER_MAX_SPEED, "Max Speed", 0.01, m_callbacks.SetEmitter[EMITTER_MAX_SPEED]);
    emitterMinMax.emplace_back(EMITTER_MIN_SPEED, "Min Speed", 0.01, m_callbacks.SetEmitter[EMITTER_MIN_SPEED]);
    emitterMinMax.emplace_back(EMITTER_MAX_SIZE, "Max Size", 0.1, m_callbacks.SetEmitter[EMITTER_MAX_SIZE]);
    emitterMinMax.emplace_back(EMITTER_MIN_SIZE, "Min Size", 0.1, m_callbacks.SetEmitter[EMITTER_MIN_SIZE]);
    emitterMinMax.emplace_back(EMITTER_MAX_AMP, "Max Amp", 0.1, m_callbacks.SetEmitter[EMITTER_MAX_AMP]);
    emitterMinMax.emplace_back(EMITTER_MIN_AMP, "Min Amp", 0.1, m_callbacks.SetEmitter[EMITTER_MIN_AMP]);
    emitterMinMax.emplace_back(EMITTER_MAX_FREQ, "Max Freq", 0.1, m_callbacks.SetEmitter[EMITTER_MAX_FREQ]);
    emitterMinMax.emplace_back(EMITTER_MIN_FREQ, "Min Freq", 0.1, m_callbacks.SetEmitter[EMITTER_MIN_FREQ]);
    emitterMinMax.emplace_back(EMITTER_MAX_WAVE, "Max Wave Sp", 0.1, m_callbacks.SetEmitter[EMITTER_MAX_WAVE]);
    emitterMinMax.emplace_back(EMITTER_MIN_WAVE, "Min Wave Sp", 0.1, m_callbacks.SetEmitter[EMITTER_MIN_WAVE]);
    m_emitterMinMax.Initialise(m_ui.emitterMinMax_box, m_ui.emitterMinMax_value, m_ui.emitterMinMax_dial, emitterMinMax);

    std::vector<ComboEntry> water;
    water.emplace_back(WATER_SPEED, "Speed", 0.01, m_callbacks.SetWater[WATER_SPEED]);
    water.emplace_back(WATER_SHALLOW_R, "Shallow R", 0.01, m_callbacks.SetWater[WATER_SHALLOW_R]);
    water.emplace_back(WATER_SHALLOW_G, "Shallow G", 0.01, m_callbacks.SetWater[WATER_SHALLOW_G]);
    water.emplace_back(WATER_SHALLOW_B, "Shallow B", 0.01, m_callbacks.SetWater[WATER_SHALLOW_B]);
    water.emplace_back(WATER_SHALLOW_A, "Shallow A", 0.01, m_callbacks.SetWater[WATER_SHALLOW_A]);
    water.emplace_back(WATER_DEEP_R, "Deep R", 0.01, m_callbacks.SetWater[WATER_DEEP_R]);
    water.emplace_back(WATER_DEEP_G, "Deep G", 0.01, m_callbacks.SetWater[WATER_DEEP_G]);
    water.emplace_back(WATER_DEEP_B, "Deep B", 0.01, m_callbacks.SetWater[WATER_DEEP_B]);
    water.emplace_back(WATER_DEEP_A, "Deep A", 0.01, m_callbacks.SetWater[WATER_DEEP_A]);
    water.emplace_back(WATER_REFLECTION_R, "Reflection R", 0.01, m_callbacks.SetWater[WATER_REFLECTION_R]);
    water.emplace_back(WATER_REFLECTION_G, "Reflection G", 0.01, m_callbacks.SetWater[WATER_REFLECTION_G]);
    water.emplace_back(WATER_REFLECTION_B, "Reflection B", 0.01, m_callbacks.SetWater[WATER_REFLECTION_B]);
    water.emplace_back(WATER_REFLECTION, "Reflection", 0.01, m_callbacks.SetWater[WATER_REFLECTION]);
    water.emplace_back(WATER_BUMP, "Bump", 0.01, m_callbacks.SetWater[WATER_BUMP]);
    water.emplace_back(WATER_BUMP_VELOCITY_X, "Bump Vel X", 0.01, m_callbacks.SetWater[WATER_BUMP_VELOCITY_X]);
    water.emplace_back(WATER_BUMP_VELOCITY_Y, "Bump Vel Y", 0.01, m_callbacks.SetWater[WATER_BUMP_VELOCITY_Y]);
    water.emplace_back(WATER_SCALE_U, "Scale U", 0.01, m_callbacks.SetWater[WATER_SCALE_U]);
    water.emplace_back(WATER_SCALE_V, "Scale V", 0.01, m_callbacks.SetWater[WATER_SCALE_V]);
    water.emplace_back(WATER_FRESNAL_SCALE, "Fresnal Scale", 0.01, m_callbacks.SetWater[WATER_FRESNAL_SCALE]);
    water.emplace_back(WATER_FRESNAL_BIAS, "Fresnal Bias", 0.01, m_callbacks.SetWater[WATER_FRESNAL_BIAS]);
    water.emplace_back(WATER_FRESNAL_POWER, "Fresnal Pow", 0.01, m_callbacks.SetWater[WATER_FRESNAL_POWER]);
    m_water.Initialise(m_ui.water_box, m_ui.water_value, m_ui.water_dial, water);

    std::vector<ComboEntry> wave;
    wave.emplace_back(WAVE_AMPLITUDE, "Amplitude", 0.01, m_callbacks.SetWave[WAVE_AMPLITUDE]);
    wave.emplace_back(WAVE_FREQUENCY, "Frequency", 0.01, m_callbacks.SetWave[WAVE_FREQUENCY]);
    wave.emplace_back(WAVE_PHASE, "Phase", 0.01, m_callbacks.SetWave[WAVE_PHASE]);
    wave.emplace_back(WAVE_DIRECTION_X, "Direction X", 0.01, m_callbacks.SetWave[WAVE_DIRECTION_X]);
    wave.emplace_back(WAVE_DIRECTION_Z, "Direction Z", 0.01, m_callbacks.SetWave[WAVE_DIRECTION_Z]);
    m_wave.Initialise(m_ui.wave_box, m_ui.wave_value, m_ui.wave_dial, wave);
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

void Tweaker::SetTimer(const std::string& timer)
{
    m_ui.timer_text->setText(QString(timer.c_str()));
    m_ui.timer_text->update();
}   

void Tweaker::SetFramesPerSec(const std::string& fps)
{
    m_ui.fps_text->setText(QString(fps.c_str()));
    m_ui.fps_text->update();
}

void Tweaker::SetMeshInstanceCount(const std::string& count)
{
    m_ui.instances_text->setText(QString(count.c_str()));
    m_ui.instances_text->update();
}

void Tweaker::SetMeshShaderName(const std::string& name)
{
    m_ui.shader_text->setText(QString(name.c_str()));
    m_ui.shader_text->update();
}

void Tweaker::SetFog(FogAttribute attribute, float value)
{
    m_fog.SetValue(attribute, value);
}

void Tweaker::SetCamera(CameraAttribute attribute, float value)
{
    m_camera.at(attribute).Set(value);
}

void Tweaker::SetLight(LightAttribute attribute, float value)
{
    if (!m_lightDiffuse.SetValue(attribute, value) &&
        !m_lightSpecular.SetValue(attribute, value))
    {
        m_light.at(attribute).Set(value);
    }
}

void Tweaker::SetMesh(MeshAttribute attribute, float value)
{
    m_mesh.SetValue(attribute, value);
}

void Tweaker::SetWave(WaveAttribute attribute, float value)
{
    m_wave.SetValue(attribute, value);
}

void Tweaker::SetEmitter(EmitterAttribute attribute, float value)
{
    if (!m_emitterColour.SetValue(attribute, value) &&
        !m_emitterMinMax.SetValue(attribute, value))
    {
        m_emitter.SetValue(attribute, value);
    }
}

void Tweaker::SetMaximumColour(ColourAttribute attribute, float value)
{
    m_maxColour.SetValue(attribute, value);
}

void Tweaker::SetMinimumColour(ColourAttribute attribute, float value)
{
    m_minColour.SetValue(attribute, value);
}

void Tweaker::SetWater(WaterAttribute attribute, float value)
{
    m_water.SetValue(attribute, value);
}

void Tweaker::SetDOFFade(float value)
{
    m_DOFFade.Set(value);
}

void Tweaker::SetDOFDistance(float value)
{
    m_DOFDistance.Set(value);
}

void Tweaker::SetBlurAmount(float value)
{
    m_blurAmount.Set(value);
}

void Tweaker::SetBlurStep(float value)
{
    m_blurStep.Set(value);
}

void Tweaker::SetGlowAmount(float value)
{
    m_glowAmount.Set(value);
}

void Tweaker::SetContrast(float value)
{
    m_contrast.Set(value);
}

void Tweaker::SetSaturation(float value)
{
    m_saturation.Set(value);
}

void Tweaker::SetDepthNear(float value)
{
    m_depthNear.Set(value);
}

void Tweaker::SetDepthFar(float value)
{
    m_depthFar.Set(value);
}

void Tweaker::SetSelectedPostMap(int selected)
{
    m_postMap.SetSelected(selected);
}

void Tweaker::SetSelectedEngine(int selected)
{
    m_renderEngine.SetSelected(selected);
}

void Tweaker::InitialiseEmitters(int selected,
                                 const std::vector<std::string>& emitters)
{
    if (!emitters.empty())
    {
        m_selectedEmitter.Initialise(m_ui.selectedParticles_box,
            selected, emitters, m_callbacks.SetSelectedEmitter);
    }
}

void Tweaker::InitialisePostMaps(int selected,
                                 const std::vector<std::string>& maps)
{
    if (!maps.empty())
    {
        m_postMap.Initialise(m_ui.postImage_box,
            selected, maps, m_callbacks.SetPostMap);
    }
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
        m_selectedMesh.Initialise(m_ui.selectedMesh_box,
            selected, meshes, m_callbacks.SetSelectedMesh);
    }
}

void Tweaker::SetParticleAmount(int amount)
{
    m_particleAmount.Set(static_cast<double>(amount));
}

void Tweaker::SetWaveAmount(int amount)
{
    const bool enabled = amount != 0;
    m_ui.wave_box->setEnabled(enabled);
    m_ui.wave_dial->setEnabled(enabled);
    m_ui.wave_value->setEnabled(enabled);
    m_ui.waveNumber_value->setEnabled(enabled);
    m_ui.waveNumber_dial->setEnabled(enabled);
    m_ui.waveNumber_value->setMaximum(std::max(0.0, static_cast<double>(amount-1.0)));
}

void Tweaker::InitialiseWater(int selected,
                              const std::vector<std::string>& water)
{
    if(!water.empty())
    {
        m_selectedWater.Initialise(m_ui.selectedWater_box,
            selected, water, m_callbacks.SetSelectedWater);
    }
}

void Tweaker::InitialiseLights(int selected,
                               const std::vector<std::string>& lights)
{
    if(!lights.empty())
    {
        m_selectedLight.Initialise(m_ui.selectedLight_box,
            selected, lights, m_callbacks.SetSelectedLight);
    }
}

bool Tweaker::HasPostMaps() const
{
    return m_postMap.IsInitialised();
}

bool Tweaker::HasMeshes() const
{
    return m_selectedMesh.IsInitialised();
}

bool Tweaker::HasLights() const
{
    return m_selectedLight.IsInitialised();
}

bool Tweaker::HasEmitters() const
{
    return m_selectedEmitter.IsInitialised();
}

bool Tweaker::HasWater() const
{
    return m_selectedWater.IsInitialised();
}

bool Tweaker::HasEngines() const
{
    return m_renderEngine.IsInitialised();
}
