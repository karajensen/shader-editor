////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - sceneModifier.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "sceneModifier.h"
#include "renderdata.h"
#include "scene.h"
#include "cache.h"
#include "camera.h"
#include "renderengine.h"
#include "diagnostic.h"
#include "timer.h"
#include "boost/lexical_cast.hpp"

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
    case PAGE_AREA:
        UpdateTerrain();
        UpdateTexture();
        break;
    case PAGE_MESH:
        UpdateMesh();
        UpdateWater();
        UpdateEmitter();
        break;
    case PAGE_POST:
        UpdateLight();
        UpdatePost(engine);
        break;
    }
}

void SceneModifier::SetApplicationRunning(bool run)
{
    m_cache->ApplicationRunning.Set(run);
}

bool SceneModifier::ReCompileShader(const std::string& text, RenderEngine& engine)
{
    const std::string& name = m_scene.GetShader(m_selectedShader).Name();
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

    m_cache->FramesPerSec.Set(m_timer.GetFPS());
    m_cache->DeltaTime.Set(m_timer.GetDeltaTime());
    m_cache->Timer.Set(m_timer.GetTotalTime());

    if (m_cache->SaveScene.Get())
    {
        m_scene.SaveSceneToFile();
        m_cache->SaveScene.Set(false);
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
        m_scene.GetMesh(m_selectedMesh).Write(*m_cache);
    }
    else if (m_selectedMesh >= 0 && 
             m_selectedMesh < static_cast<int>(m_scene.Meshes().size()))
    {
        auto& mesh = m_scene.GetMesh(m_selectedMesh);
        mesh.Read(*m_cache);

        m_cache->MeshInstances.SetUpdated(
            boost::lexical_cast<std::string>(mesh.Instances().size()));
    }
}

void SceneModifier::UpdateWater()
{
    const int selectedWater = m_cache->WaterSelected.Get();
    if(selectedWater != m_selectedWater)
    {
        m_selectedWater = selectedWater;
        m_scene.GetWater(m_selectedWater).Write(*m_cache);
    }
    else if(m_selectedWater >= 0 && 
            m_selectedWater < static_cast<int>(m_scene.Waters().size()))
    {
        m_scene.GetWater(m_selectedWater).Read(*m_cache);
    }

    if (m_selectedWater != NO_INDEX)
    {
        auto& water = m_scene.GetWater(m_selectedWater);
        const int selectedWave = m_cache->WaveSelected.Get();
        const int waveCount = static_cast<int>(water.Waves().size());

        if(selectedWave != m_selectedWave)
        {
            m_selectedWave = selectedWave;
            water.WriteWave(*m_cache, m_selectedWave);
        }
        else if(m_selectedWave >= 0 && m_selectedWave < waveCount)
        {
            water.ReadWave(*m_cache, m_selectedWave);
        }
    }
}

void SceneModifier::UpdateTerrain()
{
    const int selectedTerrain = m_cache->TerrainSelected.Get();
    if(selectedTerrain != m_selectedTerrain)
    {
        m_selectedTerrain = selectedTerrain;
        //m_scene.GetTerrain(m_selectedTerrain).Write(*m_cache);
    }
    else if(m_selectedTerrain >= 0 && 
            m_selectedTerrain < static_cast<int>(m_scene.Terrains().size()))
    {
        //m_scene.GetTerrain(m_selectedTerrain).Read(*m_cache);
    }
}

void SceneModifier::UpdateTexture()
{
    const int selectedTexture = m_cache->TextureSelected.Get();
    if(selectedTexture != m_selectedTexture)
    {
        m_selectedTexture = selectedTexture;
        auto& texture = m_scene.GetProceduralTexture(m_selectedTexture);
        texture.Write(*m_cache);
        texture.SaveTexture();
    }
    else if(m_selectedTexture >= 0 && 
            m_selectedTexture < static_cast<int>(m_scene.Textures().size()))
    {
        m_scene.GetProceduralTexture(m_selectedTexture).Read(*m_cache);
    }
}

void SceneModifier::UpdateEmitter()
{
    const int selectedEmitter = m_cache->EmitterSelected.Get();
    if(selectedEmitter != m_selectedEmitter)
    {
        m_selectedEmitter = selectedEmitter;
        m_scene.GetEmitter(m_selectedEmitter).Write(*m_cache);
    }
    else if(m_selectedEmitter >= 0 && 
            m_selectedEmitter < static_cast<int>(m_scene.Emitters().size()))
    {
        m_scene.GetEmitter(m_selectedEmitter).Read(*m_cache);
    }

    if (m_cache->PauseEmission.Get() && m_selectedEmitter != NO_INDEX)
    {
        m_scene.GetEmitter(m_selectedEmitter).TogglePaused();
        m_cache->PauseEmission.Set(false);
    }
}

void SceneModifier::UpdatePost(RenderEngine& engine)
{
    const int selectedMap = m_cache->PostMapSelected.Get();
    if (selectedMap != m_selectedMap)
    {
        m_selectedMap = selectedMap;
        m_scene.SetPostMap(selectedMap);
    }

    m_scene.GetPost().Read(*m_cache);

    if (m_cache->ToggleWireframe.Get())
    {
        engine.ToggleWireframe();
        m_cache->ToggleWireframe.Set(false);
    }
}

void SceneModifier::UpdateLight()
{
    const int selectedLight = m_cache->LightSelected.Get();
    if(selectedLight != m_selectedLight)
    {
        m_selectedLight = selectedLight;
        m_scene.GetLight(m_selectedLight).Write(*m_cache);
    }
    else if(m_selectedLight >= 0 && 
            m_selectedLight < static_cast<int>(m_scene.Lights().size()))
    {
        m_scene.GetLight(m_selectedLight).Read(*m_cache);
    }

    if (m_cache->LightDiagnostics.Get())
    {
        m_scene.GetDiagnostics().ToggleLightDiagnostics();
        m_cache->LightDiagnostics.Set(false);
    }

    if (m_cache->RenderLightsOnly.Get())
    {
        m_scene.GetPost().ToggleDiffuseTextures();
        m_cache->RenderLightsOnly.Set(false);
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
    m_cache->Lights.Set(GetLightNames());
    m_cache->Meshes.Set(GetMeshNames());
    m_cache->Shaders.Set(GetShaderNames());
    m_cache->PostMaps.Set(GetPostMapNames());
    m_cache->Waters.Set(GetWaterNames());
    m_cache->Emitters.Set(GetEmitterNames());
    m_cache->Textures.Set(GetTextureNames());
    m_cache->Terrains.Set(GetTerrainNames());

    m_scene.GetPost().Write(*m_cache);

    m_cache->Camera[CAMERA_POSITION_X].SetUpdated(
        m_camera.GetCamera(Camera::POSITION_X));

    m_cache->Camera[CAMERA_POSITION_Y].SetUpdated(
        m_camera.GetCamera(Camera::POSITION_Y));

    m_cache->Camera[CAMERA_POSITION_Z].SetUpdated(
        m_camera.GetCamera(Camera::POSITION_Z));

    m_cache->Camera[CAMERA_PITCH].SetUpdated(
        m_camera.GetCamera(Camera::ROTATION_PITCH));

    m_cache->Camera[CAMERA_YAW].SetUpdated(
        m_camera.GetCamera(Camera::ROTATION_YAW));

    m_cache->Camera[CAMERA_ROLL].SetUpdated(
        m_camera.GetCamera(Camera::ROTATION_ROLL));
}

std::vector<std::string> SceneModifier::GetLightNames() const
{
    std::vector<std::string> lights;
    for(const auto& light : m_scene.Lights())
    {
        lights.push_back(light->Name());
    }
    return lights;
}

std::vector<std::string> SceneModifier::GetEmitterNames() const
{
    std::vector<std::string> emitters;
    for(const auto& emitter : m_scene.Emitters())
    {
        emitters.push_back(emitter->Name());
    }
    return emitters;
}

std::vector<std::string> SceneModifier::GetMeshNames() const
{
    std::vector<std::string> meshes;
    for(const auto& mesh : m_scene.Meshes())
    {
        meshes.push_back(mesh->Name());
    }
    return meshes;
}

std::vector<std::string> SceneModifier::GetWaterNames() const
{
    std::vector<std::string> waters;
    for(const auto& water : m_scene.Waters())
    {
        waters.push_back(water->Name());
    }
    return waters;
}

std::vector<std::string> SceneModifier::GetPostMapNames() const
{
    std::vector<std::string> maps;
    for (int i = 0; i < PostProcessing::MAX_MAPS; ++i)
    {
        maps.push_back(PostProcessing::GetMapName(
            static_cast<PostProcessing::Map>(i)));
    }
    return maps;
}

std::vector<std::string> SceneModifier::GetTerrainNames() const
{
    std::vector<std::string> terrainNames;
    for (const auto& terrain : m_scene.Terrains())
    {
        terrainNames.push_back(terrain->Name());
    }
    return terrainNames;
}

std::vector<std::string> SceneModifier::GetTextureNames() const
{
    std::vector<std::string> textures;
    for (const auto& texture : m_scene.Textures())
    {
        if (texture->HasPixels())
        {
            textures.push_back(texture->Name());
        }
    }
    return textures;
}

std::vector<std::string> SceneModifier::GetShaderNames() const
{
    std::vector<std::string> shaders;
    for(const auto& shader : m_scene.Shaders())
    {
        shaders.push_back(shader->Name());
    }
    return shaders;
}