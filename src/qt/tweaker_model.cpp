////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - tweaker_model.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "tweaker_model.h"
#include "attribute_model.h"
#include "stringlist_model.h"

TweakerModel::TweakerModel(QObject* parent)
    : QObject(parent)
    , m_meshAttributeModel(new AttributeModel(this))
    , m_waveAttributeModel(new AttributeModel(this))
    , m_waterAttributeModel(new AttributeModel(this))
    , m_lightAttributeModel(new AttributeModel(this))
    , m_textureAttributeModel(new AttributeModel(this))
    , m_emitterAttributeModel(new AttributeModel(this))
    , m_terrainAttributeModel(new AttributeModel(this))
    , m_postAttributeModel(new AttributeModel(this))
    , m_cameraAttributeModel(new AttributeModel(this))
    , m_enginesModel(new StringListModel(this))
    , m_meshesModel(new StringListModel(this))
    , m_wavesModel(new StringListModel(this))
    , m_waterModel(new StringListModel(this))
    , m_lightsModel(new StringListModel(this))
    , m_texturesModel(new StringListModel(this))
    , m_emittersModel(new StringListModel(this))
    , m_terrainModel(new StringListModel(this))
    , m_postMapsModel(new StringListModel(this))
{
    /**
    m_ui.waveNumber_value->setMinimum(0.0);
    m_selectedWave.Initialise(1.0, 0, m_ui.waveNumber_value,
        m_ui.waveNumber_dial, m_callbacks.SetSelectedWave);

    m_camera[Camera::PositionX].Initialise(0.1, 3, m_ui.cameraX_value, nullptr, nullptr);
    m_camera[Camera::PositionY].Initialise(0.1, 3, m_ui.cameraY_value, nullptr, nullptr);
    m_camera[Camera::PositionZ].Initialise(0.1, 3, m_ui.cameraZ_value, nullptr, nullptr);
    m_camera[Camera::Pitch].Initialise(0.01, 3, m_ui.cameraPitch_value, nullptr, nullptr);
    m_camera[Camera::Yaw].Initialise(0.01, 3, m_ui.cameraYaw_value, nullptr, nullptr);
    m_camera[Camera::Roll].Initialise(0.01, 3, m_ui.cameraRoll_value, nullptr, nullptr);

    m_camera[Camera::ForwardSpd].Initialise(0.01, 3, m_ui.cameraForwardSpeed_value, 
        m_ui.cameraForwardSpeed_dial, m_callbacks.SetCamera[Camera::ForwardSpd]);

    m_camera[Camera::RotationSpd].Initialise(0.01, 3, m_ui.cameraRotationSpeed_value,
        m_ui.cameraRotationSpeed_dial, m_callbacks.SetCamera[Camera::RotationSpd]);

    m_light[LIGHT_ACTIVE].Initialise(0.1, 3, m_ui.light_active_value,
        m_ui.light_active_dial, m_callbacks.SetLight[LIGHT_ACTIVE]);

    auto AddEntry = [](std::vector<ComboEntry>& container, FloatSignals& callbacks, 
                       int attribute, double step, int precision, const char* name)
    {
        container.emplace_back(attribute, name, step, precision, callbacks[attribute]);
    };

    std::vector<ComboEntry> texture;
    AddEntry(texture, m_callbacks.SetTexture, TEXTURE_CONTRAST, 0.01, 3, "Contrast");
    AddEntry(texture, m_callbacks.SetTexture, TEXTURE_AMPLITUDE, 0.01, 3, "Amplitude");
    AddEntry(texture, m_callbacks.SetTexture, TEXTURE_SCALE, 0.01, 3, "Scale");
    AddEntry(texture, m_callbacks.SetTexture, TEXTURE_ITERATIONS, 1.0, 3, "Iterations");
    m_texture.Initialise(m_ui.textureAttributes_box, 
        m_ui.textureAttributes_value, m_ui.textureAttributes_dial, texture);

    std::vector<ComboEntry> terrain;
    AddEntry(terrain, m_callbacks.SetTerrain, TERRAIN_BUMP, 0.01, 3, "Bump");
    AddEntry(terrain, m_callbacks.SetTerrain, TERRAIN_AMBIENCE, 0.01, 3, "Ambience");
    AddEntry(terrain, m_callbacks.SetTerrain, TERRAIN_CAUSTICS_AMOUNT, 0.01, 3, "Caustics");
    AddEntry(terrain, m_callbacks.SetTerrain, TERRAIN_CAUSTICS_SCALE, 0.01, 3, "Caustic Scale");
    AddEntry(terrain, m_callbacks.SetTerrain, TERRAIN_SPECULARITY, 0.01, 3, "Specularity");
    AddEntry(terrain, m_callbacks.SetTerrain, TERRAIN_MIN_HEIGHT, 0.01, 3, "Min Height");
    AddEntry(terrain, m_callbacks.SetTerrain, TERRAIN_MAX_HEIGHT, 0.01, 3, "Max Height");
    AddEntry(terrain, m_callbacks.SetTerrain, TERRAIN_SCALE, 1.0, 1, "UV Scale");
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
    m_lightColour.Initialise(m_ui.light_colour_box, 
        m_ui.light_colour_value, m_ui.light_colour_dial, lightColour);

    std::vector<ComboEntry> lightSpecular;
    AddEntry(lightSpecular, m_callbacks.SetLight, LIGHT_SPECULAR_R, 0.01, 3, "Specular R");
    AddEntry(lightSpecular, m_callbacks.SetLight, LIGHT_SPECULAR_G, 0.01, 3, "Specular G");
    AddEntry(lightSpecular, m_callbacks.SetLight, LIGHT_SPECULAR_B, 0.01, 3, "Specular B");
    AddEntry(lightSpecular, m_callbacks.SetLight, LIGHT_SPECULARITY, 0.01, 3, "Specularity");
    m_lightSpecular.Initialise(m_ui.light_specular_box, 
        m_ui.light_specular_value, m_ui.light_specular_dial, lightSpecular);

    std::vector<ComboEntry> mesh;
    AddEntry(mesh, m_callbacks.SetMesh, MESH_AMBIENCE, 0.01, 3, "Ambience");
    AddEntry(mesh, m_callbacks.SetMesh, MESH_CAUSTICS_AMOUNT, 0.01, 3, "Caustics");
    AddEntry(mesh, m_callbacks.SetMesh, MESH_CAUSTICS_SCALE, 0.01, 3, "Caustic Scale");
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
    AddEntry(emitter, m_callbacks.SetEmitter, EMITTER_TINT_R, 0.01, 3, "Tint R");
    AddEntry(emitter, m_callbacks.SetEmitter, EMITTER_TINT_G, 0.01, 3, "Tint G");
    AddEntry(emitter, m_callbacks.SetEmitter, EMITTER_TINT_B, 0.01, 3, "Tint B");
    m_emitter.Initialise(m_ui.emitter_box, 
        m_ui.emitter_value, m_ui.emitter_dial, emitter);

    std::vector<ComboEntry> emitterMinMax;
    AddEntry(emitterMinMax, m_callbacks.SetEmitter, EMITTER_MAX_SPEED, 0.01, 3, "Max Speed");
    AddEntry(emitterMinMax, m_callbacks.SetEmitter, EMITTER_MIN_SPEED, 0.01, 3, "Min Speed");
    AddEntry(emitterMinMax, m_callbacks.SetEmitter, EMITTER_MAX_SIZE, 0.01, 3, "Max Size");
    AddEntry(emitterMinMax, m_callbacks.SetEmitter, EMITTER_MIN_SIZE, 0.01, 3, "Min Size");
    AddEntry(emitterMinMax, m_callbacks.SetEmitter, EMITTER_MAX_AMP, 0.01, 3, "Max Amp");
    AddEntry(emitterMinMax, m_callbacks.SetEmitter, EMITTER_MIN_AMP, 0.01, 3, "Min Amp");
    AddEntry(emitterMinMax, m_callbacks.SetEmitter, EMITTER_MAX_FREQ, 0.01, 3, "Max Freq");
    AddEntry(emitterMinMax, m_callbacks.SetEmitter, EMITTER_MIN_FREQ, 0.01, 3, "Min Freq");
    AddEntry(emitterMinMax, m_callbacks.SetEmitter, EMITTER_MAX_WAIT, 0.01, 3, "Max Wait");
    AddEntry(emitterMinMax, m_callbacks.SetEmitter, EMITTER_MIN_WAIT, 0.01, 3, "Min Wait");
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
    AddEntry(water, m_callbacks.SetWater, WATER_BUMP, 0.01, 3, "Bump Amount");
    AddEntry(water, m_callbacks.SetWater, WATER_BUMP_SCALE_X, 0.01, 3, "Bump Scale X");
    AddEntry(water, m_callbacks.SetWater, WATER_BUMP_SCALE_Y, 0.01, 3, "Bump Scale Y");
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
    **/
}

void TweakerModel::SetSelectedPage(GuiPage::Page page)
{
    if (m_selectedPage != page)
    {
        m_selectedPage = page;
        emit SelectedPageChanged();
    }
}

GuiPage::Page TweakerModel::SelectedPage() const
{
    return m_selectedPage;
}

void TweakerModel::SetDeltaTime(const std::string& dt)
{
    //m_ui.deltaTime_text->setText(QString(dt.c_str()));
    //m_ui.deltaTime_text->update();
}

void TweakerModel::SetFramesPerSec(const std::string& fps)
{
    //m_ui.fps_text->setText(QString(fps.c_str()));
    //m_ui.fps_text->update();
}

void TweakerModel::SetEmitterInstanceCount(const std::string& count)
{
    //m_ui.instancesEmitter_text->setText(QString(count.c_str()));
    //m_ui.instancesEmitter_text->update();
}

void TweakerModel::SetMeshInstanceCount(const std::string& count)
{
    //m_ui.instancesMesh_text->setText(QString(count.c_str()));
    //m_ui.instancesMesh_text->update();
}

void TweakerModel::SetWaterInstanceCount(const std::string& count)
{
    //m_ui.instancesWater_text->setText(QString(count.c_str()));
    //m_ui.instancesWater_text->update();
}

void TweakerModel::SetTerrainInstanceCount(const std::string& count)
{
    //m_ui.instancesTerrain_text->setText(QString(count.c_str()));
    //m_ui.instancesTerrain_text->update();
}

void TweakerModel::SetTerrainShaderName(const std::string& name)
{
    //m_ui.terrainShader_text->setText(QString(name.c_str()));
    //m_ui.terrainShader_text->update();
}

void TweakerModel::SetTexturePath(const std::string& path)
{
    if (!path.empty())
    {
        //const int buffer = 10;
        //const int width = m_ui.displayTexture_btn->width();
        //const int height = m_ui.displayTexture_btn->height();
        //m_ui.displayTexture_btn->setIcon(QIcon(path.c_str()));
        //m_ui.displayTexture_btn->setIconSize(QSize(width-buffer, height-buffer));
        //m_ui.displayTexture_btn->update();
    }
}

void TweakerModel::SetMeshShaderName(const std::string& name)
{
    //m_ui.shader_text->setText(QString(name.c_str()));
    //m_ui.shader_text->update();
}

void TweakerModel::SetWaveAmount(int amount)
{
    //const bool enabled = amount != 0;
    //m_ui.wave_box->setEnabled(enabled);
    //m_ui.wave_dial->setEnabled(enabled);
    //m_ui.wave_value->setEnabled(enabled);
    //m_ui.waveNumber_value->setEnabled(enabled);
    //m_ui.waveNumber_dial->setEnabled(enabled);
    //m_ui.waveNumber_value->setMaximum(std::max(0.0, static_cast<double>(amount-1.0)));
}

AttributeModel* TweakerModel::MeshAttributeModel() const
{
    return m_meshAttributeModel;
}

AttributeModel* TweakerModel::WaveAttributeModel() const
{
    return m_waveAttributeModel;
}

AttributeModel* TweakerModel::WaterAttributeModel() const
{
    return m_waterAttributeModel;
}

AttributeModel* TweakerModel::LightAttributeModel() const
{
    return m_lightAttributeModel;
}

AttributeModel* TweakerModel::TextureAttributeModel() const
{
    return m_textureAttributeModel;
}

AttributeModel* TweakerModel::EmitterAttributeModel() const
{
    return m_emitterAttributeModel;
}

AttributeModel* TweakerModel::TerrainAttributeModel() const
{
    return m_terrainAttributeModel;
}

AttributeModel* TweakerModel::PostAttributeModel() const
{
    return m_postAttributeModel;
}

AttributeModel* TweakerModel::CameraAttributeModel() const
{
    return m_cameraAttributeModel;
}

StringListModel* TweakerModel::EnginesModel() const
{
    return m_enginesModel;
}

StringListModel* TweakerModel::PostMapsModel() const
{
    return m_postMapsModel;
}

StringListModel* TweakerModel::MeshesModel() const
{
    return m_meshesModel;
}

StringListModel* TweakerModel::TerrainModel() const
{
    return m_terrainModel;
}

StringListModel* TweakerModel::TexturesModel() const
{
    return m_texturesModel;
}

StringListModel* TweakerModel::EmittersModel() const
{
    return m_emittersModel;
}

StringListModel* TweakerModel::WaterModel() const
{
    return m_waterModel;
}

StringListModel* TweakerModel::LightsModel() const
{
    return m_lightsModel;
}

StringListModel* TweakerModel::WavesModel() const
{
    return m_wavesModel;
}