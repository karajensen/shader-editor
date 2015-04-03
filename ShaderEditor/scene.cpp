////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - scene.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "scene.h"
#include "textureAnimated.h"
#include "diagnostic.h"
#include "sceneBuilder.h"

/**
* Internal data for the scene
*/
struct SceneData
{
    std::vector<std::unique_ptr<Shader>> shaders;   ///< All shaders in the scene
    std::vector<std::unique_ptr<Mesh>> meshes;      ///< All meshes in the scene
    std::vector<std::unique_ptr<Terrain>> terrain;  ///< All terrain in the scene
    std::vector<std::unique_ptr<Light>> lights;     ///< All lights in the scene
    std::vector<std::unique_ptr<Water>> water;      ///< All water in the scene
    std::vector<std::unique_ptr<Emitter>> emitters; //< All particle emitters in the scene
    std::vector<std::unique_ptr<Texture>> textures; ///< All textures in the scene
    std::vector<unsigned int> proceduralTextures;   ///< Indices of all editable textures
    std::unique_ptr<Diagnostic> diagnostic;         ///< Diagnostics for the scene
    std::unique_ptr<PostProcessing> postProcessing; ///< Post processing for the final image
    std::unique_ptr<AnimatedTexture> caustics;      ///< Caustic animated texture
};

Scene::Scene() = default;
Scene::~Scene() = default;

Shader& Scene::Add(std::unique_ptr<Shader> element)
{
    m_data->shaders.push_back(std::move(element));
    return *m_data->shaders[m_data->shaders.size()-1];
}

Mesh& Scene::Add(std::unique_ptr<Mesh> element)
{
    m_data->meshes.push_back(std::move(element));
    return *m_data->meshes[m_data->meshes.size()-1];
}

Terrain& Scene::Add(std::unique_ptr<Terrain> element)
{
    m_data->terrain.push_back(std::move(element));
    return *m_data->terrain[m_data->terrain.size()-1];
}

void Scene::Add(std::unique_ptr<Light> element)
{
    m_data->lights.push_back(std::move(element));
}

Water& Scene::Add(std::unique_ptr<Water> element)
{
    m_data->water.push_back(std::move(element));
    return *m_data->water[m_data->water.size()-1];
}

Emitter& Scene::Add(std::unique_ptr<Emitter> element)
{
    m_data->emitters.push_back(std::move(element));
    return *m_data->emitters[m_data->emitters.size()-1];
}

void Scene::Add(std::unique_ptr<ProceduralTexture> element)
{
    const unsigned int index = m_data->textures.size();
    m_data->textures.push_back(std::move(element));
    m_data->proceduralTextures.push_back(index);
}

unsigned int Scene::Add(std::unique_ptr<Texture> element)
{
    const unsigned int index = m_data->textures.size();
    m_data->textures.push_back(std::move(element));
    return index;
}

void Scene::Add(std::unique_ptr<PostProcessing> post)
{
    m_data->postProcessing = std::move(post);
}

void Scene::Add(std::unique_ptr<Diagnostic> diagnostics)
{
    m_data->diagnostic = std::move(diagnostics);
}

void Scene::Add(std::unique_ptr<AnimatedTexture> caustics)
{
    m_data->caustics = std::move(caustics);
    for (const std::string& path : m_data->caustics->Paths())
    {
        m_data->caustics->AddFrame(static_cast<int>(m_data->textures.size()));
        m_data->textures.push_back(std::make_unique<Texture>(path, path, Texture::LINEAR));
    }
}

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
    return *m_data->postProcessing;
}

Emitter& Scene::GetEmitter(int index)
{
    return *m_data->emitters[index];
}

Light& Scene::GetLight(int index)
{
    return *m_data->lights[index];
}

Texture& Scene::GetTexture(int index)
{
    return *m_data->textures[index];
}

int Scene::GetTextureID(int index) const
{
    return m_data->proceduralTextures[index];
}

Mesh& Scene::GetMesh(int index)
{
    return *m_data->meshes[index];
}

Water& Scene::GetWater(int index)
{
    return *m_data->water[index];
}

Terrain& Scene::GetTerrain(int index)
{
    return *m_data->terrain[index];
}

Shader& Scene::GetShader(int index)
{
    return *m_data->shaders[index];
}

PostProcessing& Scene::GetPost()
{
    return *m_data->postProcessing;
}

AnimatedTexture& Scene::GetCaustics()
{
    return *m_data->caustics;
}

Diagnostic& Scene::GetDiagnostics()
{
    return *m_data->diagnostic;
}

void Scene::SetPostMap(int index)
{
    m_data->postProcessing->SetPostMap(static_cast<PostProcessing::Map>(index));
}

void Scene::Tick(float deltatime)
{
    m_data->diagnostic->Tick();
    m_data->caustics->Tick(deltatime);

    for (auto& emitter : m_data->emitters)
    {
        emitter->Tick(deltatime);
    }

    const auto caustics = m_data->caustics->GetFrame();

    for (auto& mesh : m_data->meshes)
    {
        if (m_data->shaders[mesh->ShaderID()]->HasComponent(Shader::CAUSTICS))
        {
            mesh->SetTexture(Texture::CAUSTICS, caustics);
        }
    }

    for (auto& terrain : m_data->terrain)
    {
        if (m_data->shaders[terrain->ShaderID()]->HasComponent(Shader::CAUSTICS))
        {
            terrain->SetTexture(Texture::CAUSTICS, caustics);
        }
    }

    // Temporary to test scene
    static float timePassed = 0.0f;
    timePassed += deltatime;
    m_data->meshes[1]->Instances()[0].position.y += cos(timePassed) * 0.05f;
}

void Scene::SaveSceneToFile()
{
    m_builder->SaveSceneToFile();
}

bool Scene::Initialise()
{
    m_data = std::make_unique<SceneData>();
    m_builder = std::make_unique<SceneBuilder>(*this);

    if (m_builder->Initialise())
    {
        // Add light positions for diagnostics
        const float scale = 0.25f;
        for (const auto& light : Lights())
        {
            m_data->diagnostic->AddInstance(*light, scale);
        }

        // To prevent unnecessary shader switching, sort by shader used
        std::sort(m_data->meshes.begin(), m_data->meshes.end(), 
            [](const std::unique_ptr<Mesh>& m1, const std::unique_ptr<Mesh>& m2)->bool
            {
                return m1->ShaderID() < m2->ShaderID();
            });

        return true;
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