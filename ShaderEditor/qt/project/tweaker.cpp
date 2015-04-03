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

    m_lightDiag.Initialise(m_ui.light_diagnostics_btn, m_callbacks.LightDiagnostics);
    m_reloadScene.Initialise(m_ui.reloadScene_btn, m_callbacks.ReloadScene);
    m_reloadEngine.Initialise(m_ui.reloadEngine_btn, m_callbacks.ReloadEngine);
    m_reloadTerrain.Initialise(m_ui.reloadTerrain_btn, m_callbacks.ReloadTerrain);
    m_reloadTexture.Initialise(m_ui.reloadTexture_btn, m_callbacks.ReloadTexture);
    m_toggleWireframe.Initialise(m_ui.toggleWireframe_btn, m_callbacks.ToggleWireframe);
    m_saveScene.Initialise(m_ui.saveScene_btn, m_callbacks.SaveScene);
    m_pauseEmission.Initialise(m_ui.pauseEmission_btn, m_callbacks.PauseEmission);
    m_lightsOnly.Initialise(m_ui.light_render_only_btn, m_callbacks.RenderLightsOnly);

    m_ui.particleAmount_value->setMinimum(0.0);
    m_ui.particleAmount_value->setMaximum(500.0);
    m_particleAmount.Initialise(1.0, 0, m_ui.particleAmount_value,
        m_ui.particleAmount_dial, m_callbacks.SetParticleAmount);

    m_ui.waveNumber_value->setMinimum(0.0);
    m_selectedWave.Initialise(1.0, 0, m_ui.waveNumber_value,
        m_ui.waveNumber_dial, m_callbacks.SetSelectedWave);

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

    m_light[LIGHT_ACTIVE].Initialise(0.1, 3, m_ui.light_active_value,
        m_ui.light_active_dial, m_callbacks.SetLight[LIGHT_ACTIVE]);

    auto AddEntry = [](std::vector<ComboEntry>& container, FloatSignals& callbacks, 
                       int attribute, double step, int precision, const char* name)
    {
        container.emplace_back(attribute, name, step, precision, callbacks[attribute]);
    };

    std::vector<ComboEntry> terrain;
    AddEntry(terrain, m_callbacks.SetTerrain, TERRAIN_BUMP, 0.01, 3, "Bump");
    AddEntry(terrain, m_callbacks.SetTerrain, TERRAIN_AMBIENCE, 0.01, 3, "Ambience");
    AddEntry(terrain, m_callbacks.SetTerrain, TERRAIN_CAUSTICS, 0.01, 3, "Caustics");
    AddEntry(terrain, m_callbacks.SetTerrain, TERRAIN_SPECULARITY, 0.01, 3, "Specularity");
    AddEntry(terrain, m_callbacks.SetTerrain, TERRAIN_MIN_HEIGHT, 0.01, 3, "Min Height");
    AddEntry(terrain, m_callbacks.SetTerrain, TERRAIN_MAX_HEIGHT, 0.01, 3, "Max Height");
    m_terrain.Initialise(m_ui.terrainAttributes_box, 
        m_ui.terrainAttributes_value, m_ui.terrainAttributes_dial, terrain);

    std::vector<ComboEntry> post;
    AddEntry(post, m_callbacks.SetPost, POST_DEPTH_NEAR, 0.1, 2, "Depth Near");
    AddEntry(post, m_callbacks.SetPost, POST_DEPTH_FAR, 10.0, 1, "Depth Far");
    AddEntry(post, m_callbacks.SetPost, POST_BLUR_STEP, 0.001, 3, "Blur Step");
    AddEntry(post, m_callbacks.SetPost, POST_CAUSTIC_SPEED, 0.001, 3, "Caustic Speed");
    AddEntry(post, m_callbacks.SetPost, POST_BLOOM_AMOUNT, 0.001, 3, "Bloom Scale");
    AddEntry(post, m_callbacks.SetPost, POST_BLOOM_START, 0.001, 3, "Bloom Start");
    AddEntry(post, m_callbacks.SetPost, POST_BLOOM_FADE, 0.001, 3, "Bloom Fade");
    AddEntry(post, m_callbacks.SetPost, POST_DOF_START, 0.01, 3, "DOF Start");
    AddEntry(post, m_callbacks.SetPost, POST_DOF_FADE, 0.01, 3, "DOF Fade");
    m_post.Initialise(m_ui.post_box, m_ui.post_value, m_ui.post_dial, post);

    std::vector<ComboEntry> correction;
    AddEntry(correction, m_callbacks.SetPost, POST_CONTRAST, 0.01, 3, "Contrast");
    AddEntry(correction, m_callbacks.SetPost, POST_SATURATION, 0.01, 3, "Saturation");
    AddEntry(correction, m_callbacks.SetPost, POST_MIN_RED, 0.01, 3, "Min R");
    AddEntry(correction, m_callbacks.SetPost, POST_MIN_GREEN, 0.01, 3, "Min G");
    AddEntry(correction, m_callbacks.SetPost, POST_MIN_BLUE, 0.01, 3, "Min B");
    AddEntry(correction, m_callbacks.SetPost, POST_MAX_RED, 0.01, 3, "Max R");
    AddEntry(correction, m_callbacks.SetPost, POST_MAX_GREEN, 0.01, 3, "Max G");
    AddEntry(correction, m_callbacks.SetPost, POST_MAX_BLUE, 0.01, 3, "Max B");
    m_correction.Initialise(m_ui.correction_box, 
        m_ui.correction_value, m_ui.correction_dial, correction);

    std::vector<ComboEntry> fog;
    AddEntry(fog, m_callbacks.SetPost, POST_FOG_FADE, 0.01, 3, "Fog Fade");
    AddEntry(fog, m_callbacks.SetPost, POST_FOG_START, 0.01, 3, "Fog Start");
    AddEntry(fog, m_callbacks.SetPost, POST_FOG_RED, 0.01, 3, "Fog Red");
    AddEntry(fog, m_callbacks.SetPost, POST_FOG_GREEN, 0.01, 3, "Fog Green");
    AddEntry(fog, m_callbacks.SetPost, POST_FOG_BLUE, 0.01, 3, "Fog Blue");
    m_fog.Initialise(m_ui.fog_box, m_ui.fog_value, m_ui.fog_dial, fog);

    std::vector<ComboEntry> lightAtt;
    AddEntry(lightAtt, m_callbacks.SetLight, LIGHT_ATTENUATION_X, 0.001, 4, "Fade X");
    AddEntry(lightAtt, m_callbacks.SetLight, LIGHT_ATTENUATION_Y, 0.001, 4, "Fade Y");
    AddEntry(lightAtt, m_callbacks.SetLight, LIGHT_ATTENUATION_Z, 0.001, 4, "Fade Z");
    m_lightAtt.Initialise(m_ui.light_attenuation_box, 
        m_ui.light_attenuation_value, m_ui.light_attenuation_dial, lightAtt);

    std::vector<ComboEntry> lightPosition;
    AddEntry(lightPosition, m_callbacks.SetLight, LIGHT_POSITION_X, 0.1, 3, "Position X");
    AddEntry(lightPosition, m_callbacks.SetLight, LIGHT_POSITION_Y, 0.1, 3, "Position Y");
    AddEntry(lightPosition, m_callbacks.SetLight, LIGHT_POSITION_Z, 0.1, 3, "Position Z");
    m_lightPosition.Initialise(m_ui.light_position_box, 
        m_ui.light_position_value, m_ui.light_position_dial, lightPosition);

    std::vector<ComboEntry> lightColour;
    AddEntry(lightColour, m_callbacks.SetLight, LIGHT_DIFFUSE_R, 0.01, 3, "Diffuse R");
    AddEntry(lightColour, m_callbacks.SetLight, LIGHT_DIFFUSE_G, 0.01, 3, "Diffuse G");
    AddEntry(lightColour, m_callbacks.SetLight, LIGHT_DIFFUSE_B, 0.01, 3, "Diffuse B");
    AddEntry(lightColour, m_callbacks.SetLight, LIGHT_SPECULAR_R, 0.01, 3, "Specular R");
    AddEntry(lightColour, m_callbacks.SetLight, LIGHT_SPECULAR_G, 0.01, 3, "Specular G");
    AddEntry(lightColour, m_callbacks.SetLight, LIGHT_SPECULAR_B, 0.01, 3, "Specular B");
    AddEntry(lightColour, m_callbacks.SetLight, LIGHT_SPECULARITY, 0.01, 3, "Specularity");
    m_lightColour.Initialise(m_ui.light_colour_box, 
        m_ui.light_colour_value, m_ui.light_colour_dial, lightColour);

    std::vector<ComboEntry> mesh;
    AddEntry(mesh, m_callbacks.SetMesh, MESH_AMBIENCE, 0.01, 3, "Ambience");
    AddEntry(mesh, m_callbacks.SetMesh, MESH_CAUSTICS, 0.01, 3, "Caustics");
    AddEntry(mesh, m_callbacks.SetMesh, MESH_BUMP, 0.01, 3, "Bump");
    AddEntry(mesh, m_callbacks.SetMesh, MESH_SPECULARITY, 0.01, 3, "Specularity");
    m_mesh.Initialise(m_ui.meshAttributes_box, 
        m_ui.meshAttributes_value, m_ui.meshAttributes_dial, mesh);

    std::vector<ComboEntry> emitter;
    AddEntry(emitter, m_callbacks.SetEmitter, EMITTER_LIFETIME, 0.01, 3, "Lifetime");
    AddEntry(emitter, m_callbacks.SetEmitter, EMITTER_LIFEFADE, 0.01, 3, "Lifefade");
    AddEntry(emitter, m_callbacks.SetEmitter, EMITTER_LENGTH, 0.01, 3, "Length");
    AddEntry(emitter, m_callbacks.SetEmitter, EMITTER_WIDTH, 0.01, 3, "Width");
    AddEntry(emitter, m_callbacks.SetEmitter, EMITTER_DIR_X, 0.01, 3, "Direction X");
    AddEntry(emitter, m_callbacks.SetEmitter, EMITTER_DIR_Y, 0.01, 3, "Direction Y");
    AddEntry(emitter, m_callbacks.SetEmitter, EMITTER_DIR_Z, 0.01, 3, "Direction Z");
    AddEntry(emitter, m_callbacks.SetEmitter, EMITTER_POS_X, 0.1, 3, "Position X");
    AddEntry(emitter, m_callbacks.SetEmitter, EMITTER_POS_Y, 0.1, 3, "Position Y");
    AddEntry(emitter, m_callbacks.SetEmitter, EMITTER_POS_Z, 0.1, 3, "Position Z");
    m_emitter.Initialise(m_ui.emitter_box, 
        m_ui.emitter_value, m_ui.emitter_dial, emitter);

    std::vector<ComboEntry> emitterColor;
    AddEntry(emitterColor, m_callbacks.SetEmitter, EMITTER_TINT_R, 0.01, 3, "Tint R");
    AddEntry(emitterColor, m_callbacks.SetEmitter, EMITTER_TINT_G, 0.01, 3, "Tint G");
    AddEntry(emitterColor, m_callbacks.SetEmitter, EMITTER_TINT_B, 0.01, 3, "Tint B");
    m_emitterColour.Initialise(m_ui.emitterColour_box,
        m_ui.emitterColour_value, m_ui.emitterColour_dial, emitterColor);

    std::vector<ComboEntry> emitterMinMax;
    AddEntry(emitterMinMax, m_callbacks.SetEmitter, EMITTER_MAX_SPEED, 0.01, 3, "Max Speed");
    AddEntry(emitterMinMax, m_callbacks.SetEmitter, EMITTER_MIN_SPEED, 0.01, 3, "Min Speed");
    AddEntry(emitterMinMax, m_callbacks.SetEmitter, EMITTER_MAX_SIZE, 0.01, 3, "Max Size");
    AddEntry(emitterMinMax, m_callbacks.SetEmitter, EMITTER_MIN_SIZE, 0.01, 3, "Min Size");
    AddEntry(emitterMinMax, m_callbacks.SetEmitter, EMITTER_MAX_AMP, 0.01, 3, "Max Amp");
    AddEntry(emitterMinMax, m_callbacks.SetEmitter, EMITTER_MIN_AMP, 0.01, 3, "Min Amp");
    AddEntry(emitterMinMax, m_callbacks.SetEmitter, EMITTER_MAX_FREQ, 0.01, 3, "Max Freq");
    AddEntry(emitterMinMax, m_callbacks.SetEmitter, EMITTER_MIN_FREQ, 0.01, 3, "Min Freq");
    AddEntry(emitterMinMax, m_callbacks.SetEmitter, EMITTER_MAX_WAVE, 0.01, 3, "Max Wave Sp");
    AddEntry(emitterMinMax, m_callbacks.SetEmitter, EMITTER_MIN_WAVE, 0.01, 3, "Min Wave Sp");
    m_emitterMinMax.Initialise(m_ui.emitterMinMax_box, 
        m_ui.emitterMinMax_value, m_ui.emitterMinMax_dial, emitterMinMax);

    std::vector<ComboEntry> water;
    AddEntry(water, m_callbacks.SetWater, WATER_SPEED, 0.01, 3, "Speed");
    AddEntry(water, m_callbacks.SetWater, WATER_SHALLOW_R, 0.01, 3, "Shallow R");
    AddEntry(water, m_callbacks.SetWater, WATER_SHALLOW_G, 0.01, 3, "Shallow G");
    AddEntry(water, m_callbacks.SetWater, WATER_SHALLOW_B, 0.01, 3, "Shallow B");
    AddEntry(water, m_callbacks.SetWater, WATER_SHALLOW_A, 0.01, 3, "Shallow A");
    AddEntry(water, m_callbacks.SetWater, WATER_DEEP_R, 0.01, 3, "Deep R");
    AddEntry(water, m_callbacks.SetWater, WATER_DEEP_G, 0.01, 3, "Deep G");
    AddEntry(water, m_callbacks.SetWater, WATER_DEEP_B, 0.01, 3, "Deep B");
    AddEntry(water, m_callbacks.SetWater, WATER_DEEP_A, 0.01, 3, "Deep A");
    AddEntry(water, m_callbacks.SetWater, WATER_REFLECTION_R, 0.01, 3, "Reflection R");
    AddEntry(water, m_callbacks.SetWater, WATER_REFLECTION_G, 0.01, 3, "Reflection G");
    AddEntry(water, m_callbacks.SetWater, WATER_REFLECTION_B, 0.01, 3, "Reflection B");
    AddEntry(water, m_callbacks.SetWater, WATER_REFLECTION, 0.01, 3, "Reflection");
    AddEntry(water, m_callbacks.SetWater, WATER_BUMP, 0.01, 3, "Bump");
    AddEntry(water, m_callbacks.SetWater, WATER_BUMP_VELOCITY_X, 0.01, 3, "Bump Vel X");
    AddEntry(water, m_callbacks.SetWater, WATER_BUMP_VELOCITY_Y, 0.01, 3, "Bump Vel Y");
    AddEntry(water, m_callbacks.SetWater, WATER_SCALE_U, 0.01, 3, "Scale U");
    AddEntry(water, m_callbacks.SetWater, WATER_SCALE_V, 0.01, 3, "Scale V");
    AddEntry(water, m_callbacks.SetWater, WATER_FRESNAL_SCALE, 0.01, 3, "Fresnal Scale");
    AddEntry(water, m_callbacks.SetWater, WATER_FRESNAL_BIAS, 0.01, 3, "Fresnal Bias");
    AddEntry(water, m_callbacks.SetWater, WATER_FRESNAL_POWER, 0.01, 3, "Fresnal Pow");
    m_water.Initialise(m_ui.water_box, m_ui.water_value, m_ui.water_dial, water);

    std::vector<ComboEntry> wave;
    AddEntry(wave, m_callbacks.SetWave, WAVE_AMPLITUDE, 0.01, 3, "Amplitude");
    AddEntry(wave, m_callbacks.SetWave, WAVE_FREQUENCY, 0.01, 3, "Frequency");
    AddEntry(wave, m_callbacks.SetWave, WAVE_PHASE, 0.01, 3, "Phase");
    AddEntry(wave, m_callbacks.SetWave, WAVE_DIRECTION_X, 0.01, 3, "Direction X");
    AddEntry(wave, m_callbacks.SetWave, WAVE_DIRECTION_Z, 0.01, 3, "Direction Z");
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

void Tweaker::SetTerrainShaderName(const std::string& name)
{
    m_ui.terrainShader_text->setText(QString(name.c_str()));
    m_ui.terrainShader_text->update();
}

void Tweaker::SetTexturePath(const std::string& path)
{
    if (!path.empty())
    {
        const int buffer = 10;
        const int width = m_ui.displayTexture_btn->width();
        const int height = m_ui.displayTexture_btn->height();
        m_ui.displayTexture_btn->setIcon(QIcon(path.c_str()));
        m_ui.displayTexture_btn->setIconSize(QSize(width-buffer, height-buffer));
        m_ui.displayTexture_btn->update();
    }
}

void Tweaker::SetMeshShaderName(const std::string& name)
{
    m_ui.shader_text->setText(QString(name.c_str()));
    m_ui.shader_text->update();
}

void Tweaker::SetCamera(CameraAttribute attribute, float value)
{
    m_camera.at(attribute).Set(value);
}

void Tweaker::SetTerrain(TerrainAttribute attribute, float value)
{
    m_terrain.SetValue(attribute, value);
}

void Tweaker::SetPost(PostAttribute attribute, float value)
{
    if (!m_fog.SetValue(attribute, value) &&
        !m_correction.SetValue(attribute, value))
    {
        m_post.SetValue(attribute, value);
    }
}

void Tweaker::SetLight(LightAttribute attribute, float value)
{
    if (!m_lightColour.SetValue(attribute, value) &&
        !m_lightPosition.SetValue(attribute, value) &&
        !m_lightAtt.SetValue(attribute, value))
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

void Tweaker::InitialiseTerrain(int selected, 
                                const std::vector<std::string>& terrain)
{
    if (!terrain.empty())
    {
        m_selectedTerrain.Initialise(m_ui.selectedTerrain_box,
            selected, terrain, m_callbacks.SetSelectedTerrain);
    }
}

void Tweaker::InitialiseTextures(int selected, 
                                 const std::vector<std::string>& textures)
{
    if (!textures.empty())
    {
        m_selectedTexture.Initialise(m_ui.selectedTexture_box,
            selected, textures, m_callbacks.SetSelectedTexture);
    }
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
        m_selectedLight.Initialise(m_ui.lightSelected_box,
            selected, lights, m_callbacks.SetSelectedLight);
    }
}

bool Tweaker::HasTerrain() const
{
    return m_selectedTerrain.IsInitialised();
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

bool Tweaker::HasTextures() const
{
    return m_selectedTexture.IsInitialised();
}

bool Tweaker::HasWater() const
{
    return m_selectedWater.IsInitialised();
}

bool Tweaker::HasEngines() const
{
    return m_renderEngine.IsInitialised();
}
