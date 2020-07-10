////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - app_gui.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "app_gui.h"
#include "mesh.h"
#include "water.h"
#include "shader.h"
#include "texture.h"
#include "texture_procedural.h"
#include "postprocessing.h"
#include "emitter.h"
#include "terrain.h"
#include "light.h"
#include "scene_data.h"
#include "scene.h"
#include "cache.h"
#include "camera.h"
#include "render_engine.h"
#include "timer.h"
#include "logger.h"

#include <boost/lexical_cast.hpp>

AppGui::~AppGui() = default;

AppGui::AppGui(Scene& scene, 
               Timer& timer,
               Camera& camera,
               std::shared_ptr<Cache> cache, 
               int selectedMap,
               std::function<void(void)> reloadEngine) 
    : m_scene(scene)
    , m_data(scene.GetData())
    , m_timer(timer)
    , m_cache(cache)
    , m_camera(camera)
    , m_selectedMap(selectedMap)
    , m_reloadEngine(reloadEngine)
{
}

void AppGui::Tick(RenderEngine& engine)
{
    UpdateShader(engine);
    switch (m_cache->PageSelected.Get())
    {
    case Tweakable::GuiPage::Scene:
        UpdateScene(engine);
        break;
    case Tweakable::GuiPage::Area:
        UpdateTerrain(engine);
        UpdateTexture(engine);
        break;
    case Tweakable::GuiPage::Mesh:
        UpdateMesh();
        UpdateWater();
        UpdateEmitter();
        break;
    case Tweakable::GuiPage::Post:
        UpdateLight();
        UpdatePost(engine);
        break;
    }
}

void AppGui::SetApplicationRunning(bool run)
{
    m_cache->ApplicationRunning.Set(run);
}

bool AppGui::ReCompileShader(const std::string& text, RenderEngine& engine)
{
    const auto& shader = *m_data.shaders[m_selectedShader];
    m_cache->CompileShader.Clear();
    engine.WriteToShader(shader, text);

    std::string errors = engine.CompileShader(m_selectedShader);
    if(errors.empty())
    {
        Logger::LogInfo(shader.Name() + ": Recompiled successfully");
        return true;
    }
    else
    {
        Logger::LogInfo(shader.Name() + ": Failed Recompilation");
        MessageBox(nullptr, (shader.Name() + ":" + errors).c_str(), "Compilation Errors", MB_OK);
        return false;
    }
}

void AppGui::UpdateShader(RenderEngine& engine)
{   
    const int selectedShader = m_cache->ShaderSelected.Get();
    bool changedShader = selectedShader != m_selectedShader;
    bool recompiledShader = false;

    const std::string updatedText = m_cache->CompileShader.Get();
    if(!updatedText.empty() && m_selectedShader != -1)
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

int AppGui::GetSelectedEngine() const
{
    return m_cache->EngineSelected.Get();
}

void AppGui::SetSelectedEngine(int engine)
{
    m_cache->EngineSelected.SetUpdated(engine);

    m_selectedShader = -1; // allows selected shader to be re-cached
    m_selectedMap = -1;    // allows post values to be re-cached
}

void AppGui::UpdateScene(RenderEngine& engine)
{
    UpdateCamera();

    m_cache->FramesPerSec.Set(m_timer.GetFPS());
    m_cache->DeltaTime.Set(m_timer.GetDeltaTime());
    m_cache->Timer.Set(m_timer.GetTotalTime());

    if (m_cache->ReloadScene.Get())
    {
        m_cache->ReloadScene.Set(false);
        m_scene.Reload();

        const auto maxTextures = m_cache->Textures.Get().size();
        for (unsigned int i = 0; i < maxTextures; ++i)
        {
            engine.ReloadTexture(m_data.proceduralTextures[i]);
        }

        const auto maxTerrain = m_cache->Terrains.Get().size();
        for (unsigned int i = 0; i < maxTerrain; ++i)
        {
            engine.ReloadTerrain(i);
        }
    }

    if (m_cache->ReloadEngine.Get())
    {
        m_reloadEngine();
        m_cache->ReloadEngine.Set(false);
    }

    if (m_cache->ReloadPlacement.Get())
    {
        m_scene.ReloadPlacement();
        m_cache->ReloadPlacement.Set(false);
    }
}

void AppGui::UpdateCamera()
{
    m_cache->Camera[Tweakable::Camera::PositionX].SetUpdated(m_camera.Position().x);
    m_cache->Camera[Tweakable::Camera::PositionY].SetUpdated(m_camera.Position().y);
    m_cache->Camera[Tweakable::Camera::PositionZ].SetUpdated(m_camera.Position().z);
    m_cache->Camera[Tweakable::Camera::Pitch].SetUpdated(m_camera.Rotation().x);
    m_cache->Camera[Tweakable::Camera::Yaw].SetUpdated(m_camera.Rotation().y);
    m_cache->Camera[Tweakable::Camera::Roll].SetUpdated(m_camera.Rotation().z);

    m_camera.SetForwardSpeed(m_cache->Camera[Tweakable::Camera::ForwardSpd].Get());
    m_camera.SetRotationSpeed(m_cache->Camera[Tweakable::Camera::RotationSpd].Get());
}

void AppGui::UpdateMesh()
{
    const int selectedMesh = m_cache->MeshSelected.Get();

    if (selectedMesh != m_selectedMesh)
    {
        m_selectedMesh = selectedMesh;
        m_data.meshes[m_selectedMesh]->Write(*m_cache);
    }
    else if (m_selectedMesh >= 0 && 
             m_selectedMesh < static_cast<int>(m_scene.Meshes().size()))
    {
        auto& mesh = *m_data.meshes[m_selectedMesh];
        mesh.Read(*m_cache);
        m_cache->MeshInstances.SetUpdated(mesh.GetRenderedInstances());
    }
}

void AppGui::UpdateWater()
{
    const int selectedWater = m_cache->WaterSelected.Get();
    if(selectedWater != m_selectedWater)
    {
        m_selectedWater = selectedWater;
        m_data.water[m_selectedWater]->Write(*m_cache);
    }
    else if(m_selectedWater >= 0 && 
            m_selectedWater < static_cast<int>(m_scene.Waters().size()))
    {
        auto& water = *m_data.water[m_selectedWater];
        water.Read(*m_cache);
        m_cache->WaterInstances.SetUpdated(water.GetRenderedInstances());
    }

    if (m_selectedWater != -1)
    {
        auto& water = *m_data.water[m_selectedWater];
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

void AppGui::UpdateTerrain(RenderEngine& engine)
{
    const int selectedTerrain = m_cache->TerrainSelected.Get();
    if(selectedTerrain != m_selectedTerrain)
    {
        m_selectedTerrain = selectedTerrain;
        m_data.terrain[m_selectedTerrain]->Write(*m_cache);
    }
    else if(m_selectedTerrain >= 0 && 
            m_selectedTerrain < static_cast<int>(m_scene.Terrains().size()))
    {
        auto& terrain = *m_data.terrain[m_selectedTerrain];
        terrain.Read(*m_cache);
        m_cache->TerrainInstances.SetUpdated(terrain.GetRenderedInstances());
    }

    if (m_cache->ReloadTerrain.Get())
    {
        m_cache->ReloadTerrain.Set(false);
        m_scene.ReloadTerrain(m_selectedTerrain);
        engine.ReloadTerrain(m_selectedTerrain);
    }
}

void AppGui::UpdateTexture(RenderEngine& engine)
{
    const int selectedTexture = m_cache->TextureSelected.Get();
    if(selectedTexture != m_selectedTexture)
    {
        m_selectedTexture = selectedTexture;
        const int ID = m_data.proceduralTextures[m_selectedTexture];
        m_data.textures[ID]->Write(*m_cache);
        m_data.textures[ID]->Save();
        m_cache->TexturePath.SetUpdated(m_data.textures[ID]->Path());
    }
    else if(m_selectedTexture >= 0 && 
            m_selectedTexture < static_cast<int>(m_data.proceduralTextures.size()))
    {
        const int ID = m_data.proceduralTextures[m_selectedTexture];
        m_data.textures[ID]->Read(*m_cache);
    }

    if (m_cache->ReloadTexture.Get())
    {
        const int ID = m_data.proceduralTextures[m_selectedTexture];
        m_scene.ReloadTexture(ID);
        engine.ReloadTexture(ID);
        m_data.textures[ID]->Save();
        m_cache->TexturePath.SetUpdated(m_data.textures[ID]->Path());
        m_cache->ReloadTexture.Set(false);
    }
}

void AppGui::UpdateEmitter()
{
    const int selectedEmitter = m_cache->EmitterSelected.Get();
    if(selectedEmitter != m_selectedEmitter)
    {
        m_selectedEmitter = selectedEmitter;
        m_data.emitters[m_selectedEmitter]->Write(*m_cache);
    }
    else if(m_selectedEmitter >= 0 && 
            m_selectedEmitter < static_cast<int>(m_scene.Emitters().size()))
    {
        auto& emitter = *m_data.emitters[m_selectedEmitter];
        emitter.Read(*m_cache);
        m_cache->EmitterInstances.SetUpdated(emitter.GetRenderedInstances());
    }

    if (m_cache->PauseEmission.Get() && m_selectedEmitter != -1)
    {
        m_data.emitters[m_selectedEmitter]->TogglePaused();
        m_cache->PauseEmission.Set(false);
    }
}

void AppGui::UpdatePost(RenderEngine& engine)
{
    const int selectedMap = m_cache->PostMapSelected.Get();
    if (selectedMap != m_selectedMap)
    {
        m_selectedMap = selectedMap;
        m_scene.SetPostMap(selectedMap);
    }

    m_data.post->Read(*m_cache);

    m_data.caustics->SetSpeed(
        m_cache->Post[Tweakable::Post::CausticSpeed].Get());

    if (m_cache->ToggleWireframe.Get())
    {
        engine.ToggleWireframe();
        m_cache->ToggleWireframe.Set(false);
    }
}

void AppGui::UpdateLight()
{
    const int selectedLight = m_cache->LightSelected.Get();
    if(selectedLight != m_selectedLight)
    {
        m_selectedLight = selectedLight;
        m_data.lights[m_selectedLight]->Write(*m_cache);
    }
    else if(m_selectedLight >= 0 && 
            m_selectedLight < static_cast<int>(m_scene.Lights().size()))
    {
        m_data.lights[m_selectedLight]->Read(*m_cache);
    }

    if (m_cache->LightDiagnostics.Get())
    {
        m_data.diagnostics->ToggleLightDiagnostics();
        m_cache->LightDiagnostics.Set(false);
    }

    if (m_cache->RenderLightsOnly.Get())
    {
        m_data.post->ToggleDiffuseTextures();
        m_cache->RenderLightsOnly.Set(false);
    }
}

void AppGui::Initialise(const std::vector<std::string>& engineNames,
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

    m_cache->Camera[Tweakable::Camera::ForwardSpd].SetUpdated(
        m_camera.GetForwardSpeed());

    m_cache->Camera[Tweakable::Camera::RotationSpd].SetUpdated(
        m_camera.GetRotationSpeed());

    m_cache->Post[Tweakable::Post::CausticSpeed].SetUpdated(
        m_data.caustics->GetSpeed());

    m_data.post->Write(*m_cache);
}

std::vector<std::string> AppGui::GetLightNames() const
{
    std::vector<std::string> lights;
    for(const auto& light : m_scene.Lights())
    {
        lights.push_back(light->Name());
    }
    return lights;
}

std::vector<std::string> AppGui::GetEmitterNames() const
{
    std::vector<std::string> emitters;
    for(const auto& emitter : m_scene.Emitters())
    {
        emitters.push_back(emitter->Name());
    }
    return emitters;
}

std::vector<std::string> AppGui::GetMeshNames() const
{
    std::vector<std::string> meshes;
    for(const auto& mesh : m_scene.Meshes())
    {
        meshes.push_back(mesh->Name());
    }
    return meshes;
}

std::vector<std::string> AppGui::GetWaterNames() const
{
    int index = 0;
    std::vector<std::string> waters;
    for(const auto& water : m_scene.Waters())
    {
        waters.push_back(water->Name());
        ++index;
    }
    return waters;
}

std::vector<std::string> AppGui::GetPostMapNames() const
{
    std::vector<std::string> maps;
    for (int i = 0; i < PostProcessing::Max; ++i)
    {
        maps.push_back(PostProcessing::GetMapName(
            static_cast<PostProcessing::Map>(i)));
    }
    return maps;
}

std::vector<std::string> AppGui::GetTerrainNames() const
{
    int index = 0;
    std::vector<std::string> terrainNames;
    for (const auto& terrain : m_scene.Terrains())
    {
        terrainNames.push_back(terrain->Name());
        ++index;
    }
    return terrainNames;
}

std::vector<std::string> AppGui::GetTextureNames() const
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

std::vector<std::string> AppGui::GetShaderNames() const
{
    std::vector<std::string> shaders;
    for(const auto& shader : m_scene.Shaders())
    {
        shaders.push_back(shader->Name());
    }
    return shaders;
}
