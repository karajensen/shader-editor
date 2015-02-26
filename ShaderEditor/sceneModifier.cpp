////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - sceneModifier.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "sceneModifier.h"
#include "renderdata.h"
#include "scene.h"
#include "cache.h"
#include "camera.h"
#include "renderengine.h"
#include "timer.h"

SceneModifier::~SceneModifier() = default;

SceneModifier::SceneModifier(Scene& scene, 
                             Timer& timer,
                             Camera& camera,
                             std::shared_ptr<Cache> cache, 
                             int selectedMap) :
    m_scene(scene),
    m_timer(timer),
    m_cache(cache),
    m_camera(camera),
    m_selectedMap(selectedMap)
{
}

void SceneModifier::Tick(RenderEngine& engine)
{
    UpdateShader(engine);
    switch (m_cache->PageSelected.Get())
    {
    case PAGE_SCENE:
        UpdateScene();
        break;
    case PAGE_LIGHT:
        UpdateLight();
        break;
    case PAGE_MESH:
        UpdateMesh();
        UpdateWater();
        UpdateEmitter();
        break;
    case PAGE_POST:
        UpdatePost();
        break;
    }
}

void SceneModifier::SetApplicationRunning(bool run)
{
    m_cache->ApplicationRunning.Set(run);
}

bool SceneModifier::ReCompileShader(const std::string& text, RenderEngine& engine)
{
    const std::string& name = m_scene.GetShader(m_selectedShader).name;
    m_cache->CompileShader.Clear();
    engine.WriteToShader(name, text);

    std::string errors = engine.CompileShader(m_selectedShader);
    if(errors.empty())
    {
        Logger::LogInfo(name + ": Recompiled successfully");
        return true;
    }
    else
    {
        Logger::LogInfo(name + ": Failed Recompilation");
        MessageBox(nullptr, (name + ":" + errors).c_str(), "Compilation Errors", MB_OK);
        return false;
    }
}

void SceneModifier::UpdateShader(RenderEngine& engine)
{   
    const int selectedShader = m_cache->ShaderSelected.Get();
    bool changedShader = selectedShader != m_selectedShader;
    bool recompiledShader = false;

    const std::string updatedText = m_cache->CompileShader.Get();
    if(!updatedText.empty() && m_selectedShader != NO_INDEX)
    {
        recompiledShader = ReCompileShader(updatedText, engine);
    }

    if(changedShader || recompiledShader)
    {
        m_selectedShader = selectedShader;
        m_cache->ShaderText.SetUpdated(engine.GetShaderText(m_selectedShader));
        m_cache->ShaderAsm.SetUpdated(engine.GetShaderAssembly(m_selectedShader));
    }
}

int SceneModifier::GetSelectedEngine() const
{
    return m_cache->EngineSelected.Get();
}

void SceneModifier::SetSelectedEngine(int engine)
{
    m_cache->EngineSelected.SetUpdated(engine);

    m_selectedShader = NO_INDEX; // allows selected shader to be re-cached
    m_selectedMap = NO_INDEX;    // allows post values to be re-cached
}

bool SceneModifier::RequiresReload()
{
    if (m_cache->ReloadScene.Get())
    {
        m_cache->ReloadScene.Set(false);
        return true;
    }
    return false;
}

void SceneModifier::UpdateScene()
{
    UpdateCamera();

    const int selectedMap = m_cache->PostMapSelected.Get();
    if (selectedMap != m_selectedMap)
    {
        m_selectedMap = selectedMap;
        m_scene.SetPostMap(selectedMap);
    }

    m_cache->FramesPerSec.Set(m_timer.GetFPS());
    m_cache->DeltaTime.Set(m_timer.GetDeltaTime());
    m_cache->Timer.Set(m_timer.GetTotalTime());

    if (m_cache->SaveScene.Get())
    {
        m_scene.SaveSceneToFile();
        m_cache->SaveScene.Set(false);
    }

    if (m_cache->SavePost.Get())
    {
        m_scene.SavePostProcessingtoFile();
        m_cache->SavePost.Set(false);
    }
}

void SceneModifier::UpdateCamera()
{
    if (m_camera.HasMouseRotatedCamera())
    {
        m_cache->Camera[CAMERA_POSITION_X].SetUpdated(m_camera.GetCamera(Camera::POSITION_X));
        m_cache->Camera[CAMERA_POSITION_Y].SetUpdated(m_camera.GetCamera(Camera::POSITION_Y));
        m_cache->Camera[CAMERA_POSITION_Z].SetUpdated(m_camera.GetCamera(Camera::POSITION_Z));
        m_cache->Camera[CAMERA_PITCH].SetUpdated(m_camera.GetCamera(Camera::ROTATION_PITCH));
        m_cache->Camera[CAMERA_YAW].SetUpdated(m_camera.GetCamera(Camera::ROTATION_YAW));
        m_cache->Camera[CAMERA_ROLL].SetUpdated(m_camera.GetCamera(Camera::ROTATION_ROLL));
    }
    else
    {
        auto updateCamera = [this](CameraAttribute attribute, Camera::Component component)
        {
            if (!m_cache->Camera[attribute].RequiresUpdate())
            {
                m_camera.SetCamera(component, m_cache->Camera[attribute].Get());
            }
        };

        updateCamera(CAMERA_POSITION_X, Camera::POSITION_X);
        updateCamera(CAMERA_POSITION_Y, Camera::POSITION_Y);
        updateCamera(CAMERA_POSITION_Z, Camera::POSITION_Z);
        updateCamera(CAMERA_PITCH, Camera::ROTATION_PITCH);
        updateCamera(CAMERA_ROLL, Camera::ROTATION_ROLL);
        updateCamera(CAMERA_YAW, Camera::ROTATION_YAW);
    }
}

void SceneModifier::UpdateMesh()
{
    const int selectedMesh = m_cache->MeshSelected.Get();
    if (selectedMesh != m_selectedMesh)
    {
        m_selectedMesh = selectedMesh;
        auto& mesh = m_scene.GetMesh(m_selectedMesh);

        m_cache->Mesh[MESH_AMBIENCE].SetUpdated(mesh.ambience);
        m_cache->Mesh[MESH_SPECULARITY].SetUpdated(mesh.specularity);
        m_cache->Mesh[MESH_BUMP].SetUpdated(mesh.bump);
        m_cache->Mesh[MESH_GLOW].SetUpdated(mesh.glow);
        m_cache->MeshShader.SetUpdated(m_scene.GetShader(mesh.shaderIndex).name);
    }
    else if (m_selectedMesh >= 0 && m_selectedMesh < m_scene.GetMeshCount())
    {
        auto& mesh = m_scene.GetMesh(m_selectedMesh);
        mesh.specularity = m_cache->Mesh[MESH_SPECULARITY].Get();
        mesh.ambience = m_cache->Mesh[MESH_AMBIENCE].Get();
        mesh.bump = m_cache->Mesh[MESH_BUMP].Get();
        mesh.glow = m_cache->Mesh[MESH_GLOW].Get();
    }
}

void SceneModifier::UpdateWater()
{
    const int selectedWater = m_cache->WaterSelected.Get();
    if(selectedWater != m_selectedWater)
    {
        m_selectedWater = selectedWater;
        auto& water = m_scene.GetWater(m_selectedWater);

        m_cache->Water[WATER_BUMP].SetUpdated(water.bump);
        m_cache->Water[WATER_BUMP_VELOCITY_X].SetUpdated(water.bumpVelocity.x);
        m_cache->Water[WATER_BUMP_VELOCITY_Y].SetUpdated(water.bumpVelocity.y);
        m_cache->Water[WATER_SPEED].SetUpdated(water.speed);
        m_cache->Water[WATER_DEEP_R].SetUpdated(water.deepColour.r);
        m_cache->Water[WATER_DEEP_G].SetUpdated(water.deepColour.g);
        m_cache->Water[WATER_DEEP_B].SetUpdated(water.deepColour.b);
        m_cache->Water[WATER_DEEP_A].SetUpdated(water.deepColour.a);
        m_cache->Water[WATER_SHALLOW_R].SetUpdated(water.shallowColour.r);
        m_cache->Water[WATER_SHALLOW_G].SetUpdated(water.shallowColour.g);
        m_cache->Water[WATER_SHALLOW_B].SetUpdated(water.shallowColour.b);
        m_cache->Water[WATER_SHALLOW_A].SetUpdated(water.shallowColour.a);
        m_cache->Water[WATER_REFLECTION_R].SetUpdated(water.reflectionTint.r);
        m_cache->Water[WATER_REFLECTION_G].SetUpdated(water.reflectionTint.g);
        m_cache->Water[WATER_REFLECTION_B].SetUpdated(water.reflectionTint.b);
        m_cache->Water[WATER_REFLECTION].SetUpdated(water.reflection);
        m_cache->Water[WATER_FRESNAL_SCALE].SetUpdated(water.fresnal.x);
        m_cache->Water[WATER_FRESNAL_BIAS].SetUpdated(water.fresnal.y);
        m_cache->Water[WATER_FRESNAL_POWER].SetUpdated(water.fresnal.z);
        m_cache->Water[WATER_SCALE_U].SetUpdated(water.uvScale.x);
        m_cache->Water[WATER_SCALE_V].SetUpdated(water.uvScale.y);
        m_cache->WaveAmount.SetUpdated(static_cast<int>(water.waves.size()));
    }
    else if(m_selectedWater >= 0 && m_selectedWater < m_scene.GetWaterCount())
    {
        auto& water = m_scene.GetWater(m_selectedWater);
        water.bump = m_cache->Water[WATER_BUMP].Get();
        water.bumpVelocity.x = m_cache->Water[WATER_BUMP_VELOCITY_X].Get();
        water.bumpVelocity.y = m_cache->Water[WATER_BUMP_VELOCITY_Y].Get();
        water.speed = m_cache->Water[WATER_SPEED].Get();
        water.deepColour.r = m_cache->Water[WATER_DEEP_R].Get();
        water.deepColour.g = m_cache->Water[WATER_DEEP_G].Get();
        water.deepColour.b = m_cache->Water[WATER_DEEP_B].Get();
        water.deepColour.a = m_cache->Water[WATER_DEEP_A].Get();
        water.shallowColour.r = m_cache->Water[WATER_SHALLOW_R].Get();
        water.shallowColour.g = m_cache->Water[WATER_SHALLOW_G].Get();
        water.shallowColour.b = m_cache->Water[WATER_SHALLOW_B].Get();
        water.shallowColour.a = m_cache->Water[WATER_SHALLOW_A].Get();
        water.reflectionTint.r = m_cache->Water[WATER_REFLECTION_R].Get();
        water.reflectionTint.g = m_cache->Water[WATER_REFLECTION_G].Get();
        water.reflectionTint.b = m_cache->Water[WATER_REFLECTION_B].Get();
        water.reflection = m_cache->Water[WATER_REFLECTION].Get();
        water.fresnal.x = m_cache->Water[WATER_FRESNAL_SCALE].Get();
        water.fresnal.y = m_cache->Water[WATER_FRESNAL_BIAS].Get();
        water.fresnal.z = m_cache->Water[WATER_FRESNAL_POWER].Get();
        water.uvScale.x = m_cache->Water[WATER_SCALE_U].Get();
        water.uvScale.y = m_cache->Water[WATER_SCALE_V].Get();
    }

    if (m_selectedWater != NO_INDEX)
    {
        auto& water = m_scene.GetWater(m_selectedWater);
        const int selectedWave = m_cache->WaveSelected.Get();

        if(selectedWave != m_selectedWave)
        {
            m_selectedWave = selectedWave;
            auto& wave = water.waves[m_selectedWave];

            m_cache->Wave[WAVE_AMPLITUDE].SetUpdated(wave.amplitude);
            m_cache->Wave[WAVE_FREQUENCY].SetUpdated(wave.frequency);
            m_cache->Wave[WAVE_PHASE].SetUpdated(wave.phase);
            m_cache->Wave[WAVE_DIRECTION_X].SetUpdated(wave.directionX);
            m_cache->Wave[WAVE_DIRECTION_Z].SetUpdated(wave.directionZ);
        }
        else if(m_selectedWave >= 0 && m_selectedWave < static_cast<int>(water.waves.size()))
        {
            auto& wave = water.waves[m_selectedWave];
            wave.amplitude = m_cache->Wave[WAVE_AMPLITUDE].Get();
            wave.frequency = m_cache->Wave[WAVE_FREQUENCY].Get();
            wave.phase = m_cache->Wave[WAVE_PHASE].Get();
            wave.directionX = m_cache->Wave[WAVE_DIRECTION_X].Get();
            wave.directionZ = m_cache->Wave[WAVE_DIRECTION_Z].Get();
        }
    }
}

void SceneModifier::RestrictMinMax(Lockable<float>& minCached,
                                   Lockable<float>& maxCached, 
                                   float& minValue,
                                   float& maxValue,
                                   bool restrictNegative)
{
    const float minCachedValue = minCached.Get();
    const float maxCachedValue= maxCached.Get();

    if (minCachedValue >= maxCachedValue || 
        (restrictNegative && minCachedValue < 0.0f) || 
        (restrictNegative && maxCachedValue < 0.0f))
    {
        minCached.SetUpdated(minValue);
        maxCached.SetUpdated(maxValue);
    }
    else
    {
        minValue = minCachedValue;
        maxValue = maxCachedValue;
    }
}

void SceneModifier::UpdateEmitter()
{
    const int selectedEmitter = m_cache->EmitterSelected.Get();
    if(selectedEmitter != m_selectedEmitter)
    {
        m_selectedEmitter = selectedEmitter;
        auto& emitter = m_scene.GetEmitter(m_selectedEmitter);

        m_cache->Emitter[EMITTER_LENGTH].SetUpdated(emitter.length);
        m_cache->Emitter[EMITTER_WIDTH].SetUpdated(emitter.width);
        m_cache->Emitter[EMITTER_POS_X].SetUpdated(emitter.position.x);
        m_cache->Emitter[EMITTER_POS_Y].SetUpdated(emitter.position.y);
        m_cache->Emitter[EMITTER_POS_Z].SetUpdated(emitter.position.z);
        m_cache->Emitter[EMITTER_DIR_X].SetUpdated(emitter.direction.x);
        m_cache->Emitter[EMITTER_DIR_Y].SetUpdated(emitter.direction.y);
        m_cache->Emitter[EMITTER_DIR_Z].SetUpdated(emitter.direction.z);
        m_cache->Emitter[EMITTER_LIFETIME].SetUpdated(emitter.lifeTime);
        m_cache->Emitter[EMITTER_LIFEFADE].SetUpdated(emitter.lifeFade);
        m_cache->Emitter[EMITTER_TINT_R].SetUpdated(emitter.tint.r);
        m_cache->Emitter[EMITTER_TINT_G].SetUpdated(emitter.tint.g);
        m_cache->Emitter[EMITTER_TINT_B].SetUpdated(emitter.tint.b);
        m_cache->Emitter[EMITTER_MAX_SPEED].SetUpdated(emitter.maxSpeed);
        m_cache->Emitter[EMITTER_MIN_SPEED].SetUpdated(emitter.minSpeed);
        m_cache->Emitter[EMITTER_MAX_SIZE].SetUpdated(emitter.maxSize);
        m_cache->Emitter[EMITTER_MIN_SIZE].SetUpdated(emitter.minSize);
        m_cache->Emitter[EMITTER_MAX_FREQ].SetUpdated(emitter.maxFrequency);
        m_cache->Emitter[EMITTER_MIN_FREQ].SetUpdated(emitter.minFrequency);
        m_cache->Emitter[EMITTER_MAX_AMP].SetUpdated(emitter.maxAmplitude);
        m_cache->Emitter[EMITTER_MIN_AMP].SetUpdated(emitter.minAmplitude);
        m_cache->Emitter[EMITTER_MAX_WAVE].SetUpdated(emitter.maxWaveSpeed);
        m_cache->Emitter[EMITTER_MIN_WAVE].SetUpdated(emitter.minWaveSpeed);
        m_cache->ParticleAmount.SetUpdated(static_cast<int>(emitter.particles.size()));
    }
    else if(m_selectedEmitter >= 0 && m_selectedEmitter < m_scene.GetEmitterCount())
    {
        auto& emitter = m_scene.GetEmitter(m_selectedEmitter);
        emitter.length = m_cache->Emitter[EMITTER_LENGTH].Get();
        emitter.width = m_cache->Emitter[EMITTER_WIDTH].Get();
        emitter.position.x = m_cache->Emitter[EMITTER_POS_X].Get();
        emitter.position.y = m_cache->Emitter[EMITTER_POS_Y].Get();
        emitter.position.z = m_cache->Emitter[EMITTER_POS_Z].Get();
        emitter.direction.x = m_cache->Emitter[EMITTER_DIR_X].Get();
        emitter.direction.y = m_cache->Emitter[EMITTER_DIR_Y].Get();
        emitter.direction.z = m_cache->Emitter[EMITTER_DIR_Z].Get();
        emitter.lifeTime = m_cache->Emitter[EMITTER_LIFETIME].Get();
        emitter.lifeFade = m_cache->Emitter[EMITTER_LIFEFADE].Get();
        emitter.tint.r = m_cache->Emitter[EMITTER_TINT_R].Get();
        emitter.tint.g = m_cache->Emitter[EMITTER_TINT_G].Get();
        emitter.tint.b = m_cache->Emitter[EMITTER_TINT_B].Get();
        emitter.Resize(m_cache->ParticleAmount.Get());

        RestrictMinMax(
            m_cache->Emitter[EMITTER_MIN_SIZE],
            m_cache->Emitter[EMITTER_MAX_SIZE],
            emitter.minSize, emitter.maxSize, true);

        RestrictMinMax(
            m_cache->Emitter[EMITTER_MIN_SPEED],
            m_cache->Emitter[EMITTER_MAX_SPEED],
            emitter.minSpeed, emitter.maxSpeed);

        RestrictMinMax(
            m_cache->Emitter[EMITTER_MIN_AMP],
            m_cache->Emitter[EMITTER_MAX_AMP],
            emitter.minAmplitude, emitter.maxAmplitude);

        RestrictMinMax(
            m_cache->Emitter[EMITTER_MIN_WAVE],
            m_cache->Emitter[EMITTER_MAX_WAVE],
            emitter.minWaveSpeed, emitter.maxWaveSpeed);

        RestrictMinMax(
            m_cache->Emitter[EMITTER_MIN_FREQ],
            m_cache->Emitter[EMITTER_MAX_FREQ],
            emitter.minFrequency, emitter.maxFrequency);
    }

    if (m_cache->PauseEmission.Get() && m_selectedEmitter != NO_INDEX)
    {
        m_scene.GetEmitter(m_selectedEmitter).TogglePaused();
        m_cache->PauseEmission.Set(false);
    }
}

void SceneModifier::UpdatePost()
{
    PostProcessing& post = m_scene.GetPost();
    post.dofDistance = m_cache->DOFDistance.Get();
    post.dofFade = m_cache->DOFFade.Get();
    post.blurAmount = m_cache->BlurAmount.Get();
    post.blurStep = m_cache->BlurStep.Get();
    post.glowAmount = m_cache->GlowAmount.Get();
    post.contrast = m_cache->Contrast.Get();
    post.saturation = m_cache->Saturation.Get();
    post.depthFar = m_cache->DepthFar.Get();
    post.depthNear = m_cache->DepthNear.Get();
    post.fogColour.r = m_cache->Fog[FOG_RED].Get();
    post.fogColour.g = m_cache->Fog[FOG_GREEN].Get();
    post.fogColour.b = m_cache->Fog[FOG_BLUE].Get();
    post.fogDistance = m_cache->Fog[FOG_DISTANCE].Get();
    post.fogFade = m_cache->Fog[FOG_FADE].Get();
    post.minimumColour.r = m_cache->MinColour[RED].Get();
    post.minimumColour.g = m_cache->MinColour[GREEN].Get();
    post.minimumColour.b = m_cache->MinColour[BLUE].Get();
    post.maximumColour.r = m_cache->MaxColour[RED].Get();
    post.maximumColour.g = m_cache->MaxColour[GREEN].Get();
    post.maximumColour.b = m_cache->MaxColour[BLUE].Get();
}

void SceneModifier::UpdateLight()
{
    const int selectedLight = m_cache->LightSelected.Get();
    if(selectedLight != m_selectedLight)
    {
        m_selectedLight = selectedLight;

        auto& light = m_scene.GetLight(m_selectedLight);
        m_cache->Light[LIGHT_POSITION_X].SetUpdated(light.position.x);
        m_cache->Light[LIGHT_POSITION_Y].SetUpdated(light.position.y);
        m_cache->Light[LIGHT_POSITION_Z].SetUpdated(light.position.z);
        m_cache->Light[LIGHT_ATTENUATION_X].SetUpdated(light.attenuation.x);
        m_cache->Light[LIGHT_ATTENUATION_Y].SetUpdated(light.attenuation.y);
        m_cache->Light[LIGHT_ATTENUATION_Z].SetUpdated(light.attenuation.z);
        m_cache->Light[LIGHT_DIFFUSE_R].SetUpdated(light.diffuse.r);
        m_cache->Light[LIGHT_DIFFUSE_G].SetUpdated(light.diffuse.g);
        m_cache->Light[LIGHT_DIFFUSE_B].SetUpdated(light.diffuse.b);
        m_cache->Light[LIGHT_SPECULAR_R].SetUpdated(light.specular.r);
        m_cache->Light[LIGHT_SPECULAR_G].SetUpdated(light.specular.g);
        m_cache->Light[LIGHT_SPECULAR_B].SetUpdated(light.specular.b);
        m_cache->Light[LIGHT_SPECULARITY].SetUpdated(light.specularity);
    }
    else if(m_selectedLight >= 0 && m_selectedLight < m_scene.GetLightCount())
    {
        auto& light = m_scene.GetLight(m_selectedLight);
        light.position.x = m_cache->Light[LIGHT_POSITION_X].Get();
        light.position.y = m_cache->Light[LIGHT_POSITION_Y].Get();
        light.position.z = m_cache->Light[LIGHT_POSITION_Z].Get();
        light.attenuation.x = m_cache->Light[LIGHT_ATTENUATION_X].Get();
        light.attenuation.y = m_cache->Light[LIGHT_ATTENUATION_Y].Get();
        light.attenuation.z = m_cache->Light[LIGHT_ATTENUATION_Z].Get();
        light.diffuse.r = m_cache->Light[LIGHT_DIFFUSE_R].Get();
        light.diffuse.g = m_cache->Light[LIGHT_DIFFUSE_G].Get();
        light.diffuse.b = m_cache->Light[LIGHT_DIFFUSE_B].Get();
        light.specular.r = m_cache->Light[LIGHT_SPECULAR_R].Get();
        light.specular.g = m_cache->Light[LIGHT_SPECULAR_G].Get();
        light.specular.b = m_cache->Light[LIGHT_SPECULAR_B].Get();
        light.specularity = m_cache->Light[LIGHT_SPECULARITY].Get();
    }

    if (m_cache->LightDiagnostics.Get())
    {
        m_cache->LightDiagnostics.Set(false);
    }
}

void SceneModifier::Initialise(const std::vector<std::string>& engineNames,
                               int selectedEngine)
{
    m_scene.SetPostMap(m_selectedMap);
    m_engineAmount = engineNames.size();

    m_cache->PostMapSelected.Set(m_selectedMap);
    m_cache->EngineSelected.Set(selectedEngine);

    m_cache->Engines.Set(engineNames);
    m_cache->Lights.Set(m_scene.GetLightNames());
    m_cache->Meshes.Set(m_scene.GetMeshNames());
    m_cache->Shaders.Set(m_scene.GetShaderNames());
    m_cache->PostMaps.Set(m_scene.GetPostMapNames());
    m_cache->Waters.Set(m_scene.GetWaterNames());
    m_cache->Emitters.Set(m_scene.GetEmitterNames());

    m_cache->WaveAmount.SetUpdated(m_scene.GetWaterCount() > 0 ? 
        static_cast<int>(m_scene.GetWater(0).waves.size()) : 0);

    const PostProcessing& post = m_scene.Post();
    m_cache->DepthNear.SetUpdated(post.depthNear);
    m_cache->DepthFar.SetUpdated(post.depthFar);
    m_cache->DOFDistance.SetUpdated(post.dofDistance);
    m_cache->DOFFade.SetUpdated(post.dofFade);
    m_cache->BlurAmount.SetUpdated(post.blurAmount);
    m_cache->BlurStep.SetUpdated(post.blurStep);
    m_cache->GlowAmount.SetUpdated(post.glowAmount);
    m_cache->Contrast.SetUpdated(post.contrast);
    m_cache->Saturation.SetUpdated(post.saturation);
    m_cache->Fog[FOG_RED].SetUpdated(post.fogColour.r);
    m_cache->Fog[FOG_GREEN].SetUpdated(post.fogColour.g);
    m_cache->Fog[FOG_BLUE].SetUpdated(post.fogColour.b);
    m_cache->Fog[FOG_DISTANCE].SetUpdated(post.fogDistance);
    m_cache->Fog[FOG_FADE].SetUpdated(post.fogFade);
    m_cache->MinColour[RED].SetUpdated(post.minimumColour.r);
    m_cache->MinColour[GREEN].SetUpdated(post.minimumColour.g);
    m_cache->MinColour[BLUE].SetUpdated(post.minimumColour.b);
    m_cache->MaxColour[RED].SetUpdated(post.maximumColour.r);
    m_cache->MaxColour[GREEN].SetUpdated(post.maximumColour.g);
    m_cache->MaxColour[BLUE].SetUpdated(post.maximumColour.b);

    m_cache->Camera[CAMERA_POSITION_X].SetUpdated(m_camera.GetCamera(Camera::POSITION_X));
    m_cache->Camera[CAMERA_POSITION_Y].SetUpdated(m_camera.GetCamera(Camera::POSITION_Y));
    m_cache->Camera[CAMERA_POSITION_Z].SetUpdated(m_camera.GetCamera(Camera::POSITION_Z));
    m_cache->Camera[CAMERA_PITCH].SetUpdated(m_camera.GetCamera(Camera::ROTATION_PITCH));
    m_cache->Camera[CAMERA_YAW].SetUpdated(m_camera.GetCamera(Camera::ROTATION_YAW));
    m_cache->Camera[CAMERA_ROLL].SetUpdated(m_camera.GetCamera(Camera::ROTATION_ROLL));
}
