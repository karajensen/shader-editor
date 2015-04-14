////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - scene.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "scene.h"
#include "camera.h"
#include "sceneData.h"
#include "sceneUpdater.h"
#include "sceneBuilder.h"

Scene::Scene() = default;
Scene::~Scene() = default;

const std::vector<std::unique_ptr<Water>>& Scene::Waters() const
{
    return m_data->water;
}

const std::vector<std::unique_ptr<Mesh>>& Scene::Meshes() const
{
    return m_data->meshes;
}

const std::vector<std::unique_ptr<Shader>>& Scene::Shaders() const
{
    return m_data->shaders;
}

const std::vector<std::unique_ptr<Light>>& Scene::Lights() const
{
    return m_data->lights;
}

const std::vector<std::unique_ptr<Terrain>>& Scene::Terrains() const
{
    return m_data->terrain;
}

const std::vector<std::unique_ptr<Texture>>& Scene::Textures() const
{
    return m_data->textures;
}

const std::vector<std::unique_ptr<Emitter>>& Scene::Emitters() const
{
    return m_data->emitters;
}

const PostProcessing& Scene::Post() const
{
    return *m_data->post;
}

void Scene::SetPostMap(int index)
{
    m_data->post->SetPostMap(static_cast<PostProcessing::Map>(index));
}

void Scene::Tick(float deltatime, const Camera& camera)
{
    const int causticsTexture = m_data->caustics->GetFrame();
    const Float3& position = camera.Position();
    const BoundingArea& bounds = camera.GetBounds();

    m_data->diagnostics->Tick();
    m_data->caustics->Tick(deltatime);
    m_updater->Update(position);

    for (auto& emitter : m_data->emitters)
    {
        emitter->Tick(deltatime, position, bounds);
    }

    for (auto& mesh : m_data->meshes)
    {
        mesh->Tick(position, bounds, causticsTexture);
    }

    for (auto& terrain : m_data->terrain)
    {
        terrain->Tick(position, bounds, causticsTexture);
    }

    for (auto& water : m_data->water)
    {
        water->Tick(position, bounds, causticsTexture);
    }
}

void Scene::SaveSceneToFile()
{
    m_builder->SaveSceneToFile();
}

SceneData& Scene::GetData()
{
    return *m_data;
}

bool Scene::Initialise(const Float3& camera)
{
    m_data = std::make_unique<SceneData>();
    m_builder = std::make_unique<SceneBuilder>(*m_data);
    m_updater = std::make_unique<SceneUpdater>(*m_data);

    if (m_builder->Initialise())
    {
        // Add light positions for diagnostics
        const float scale = 0.25f;
        for (const auto& light : Lights())
        {
            m_data->diagnostics->AddInstance(*light, scale);
        }

        // To prevent unnecessary shader switching, sort by shader used
        std::sort(m_data->meshes.begin(), m_data->meshes.end(), 
            [](const std::unique_ptr<Mesh>& m1, const std::unique_ptr<Mesh>& m2)->bool
            {
                return m1->ShaderID() < m2->ShaderID();
            });

        return m_updater->Initialise(camera);
    }
    return false;
}

void Scene::Reload()
{
    for (int ID : m_data->proceduralTextures)
    {
        m_data->textures[ID]->Reload();
    }

    for (auto& terrain : m_data->terrain)
    {
        terrain->Reload();
    }
}

void Scene::ReloadTexture(int ID)
{
    m_data->textures[ID]->Reload();
}

void Scene::ReloadTerrain(int ID)
{
    m_data->terrain[ID]->Reload();
}