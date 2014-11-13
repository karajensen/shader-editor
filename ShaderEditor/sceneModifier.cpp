////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - sceneModifier.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "sceneModifier.h"
#include "scene.h"
#include "cache.h"
#include "renderengine.h"
#include "timer.h"

SceneModifier::SceneModifier(Scene& scene, 
                             Timer& timer,
                             std::shared_ptr<Cache> cache, 
                             int selectedMap) :
    m_scene(scene),
    m_timer(timer),
    m_cache(cache),
    m_selectedMap(selectedMap)
{
}

SceneModifier::~SceneModifier()
{
}

void SceneModifier::Tick(RenderEngine& engine, 
                         const Float2& mousePosition, 
                         const Float2& mouseDirection)
{
    UpdateShader(engine);
    switch (m_cache->PageSelected.Get())
    {
    case SCENE:
        UpdateScene(mousePosition, mouseDirection);
        break;
    case LIGHT:
        UpdateLight();
        break;
    case MESH:
        UpdateMesh();
        break;
    case POST:
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
        ShowMessage(name + ":" + errors, "Compilation Errors");
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

void SceneModifier::UpdateScene(const Float2& mousePosition, 
                                const Float2& mouseDirection)
{
    const int selectedMap = m_cache->PostMapSelected.Get();
    if (selectedMap != m_selectedMap)
    {
        m_selectedMap = selectedMap;
        m_scene.SetPostMap(selectedMap);
    }

    m_cache->MousePosition.Set(mousePosition);
    m_cache->MouseDirection.Set(mouseDirection);

    m_cache->FramesPerSec.Set(m_timer.GetFPS());
    m_cache->DeltaTime.Set(m_timer.GetDeltaTime());
    m_cache->Timer.Set(m_timer.GetTotalTime());

    if (m_cache->SaveLights.Get())
    {
        m_scene.SaveLightsToFile();
        m_cache->SaveLights.Set(false);
    }

    if (m_cache->SaveMeshes.Get())
    {
        m_scene.SaveMeshesToFile();
        m_cache->SaveMeshes.Set(false);
    }

    if (m_cache->SavePost.Get())
    {
        m_scene.SavePostProcessingtoFile();
        m_cache->SavePost.Set(false);
    }
}

void SceneModifier::UpdateMesh()
{
    const int selectedMesh = m_cache->MeshSelected.Get();
    if(selectedMesh != m_selectedMesh)
    {
        m_selectedMesh = selectedMesh;
        auto& mesh = m_scene.GetMesh(m_selectedMesh);

        m_cache->Mesh[MESH_AMBIENCE].SetUpdated(mesh.ambience);
        m_cache->Mesh[MESH_SPECULARITY].SetUpdated(mesh.specularity);
        m_cache->Mesh[MESH_BUMP].SetUpdated(mesh.bump);
        m_cache->Mesh[MESH_GLOW].SetUpdated(mesh.glow);
        m_cache->MeshShader.SetUpdated(m_scene.GetShader(mesh.shaderIndex).name);
    }
    else if(m_selectedMesh >= 0 && m_selectedMesh < m_scene.GetMeshCount())
    {
        auto& mesh = m_scene.GetMesh(m_selectedMesh);
        mesh.specularity = m_cache->Mesh[MESH_SPECULARITY].Get();
        mesh.ambience = m_cache->Mesh[MESH_AMBIENCE].Get();
        mesh.bump = m_cache->Mesh[MESH_BUMP].Get();
        mesh.glow = m_cache->Mesh[MESH_GLOW].Get();
    }

    const int selectedWater = m_cache->WaterSelected.Get();
    if(selectedWater != m_selectedWater)
    {
        m_selectedWater = selectedWater;
        auto& water = m_scene.GetWater(m_selectedWater);

        m_cache->Water[WATER_BUMP].SetUpdated(water.bump);
        m_cache->Water[WATER_BUMP_SPEED].SetUpdated(water.bumpSpeed);
        m_cache->Water[WATER_SPEED].SetUpdated(water.speed);
        m_cache->Water[WATER_DEEP_R].SetUpdated(water.deepColour.r);
        m_cache->Water[WATER_DEEP_G].SetUpdated(water.deepColour.g);
        m_cache->Water[WATER_DEEP_B].SetUpdated(water.deepColour.b);
        m_cache->Water[WATER_SHALLOW_R].SetUpdated(water.shallowColour.r);
        m_cache->Water[WATER_SHALLOW_G].SetUpdated(water.shallowColour.g);
        m_cache->Water[WATER_SHALLOW_B].SetUpdated(water.shallowColour.b);
        m_cache->Water[WATER_REFLECTION_R].SetUpdated(water.reflectionTint.r);
        m_cache->Water[WATER_REFLECTION_G].SetUpdated(water.reflectionTint.g);
        m_cache->Water[WATER_REFLECTION_B].SetUpdated(water.reflectionTint.b);
        m_cache->Water[WATER_REFLECTION].SetUpdated(water.reflection);
        m_cache->Water[WATER_FRESNAL_FACTOR].SetUpdated(water.fresnalFactor);
        m_cache->Water[WATER_OFFSET_U].SetUpdated(water.textureOffset.x);
        m_cache->Water[WATER_OFFSET_V].SetUpdated(water.textureOffset.y);
        m_cache->WaveAmount.SetUpdated(static_cast<int>(water.waves.size()));
    }
    else if(m_selectedWater >= 0 && m_selectedWater < m_scene.GetWaterCount())
    {
        auto& water = m_scene.GetWater(m_selectedWater);
        water.bump = m_cache->Water[WATER_BUMP].Get();
        water.bumpSpeed = m_cache->Water[WATER_BUMP_SPEED].Get();
        water.speed = m_cache->Water[WATER_SPEED].Get();
        water.deepColour.r = m_cache->Water[WATER_DEEP_R].Get();
        water.deepColour.g = m_cache->Water[WATER_DEEP_G].Get();
        water.deepColour.b = m_cache->Water[WATER_DEEP_B].Get();
        water.shallowColour.r = m_cache->Water[WATER_SHALLOW_R].Get();
        water.shallowColour.g = m_cache->Water[WATER_SHALLOW_G].Get();
        water.shallowColour.b = m_cache->Water[WATER_SHALLOW_B].Get();
        water.reflectionTint.r = m_cache->Water[WATER_REFLECTION_R].Get();
        water.reflectionTint.g = m_cache->Water[WATER_REFLECTION_G].Get();
        water.reflectionTint.b = m_cache->Water[WATER_REFLECTION_B].Get();
        water.reflection = m_cache->Water[WATER_REFLECTION].Get();
        water.fresnalFactor = m_cache->Water[WATER_FRESNAL_FACTOR].Get();
        water.textureOffset.x = m_cache->Water[WATER_OFFSET_U].Get();
        water.textureOffset.y = m_cache->Water[WATER_OFFSET_V].Get();
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
            m_cache->Wave[WAVE_SPEED].SetUpdated(wave.speed);
            m_cache->Wave[WAVE_DIRECTION_X].SetUpdated(wave.directionX);
            m_cache->Wave[WAVE_DIRECTION_Z].SetUpdated(wave.directionZ);
        }
        else if(m_selectedWave >= 0 && m_selectedWave < static_cast<int>(water.waves.size()))
        {
            auto& wave = water.waves[m_selectedWave];
            wave.amplitude = m_cache->Wave[WAVE_AMPLITUDE].Get();
            wave.frequency = m_cache->Wave[WAVE_FREQUENCY].Get();
            wave.speed = m_cache->Wave[WAVE_SPEED].Get();
            wave.directionX = m_cache->Wave[WAVE_DIRECTION_X].Get();
            wave.directionZ = m_cache->Wave[WAVE_DIRECTION_Z].Get();
        }
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
}
