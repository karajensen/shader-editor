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
    m_pauseEmission.Initialise(m_ui.pauseEmission_btn, m_callbacks.PauseEmission);
    m_lightsOnly.Initialise(m_ui.renderLightsOnly_btn, m_callbacks.RenderLightsOnly);

    m_ui.particleAmount_value->setMinimum(0.0);
    m_ui.particleAmount_value->setMaximum(500.0);
    m_particleAmount.Initialise(1.0, 0, m_ui.particleAmount_value,
        m_ui.particleAmount_dial, m_callbacks.SetParticleAmount);

    m_ui.waveNumber_value->setMinimum(0.0);
    m_selectedWave.Initialise(1.0, 0, m_ui.waveNumber_value,
        m_ui.waveNumber_dial, m_callbacks.SetSelectedWave);

    m_post[POST_BLOOM_AMOUNT].Initialise(0.01, 3, m_ui.bloomAmount_value,
        m_ui.bloomAmount_dial, m_callbacks.SetPost[POST_BLOOM_AMOUNT]);

    m_post[POST_BLOOM_START].Initialise(0.01, 3, m_ui.bloomStart_value,
        m_ui.bloomStart_dial, m_callbacks.SetPost[POST_BLOOM_START]);

    m_post[POST_CONTRAST].Initialise(0.01, 3, m_ui.contrast_value,
        m_ui.contrast_dial, m_callbacks.SetPost[POST_CONTRAST]);

    m_post[POST_SATURATION].Initialise(0.01, 3, m_ui.saturation_value,
        m_ui.saturation_dial, m_callbacks.SetPost[POST_SATURATION]);    

    m_post[POST_BLUR_AMOUNT].Initialise(0.01, 4, m_ui.blurring_value,
        m_ui.blurring_dial, m_callbacks.SetPost[POST_BLUR_AMOUNT]);    

    m_post[POST_BLUR_STEP].Initialise(0.0001, 4, m_ui.blurStep_value,
        m_ui.blurStep_dial, m_callbacks.SetPost[POST_BLUR_STEP]);    

    m_post[POST_DOF_DISTANCE].Initialise(0.01, 3, m_ui.dofDistance_value,
        m_ui.dofDistance_dial, m_callbacks.SetPost[POST_DOF_DISTANCE]); 

    m_post[POST_DOF_FADE].Initialise(0.01, 3, m_ui.dofFade_value,
        m_ui.dofFade_dial, m_callbacks.SetPost[POST_DOF_FADE]); 

    m_post[POST_DEPTH_NEAR].Initialise(0.1, 2, m_ui.depthNear_value,
        m_ui.depthNear_dial, m_callbacks.SetPost[POST_DEPTH_NEAR]); 

    m_post[POST_DEPTH_FAR].Initialise(10.0, 2, m_ui.depthFar_value,
        m_ui.depthFar_dial, m_callbacks.SetPost[POST_DEPTH_FAR]); 

    m_camera[CAMERA_POSITION_X].Initialise(0.1, 3, m_ui.cameraX_value,
        m_ui.cameraX_dial, m_callbacks.SetCamera[CAMERA_POSITION_X]);

    m_camera[CAMERA_POSITION_Y].Initialise(0.1, 3, m_ui.cameraY_value,
        m_ui.cameraY_dial, m_callbacks.SetCamera[CAMERA_POSITION_Y]);

    m_camera[CAMERA_POSITION_Z].Initialise(0.1, 3, m_ui.cameraZ_value,
        m_ui.cameraZ_dial, m_callbacks.SetCamera[CAMERA_POSITION_Z]);

    m_camera[CAMERA_PITCH].Initialise(0.01, 3, m_ui.cameraPitch_value,
        m_ui.cameraPitch_dial, m_callbacks.SetCamera[CAMERA_PITCH]);

    m_camera[CAMERA_YAW].Initialise(0.01, 3, m_ui.cameraYaw_value,
        m_ui.cameraYaw_dial, m_callbacks.SetCamera[CAMERA_YAW]);

    m_camera[CAMERA_ROLL].Initialise(0.01, 3, m_ui.cameraRoll_value,
        m_ui.cameraRoll_dial, m_callbacks.SetCamera[CAMERA_ROLL]);

    m_light[LIGHT_POSITION_X].Initialise(0.1, 3, m_ui.positionX_value,
        m_ui.positionX_dial, m_callbacks.SetLight[LIGHT_POSITION_X]);
    
    m_light[LIGHT_POSITION_Y].Initialise(0.1, 3, m_ui.positionY_value,
        m_ui.positionY_dial, m_callbacks.SetLight[LIGHT_POSITION_Y]);
    
    m_light[LIGHT_POSITION_Z].Initialise(0.1, 3, m_ui.positionZ_value,
        m_ui.positionZ_dial, m_callbacks.SetLight[LIGHT_POSITION_Z]);
    
    m_light[LIGHT_ATTENUATION_X].Initialise(0.01, 4, m_ui.attenuationX_value,
        m_ui.attenuationX_dial, m_callbacks.SetLight[LIGHT_ATTENUATION_X]);
    
    m_light[LIGHT_ATTENUATION_Y].Initialise(0.001, 4, m_ui.attenuationY_value,
        m_ui.attenuationY_dial, m_callbacks.SetLight[LIGHT_ATTENUATION_Y]);
    
    m_light[LIGHT_ATTENUATION_Z].Initialise(0.0001, 4, m_ui.attenuationZ_value,
        m_ui.attenuationZ_dial, m_callbacks.SetLight[LIGHT_ATTENUATION_Z]);

    m_light[LIGHT_SPECULARITY].Initialise(0.1, 3, m_ui.specularity_value,
        m_ui.specularity_dial, m_callbacks.SetLight[LIGHT_SPECULARITY]);

    m_light[LIGHT_ACTIVE].Initialise(0.1, 3, m_ui.lightActive_value,
        m_ui.lightActive_dial, m_callbacks.SetLight[LIGHT_ACTIVE]);

    auto AddEntry = [](std::vector<ComboEntry>& container, FloatSignals& callbacks, 
                       int attribute, double step, const char* name)
    {
        container.emplace_back(attribute, name, step, 3, callbacks[attribute]);
    };

    std::vector<ComboEntry> diffuse;
    AddEntry(diffuse, m_callbacks.SetLight, LIGHT_DIFFUSE_R, 0.01, "Diffuse R");
    AddEntry(diffuse, m_callbacks.SetLight, LIGHT_DIFFUSE_G, 0.01, "Diffuse G");
    AddEntry(diffuse, m_callbacks.SetLight, LIGHT_DIFFUSE_B, 0.01, "Diffuse B");
    m_lightDiffuse.Initialise(m_ui.light_diffuse_box, 
        m_ui.light_diffuse_value, m_ui.light_diffuse_dial, diffuse);

    std::vector<ComboEntry> specular;
    AddEntry(specular, m_callbacks.SetLight, LIGHT_SPECULAR_R, 0.01, "Specular R");
    AddEntry(specular, m_callbacks.SetLight, LIGHT_SPECULAR_G, 0.01, "Specular G");
    AddEntry(specular, m_callbacks.SetLight, LIGHT_SPECULAR_B, 0.01, "Specular B");
    m_lightSpecular.Initialise(m_ui.light_specular_box, 
        m_ui.light_specular_value, m_ui.light_specular_dial, specular);

    std::vector<ComboEntry> mesh;
    AddEntry(mesh, m_callbacks.SetMesh, MESH_AMBIENCE, 0.01, "Ambience");
    AddEntry(mesh, m_callbacks.SetMesh, MESH_BUMP, 0.01, "Bump");
    AddEntry(mesh, m_callbacks.SetMesh, MESH_SPECULARITY, 0.01, "Specularity");
    m_mesh.Initialise(m_ui.meshAttributes_box, 
        m_ui.meshAttributes_value, m_ui.meshAttributes_dial, mesh);

    std::vector<ComboEntry> minColour;
    AddEntry(minColour, m_callbacks.SetMinColour, RED, 0.01, "Min Red");
    AddEntry(minColour, m_callbacks.SetMinColour, GREEN, 0.01, "Min Green");
    AddEntry(minColour, m_callbacks.SetMinColour, BLUE, 0.01, "Min Blue");
    m_minColour.Initialise(m_ui.minRange_box, 
        m_ui.minRange_value, m_ui.minRange_dial, minColour);

    std::vector<ComboEntry> maxColour;
    AddEntry(maxColour, m_callbacks.SetMaxColour, RED, 0.01, "Max Red");
    AddEntry(maxColour, m_callbacks.SetMaxColour, GREEN, 0.01, "Max Green");
    AddEntry(maxColour, m_callbacks.SetMaxColour, BLUE, 0.01, "Max Blue");
    m_maxColour.Initialise(m_ui.maxRange_box, 
        m_ui.maxRange_value, m_ui.maxRange_dial, maxColour);

    std::vector<ComboEntry> fog;
    AddEntry(fog, m_callbacks.SetFog, FOG_FADE, 0.01, "Fog Fade");
    AddEntry(fog, m_callbacks.SetFog, FOG_DISTANCE, 0.01, "Fog Start");
    AddEntry(fog, m_callbacks.SetFog, FOG_RED, 0.01, "Fog Red");
    AddEntry(fog, m_callbacks.SetFog, FOG_GREEN, 0.01, "Fog Green");
    AddEntry(fog, m_callbacks.SetFog, FOG_BLUE, 0.01, "Fog Blue");
    m_fog.Initialise(m_ui.fog_box, m_ui.fog_value, m_ui.fog_dial, fog);

    std::vector<ComboEntry> emitter;
    AddEntry(emitter, m_callbacks.SetEmitter, EMITTER_LIFETIME, 0.01, "Lifetime");
    AddEntry(emitter, m_callbacks.SetEmitter, EMITTER_LIFEFADE, 0.01, "Lifefade");
    AddEntry(emitter, m_callbacks.SetEmitter, EMITTER_LENGTH, 0.01, "Length");
    AddEntry(emitter, m_callbacks.SetEmitter, EMITTER_WIDTH, 0.01, "Width");
    AddEntry(emitter, m_callbacks.SetEmitter, EMITTER_DIR_X, 0.01, "Direction X");
    AddEntry(emitter, m_callbacks.SetEmitter, EMITTER_DIR_Y, 0.01, "Direction Y");
    AddEntry(emitter, m_callbacks.SetEmitter, EMITTER_DIR_Z, 0.01, "Direction Z");
    AddEntry(emitter, m_callbacks.SetEmitter, EMITTER_POS_X, 0.1, "Position X");
    AddEntry(emitter, m_callbacks.SetEmitter, EMITTER_POS_Y, 0.1, "Position Y");
    AddEntry(emitter, m_callbacks.SetEmitter, EMITTER_POS_Z, 0.1, "Position Z");
    m_emitter.Initialise(m_ui.emitter_box, 
        m_ui.emitter_value, m_ui.emitter_dial, emitter);

    std::vector<ComboEntry> emitterColor;
    AddEntry(emitterColor, m_callbacks.SetEmitter, EMITTER_TINT_R, 0.01, "Tint R");
    AddEntry(emitterColor, m_callbacks.SetEmitter, EMITTER_TINT_G, 0.01, "Tint G");
    AddEntry(emitterColor, m_callbacks.SetEmitter, EMITTER_TINT_B, 0.01, "Tint B");
    m_emitterColour.Initialise(m_ui.emitterColour_box,
        m_ui.emitterColour_value, m_ui.emitterColour_dial, emitterColor);

    std::vector<ComboEntry> emitterMinMax;
    AddEntry(emitterMinMax, m_callbacks.SetEmitter, EMITTER_MAX_SPEED, 0.01, "Max Speed");
    AddEntry(emitterMinMax, m_callbacks.SetEmitter, EMITTER_MIN_SPEED, 0.01, "Min Speed");
    AddEntry(emitterMinMax, m_callbacks.SetEmitter, EMITTER_MAX_SIZE, 0.01, "Max Size");
    AddEntry(emitterMinMax, m_callbacks.SetEmitter, EMITTER_MIN_SIZE, 0.01, "Min Size");
    AddEntry(emitterMinMax, m_callbacks.SetEmitter, EMITTER_MAX_AMP, 0.01, "Max Amp");
    AddEntry(emitterMinMax, m_callbacks.SetEmitter, EMITTER_MIN_AMP, 0.01, "Min Amp");
    AddEntry(emitterMinMax, m_callbacks.SetEmitter, EMITTER_MAX_FREQ, 0.01, "Max Freq");
    AddEntry(emitterMinMax, m_callbacks.SetEmitter, EMITTER_MIN_FREQ, 0.01, "Min Freq");
    AddEntry(emitterMinMax, m_callbacks.SetEmitter, EMITTER_MAX_WAVE, 0.01, "Max Wave Sp");
    AddEntry(emitterMinMax, m_callbacks.SetEmitter, EMITTER_MIN_WAVE, 0.01, "Min Wave Sp");
    m_emitterMinMax.Initialise(m_ui.emitterMinMax_box, 
        m_ui.emitterMinMax_value, m_ui.emitterMinMax_dial, emitterMinMax);

    std::vector<ComboEntry> water;
    AddEntry(water, m_callbacks.SetWater, WATER_SPEED, 0.01, "Speed");
    AddEntry(water, m_callbacks.SetWater, WATER_SHALLOW_R, 0.01, "Shallow R");
    AddEntry(water, m_callbacks.SetWater, WATER_SHALLOW_G, 0.01, "Shallow G");
    AddEntry(water, m_callbacks.SetWater, WATER_SHALLOW_B, 0.01, "Shallow B");
    AddEntry(water, m_callbacks.SetWater, WATER_SHALLOW_A, 0.01, "Shallow A");
    AddEntry(water, m_callbacks.SetWater, WATER_DEEP_R, 0.01, "Deep R");
    AddEntry(water, m_callbacks.SetWater, WATER_DEEP_G, 0.01, "Deep G");
    AddEntry(water, m_callbacks.SetWater, WATER_DEEP_B, 0.01, "Deep B");
    AddEntry(water, m_callbacks.SetWater, WATER_DEEP_A, 0.01, "Deep A");
    AddEntry(water, m_callbacks.SetWater, WATER_REFLECTION_R, 0.01, "Reflection R");
    AddEntry(water, m_callbacks.SetWater, WATER_REFLECTION_G, 0.01, "Reflection G");
    AddEntry(water, m_callbacks.SetWater, WATER_REFLECTION_B, 0.01, "Reflection B");
    AddEntry(water, m_callbacks.SetWater, WATER_REFLECTION, 0.01, "Reflection");
    AddEntry(water, m_callbacks.SetWater, WATER_BUMP, 0.01, "Bump");
    AddEntry(water, m_callbacks.SetWater, WATER_BUMP_VELOCITY_X, 0.01, "Bump Vel X");
    AddEntry(water, m_callbacks.SetWater, WATER_BUMP_VELOCITY_Y, 0.01, "Bump Vel Y");
    AddEntry(water, m_callbacks.SetWater, WATER_SCALE_U, 0.01, "Scale U");
    AddEntry(water, m_callbacks.SetWater, WATER_SCALE_V, 0.01, "Scale V");
    AddEntry(water, m_callbacks.SetWater, WATER_FRESNAL_SCALE, 0.01, "Fresnal Scale");
    AddEntry(water, m_callbacks.SetWater, WATER_FRESNAL_BIAS, 0.01, "Fresnal Bias");
    AddEntry(water, m_callbacks.SetWater, WATER_FRESNAL_POWER, 0.01, "Fresnal Pow");
    m_water.Initialise(m_ui.water_box, m_ui.water_value, m_ui.water_dial, water);

    std::vector<ComboEntry> wave;
    AddEntry(wave, m_callbacks.SetWave, WAVE_AMPLITUDE, 0.01, "Amplitude");
    AddEntry(wave, m_callbacks.SetWave, WAVE_FREQUENCY, 0.01, "Frequency");
    AddEntry(wave, m_callbacks.SetWave, WAVE_PHASE, 0.01, "Phase");
    AddEntry(wave, m_callbacks.SetWave, WAVE_DIRECTION_X, 0.01, "Direction X");
    AddEntry(wave, m_callbacks.SetWave, WAVE_DIRECTION_Z, 0.01, "Direction Z");
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

void Tweaker::SetPost(PostAttribute attribute, float value)
{
    m_post.at(attribute).Set(value);
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
