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

    m_reloadScene.Initialise(m_ui.reloadScene_btn, m_callbacks.ReloadScene);
    m_saveScene.Initialise(m_ui.saveScene_btn, m_callbacks.SaveScene);
    m_savePost.Initialise(m_ui.savePost_btn, m_callbacks.SavePost);

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

    m_light[LIGHT_POSITION_X].Initialise(1.0, m_ui.positionX_value,
        m_ui.positionX_dial, m_callbacks.SetLight[LIGHT_POSITION_X]);

    m_light[LIGHT_POSITION_Y].Initialise(1.0, m_ui.positionY_value,
        m_ui.positionY_dial, m_callbacks.SetLight[LIGHT_POSITION_Y]);

    m_light[LIGHT_POSITION_Z].Initialise(1.0, m_ui.positionZ_value,
        m_ui.positionZ_dial, m_callbacks.SetLight[LIGHT_POSITION_Z]);

    m_light[LIGHT_ATTENUATION_X].Initialise(0.01, m_ui.attenuationX_value,
        m_ui.attenuationX_dial, m_callbacks.SetLight[LIGHT_ATTENUATION_X]);

    m_light[LIGHT_ATTENUATION_Y].Initialise(0.001, m_ui.attenuationY_value,
        m_ui.attenuationY_dial, m_callbacks.SetLight[LIGHT_ATTENUATION_Y]);

    m_light[LIGHT_ATTENUATION_Z].Initialise(0.0001, m_ui.attenuationZ_value,
        m_ui.attenuationZ_dial, m_callbacks.SetLight[LIGHT_ATTENUATION_Z]);

    m_light[LIGHT_DIFFUSE_R].Initialise(0.01, m_ui.diffuseRed_value,
        m_ui.diffuseRed_dial, m_callbacks.SetLight[LIGHT_DIFFUSE_R]);

    m_light[LIGHT_DIFFUSE_G].Initialise(0.01, m_ui.diffuseGreen_value,
        m_ui.diffuseGreen_dial, m_callbacks.SetLight[LIGHT_DIFFUSE_G]);

    m_light[LIGHT_DIFFUSE_B].Initialise(0.01, m_ui.diffuseBlue_value,
        m_ui.diffuseBlue_dial, m_callbacks.SetLight[LIGHT_DIFFUSE_B]);

    m_light[LIGHT_SPECULAR_R].Initialise(0.01, m_ui.specularRed_value,
        m_ui.specularRed_dial, m_callbacks.SetLight[LIGHT_SPECULAR_R]);

    m_light[LIGHT_SPECULAR_G].Initialise(0.01, m_ui.specularGreen_value,
        m_ui.specularGreen_dial, m_callbacks.SetLight[LIGHT_SPECULAR_G]);

    m_light[LIGHT_SPECULAR_B].Initialise(0.01, m_ui.specularBlue_value,
        m_ui.specularBlue_dial, m_callbacks.SetLight[LIGHT_SPECULAR_B]);

    m_light[LIGHT_SPECULARITY].Initialise(0.1, m_ui.lightSpecularity_value,
        m_ui.lightSpecularity_dial, m_callbacks.SetLight[LIGHT_SPECULARITY]);

    std::vector<ComboEntry> mesh(MESH_ATTRIBUTES);
    mesh[MESH_AMBIENCE].Set("Ambience", 0.01, m_callbacks.SetMesh[MESH_AMBIENCE]);
    mesh[MESH_BUMP].Set("Bump", 0.01, m_callbacks.SetMesh[MESH_BUMP]);
    mesh[MESH_GLOW].Set("Glow", 0.01, m_callbacks.SetMesh[MESH_GLOW]);
    mesh[MESH_SPECULARITY].Set("Specularity", 0.01, m_callbacks.SetMesh[MESH_SPECULARITY]);
    m_mesh.Initialise(m_ui.meshAttributes_box,
        m_ui.meshAttributes_value, m_ui.meshAttributes_dial, mesh);

    std::vector<ComboEntry> minimumColour(COLOUR_ATTRIBUTES);
    minimumColour[RED].Set("Min Red", 0.01, m_callbacks.SetMinColour[RED]);
    minimumColour[GREEN].Set("Min Green", 0.01, m_callbacks.SetMinColour[GREEN]);
    minimumColour[BLUE].Set("Min Blue", 0.01, m_callbacks.SetMinColour[BLUE]);
    m_minColour.Initialise(m_ui.minRange_box,
        m_ui.minRange_value, m_ui.minRange_dial, minimumColour);

    std::vector<ComboEntry> maximumColour(COLOUR_ATTRIBUTES);
    maximumColour[RED].Set("Max Red", 0.01, m_callbacks.SetMaxColour[RED]);
    maximumColour[GREEN].Set("Max Green", 0.01, m_callbacks.SetMaxColour[GREEN]);
    maximumColour[BLUE].Set("Max Blue", 0.01, m_callbacks.SetMaxColour[BLUE]);
    m_maxColour.Initialise(m_ui.maxRange_box,
        m_ui.maxRange_value, m_ui.maxRange_dial, maximumColour);

    std::vector<ComboEntry> fog(FOG_ATTRIBUTES);
    fog[FOG_DISTANCE].Set("Fog Start", 0.01, m_callbacks.SetFog[FOG_DISTANCE]);
    fog[FOG_FADE].Set("Fog Fade", 0.01, m_callbacks.SetFog[FOG_FADE]);
    fog[FOG_RED].Set("Fog Red", 0.01, m_callbacks.SetFog[FOG_RED]);
    fog[FOG_GREEN].Set("Fog Green", 0.01, m_callbacks.SetFog[FOG_GREEN]);
    fog[FOG_BLUE].Set("Fog Blue", 0.01, m_callbacks.SetFog[FOG_BLUE]);
    m_fog.Initialise(m_ui.fog_box, m_ui.fog_value, m_ui.fog_dial, fog);

    std::vector<ComboEntry> emitter(EMITTER_ATTRIBUTES);
    emitter[EMITTER_LENGTH].Set("Length", 0.01, m_callbacks.SetEmitter[EMITTER_LENGTH]);
    emitter[EMITTER_WIDTH].Set("Width", 0.01, m_callbacks.SetEmitter[EMITTER_WIDTH]);
    emitter[EMITTER_DIR_X].Set("Direction X", 0.01, m_callbacks.SetEmitter[EMITTER_DIR_X]);
    emitter[EMITTER_DIR_Y].Set("Direction Y", 0.01, m_callbacks.SetEmitter[EMITTER_DIR_Y]);
    emitter[EMITTER_DIR_Z].Set("Direction Z", 0.01, m_callbacks.SetEmitter[EMITTER_DIR_Z]);
    emitter[EMITTER_POS_X].Set("Position X", 0.01, m_callbacks.SetEmitter[EMITTER_POS_X]);
    emitter[EMITTER_POS_Y].Set("Position Y", 0.01, m_callbacks.SetEmitter[EMITTER_POS_Y]);
    emitter[EMITTER_POS_Z].Set("Position Z", 0.01, m_callbacks.SetEmitter[EMITTER_POS_Z]);
    m_emitter.Initialise(m_ui.emitter_box, m_ui.emitter_value, m_ui.emitter_dial, emitter);

    std::vector<ComboEntry> particle(PARTICLE_ATTRIBUTES);
    particle[PARTICLE_LIFETIME].Set("Lifetime", 0.01, m_callbacks.SetParticles[PARTICLE_LIFETIME]);
    particle[PARTICLE_SPEED].Set("Speed", 0.01, m_callbacks.SetParticles[PARTICLE_SPEED]);
    particle[PARTICLE_SPEED_VAR].Set("Speed Var.", 0.01, m_callbacks.SetParticles[PARTICLE_SPEED_VAR]);
    particle[PARTICLE_SIZE].Set("Size", 0.01, m_callbacks.SetParticles[PARTICLE_SIZE]);
    particle[PARTICLE_SIZE_VAR].Set("Size Var.", 0.01, m_callbacks.SetParticles[PARTICLE_SIZE_VAR]);
    particle[PARTICLE_TINT_R].Set("Tint R", 0.01, m_callbacks.SetParticles[PARTICLE_TINT_R]);
    particle[PARTICLE_TINT_G].Set("Tint G", 0.01, m_callbacks.SetParticles[PARTICLE_TINT_G]);
    particle[PARTICLE_TINT_B].Set("Tint B", 0.01, m_callbacks.SetParticles[PARTICLE_TINT_B]);
    m_particles.Initialise(m_ui.particle_box, m_ui.particle_value, m_ui.particle_dial, particle);

    std::vector<ComboEntry> water(WATER_ATTRIBUTES);
    water[WATER_SHALLOW_R].Set("Shallow R", 0.01, m_callbacks.SetWater[WATER_SHALLOW_R]);
    water[WATER_SHALLOW_G].Set("Shallow G", 0.01, m_callbacks.SetWater[WATER_SHALLOW_G]);
    water[WATER_SHALLOW_B].Set("Shallow B", 0.01, m_callbacks.SetWater[WATER_SHALLOW_B]);
    water[WATER_DEEP_R].Set("Deep R", 0.01, m_callbacks.SetWater[WATER_DEEP_R]);
    water[WATER_DEEP_G].Set("Deep G", 0.01, m_callbacks.SetWater[WATER_DEEP_G]);
    water[WATER_DEEP_B].Set("Deep B", 0.01, m_callbacks.SetWater[WATER_DEEP_B]);
    water[WATER_REFLECTION_R].Set("Reflection R", 0.01, m_callbacks.SetWater[WATER_REFLECTION_R]);
    water[WATER_REFLECTION_G].Set("Reflection G", 0.01, m_callbacks.SetWater[WATER_REFLECTION_G]);
    water[WATER_REFLECTION_B].Set("Reflection B", 0.01, m_callbacks.SetWater[WATER_REFLECTION_B]);
    water[WATER_REFLECTION].Set("Reflection", 0.01, m_callbacks.SetWater[WATER_REFLECTION]);
    water[WATER_BUMP].Set("Bump", 0.01, m_callbacks.SetWater[WATER_BUMP]);
    water[WATER_BUMP_VELOCITY_X].Set("Bump Vel X", 0.01, m_callbacks.SetWater[WATER_BUMP_VELOCITY_X]);
    water[WATER_BUMP_VELOCITY_Y].Set("Bump Vel Y", 0.01, m_callbacks.SetWater[WATER_BUMP_VELOCITY_Y]);
    water[WATER_SPEED].Set("Speed", 0.01, m_callbacks.SetWater[WATER_SPEED]);
    water[WATER_SCALE_U].Set("Scale U", 0.01, m_callbacks.SetWater[WATER_SCALE_U]);
    water[WATER_SCALE_V].Set("Scale V", 0.01, m_callbacks.SetWater[WATER_SCALE_V]);
    water[WATER_FRESNAL_FACTOR].Set("Fresnal", 0.01, m_callbacks.SetWater[WATER_FRESNAL_FACTOR]);
    m_water.Initialise(m_ui.water_box, m_ui.water_value, m_ui.water_dial, water);

    std::vector<ComboEntry> wave(WAVE_ATTRIBUTES);
    wave[WAVE_AMPLITUDE].Set("Amplitude", 0.01, m_callbacks.SetWave[WAVE_AMPLITUDE]);
    wave[WAVE_FREQUENCY].Set("Frequency", 0.01, m_callbacks.SetWave[WAVE_FREQUENCY]);
    wave[WAVE_PHASE].Set("Phase", 0.01, m_callbacks.SetWave[WAVE_PHASE]);
    wave[WAVE_DIRECTION_X].Set("Direction X", 0.01, m_callbacks.SetWave[WAVE_DIRECTION_X]);
    wave[WAVE_DIRECTION_Z].Set("Direction Z", 0.01, m_callbacks.SetWave[WAVE_DIRECTION_Z]);
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
    m_camera[attribute].Set(value);
}

void Tweaker::SetLight(LightAttribute attribute, float value)
{
    m_light[attribute].Set(value);
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
    m_emitter.SetValue(attribute, value);
}

void Tweaker::SetParticles(ParticleAttribute attribute, float value)
{
    m_particles.SetValue(attribute, value);
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
