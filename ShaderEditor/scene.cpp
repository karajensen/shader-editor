////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - scene.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "scene.h"
#include "diagnostic.h"
#include "fragmentlinker.h"
#include "renderdata.h"
#include "ptree_utilities.h"
#include "boost/assign.hpp"
#include "boost/filesystem.hpp"
#include "boost/algorithm/string.hpp"
#include <algorithm>

namespace
{
    /**
    * Shaders that do not need to be generated through fragment linking
    */
    std::map<std::string, ShaderIndex> SPECIAL_SHADERS = boost::assign::map_list_of
        ("post_effects", POST_SHADER)
        ("pre_effects", PRE_SHADER)
        ("blur_horizontal", BLUR_HORIZONTAL_SHADER)
        ("blur_vertical", BLUR_VERTICAL_SHADER)
        ("water", WATER_SHADER)
        ("particle", PARTICLE_SHADER)
        ("diagnostic", DIAGNOSTIC_SHADER);

    /**
    * Resource file information
    */
    const std::string EMITTERS_NAME("Emitters");
    const std::string MESHES_NAME("Meshes");
    const std::string POST_NAME("PostProcessing");
    const std::string LIGHTS_NAME("Lights");
    const std::string EMITTERS_PATH(ASSETS_PATH + EMITTERS_NAME);
    const std::string MESHES_PATH(ASSETS_PATH + MESHES_NAME);
    const std::string LIGHTS_PATH(ASSETS_PATH + LIGHTS_NAME);
    const std::string POST_PATH(ASSETS_PATH + POST_NAME);
    const std::string TEXTURE_PATH(ASSETS_PATH + "Textures");
    const std::string SAVED("_saved");
    const std::string XML(".xml");
    const std::string BLANK_TEXTURE("blank.png");
}

Scene::Scene() = default;
Scene::~Scene() = default;

bool Scene::Initialise()
{
    FragmentLinker linker;
    if (InitialiseTextures() &&
        InitialisePost() &&
        InitialiseLighting() &&
        linker.Initialise(m_lights.size()) &&
        InitialiseShaders(linker) &&
        InitialiseMeshes(linker) &&
        InitialiseEmitters() && 
        InitialiseDiagnostics())
    {
        // To prevent unnecessary shader switching, sort by shader used
        std::sort(m_meshes.begin(), m_meshes.end(), [](const Mesh& m1, const Mesh& m2)->bool
        {
            return m1.ShaderID() < m2.ShaderID();
        });
        return true;
    }
    return false;
}

bool Scene::InitialiseShaders(FragmentLinker& linker)
{
    bool success = true;

    auto CreateShader = [this, &linker, &success](const std::string& name, int index)
    {
        m_shaders[index].Set(name, index);
        if (!linker.GenerateFromFile(m_shaders[index]))
        {
            Logger::LogError("Could not generate shader " + name);
            success = false;
        }
        else
        {
            Logger::LogInfo("Shader: " + name + " loaded");
        }
    };

    assert(m_shaders.empty());
    m_shaders.resize(SPECIAL_SHADERS.size());

    for (auto& pair : SPECIAL_SHADERS)
    {
        CreateShader(pair.first, pair.second);
    }

    return success;
}

bool Scene::InitialisePost()
{
    const boost::property_tree::ptree tree = ReadXMLFile(POST_NAME, POST_PATH + XML);
    m_postProcessing = std::make_unique<PostProcessing>(tree);
    Logger::LogInfo("PostProcessing: Successfully initialised");
    return true;
}

bool Scene::InitialiseLighting()
{
    boost::property_tree::ptree tree = ReadXMLFile(LIGHTS_NAME, LIGHTS_PATH + XML);
    for (boost::property_tree::ptree::iterator it = tree.begin(); it != tree.end(); ++it)
    {
        m_lights.emplace_back(it->second);
    }

    Logger::LogInfo("Lighting: Successfully initialised");
    return true;
}

bool Scene::InitialiseEmitters()
{
    boost::property_tree::ptree tree = ReadXMLFile(EMITTERS_NAME, EMITTERS_PATH + XML);
    for (boost::property_tree::ptree::iterator it = tree.begin(); it != tree.end(); ++it)
    {
        m_emitters.emplace_back(it->second, PARTICLE_SHADER);
        Emitter& emitter = m_emitters.at(m_emitters.size()-1);
        for (const auto& name : emitter.TextureNames())
        {
            emitter.AddTexture(AddTexture(name));
        }
    }

    Logger::LogInfo("Particles: Successfully initialised");
    return true;
}

bool Scene::InitialiseDiagnostics()
{
    for (Mesh& mesh : m_meshes)
    {
        if (mesh.ShaderID() == DIAGNOSTIC_SHADER)
        {
            m_diagnostic = std::make_unique<Diagnostic>(mesh.Instances());
            break;
        }
    }

    if (!m_diagnostic)
    {
        Logger::LogError("Diagnostics: Could not find mesh");
        return false;
    }

    const float scale = 0.25f;
    for (const Light& light : m_lights)
    {
        m_diagnostic->AddInstance(light, scale);
    }
    return true;
}

bool Scene::InitialiseTextures()
{
    assert(m_textures.empty());
    m_textures.emplace_back(BLANK_TEXTURE, TEXTURE_PATH + "//" + BLANK_TEXTURE);
    return true;
}

bool Scene::InitialiseMeshes(FragmentLinker& linker)
{	
    boost::property_tree::ptree tree = ReadXMLFile(MESHES_NAME, MESHES_PATH + XML);
    for(boost::property_tree::ptree::iterator itr = tree.begin(); itr != tree.end(); ++itr)
    {
        const boost::property_tree::ptree node = itr->second;

        if (itr->first == "Water")
        {
            m_water.emplace_back(node);
            if (!InitialiseWater(m_water.at(m_water.size()-1), node))
            {
                return false;
            }
        }
        else if (itr->first == "Mesh")
        {
            m_meshes.emplace_back(node);
            if(!InitialiseMesh(m_meshes.at(m_meshes.size()-1), linker, node))
            {
                return false;
            }
        }
    }
    return true;
}

bool Scene::CreateMesh(MeshData& mesh, bool hasNormals)
{
    return mesh.Initialise(MESHES_PATH + "//" + mesh.Name(), hasNormals, 
        m_shaders[mesh.ShaderID()].HasComponent(Shader::BUMP));
}

bool Scene::InitialiseWater(Water& water, 
                            const boost::property_tree::ptree& node)
{
    InitialiseMeshTextures(water);
    water.SetShaderID(m_shaders[WATER_SHADER].Name(), WATER_SHADER);
    return CreateMesh(water, false);
}

bool Scene::InitialiseMesh(Mesh& mesh, 
                           FragmentLinker& linker,
                           const boost::property_tree::ptree& node)
{
    InitialiseMeshTextures(mesh);
    InitialiseMeshShader(mesh, linker, node);
    return CreateMesh(mesh, true);
}

void Scene::InitialiseMeshTextures(MeshData& mesh)
{
    for (int i = 0; i < Texture::MAX_TYPES; ++i)
    {
        const auto type = static_cast<Texture::Type>(i);
        const std::string name = mesh.TextureNames().at(type);
        if (!name.empty())
        {
            mesh.SetTexture(type, AddTexture(name));
        }
    }
}

void Scene::InitialiseMeshShader(Mesh& mesh, 
                                 FragmentLinker& linker,
                                 const boost::property_tree::ptree& node)
{
    // Get the shader used by the mesh
    std::string shader = GetValue<std::string>(node, "Shader");

    // Ensure not asking for a specialised shader
    int shaderIndex = GetShaderIndex(shader);
    if (shaderIndex == NO_INDEX)
    {
        // Copy each component featured in the given shader name to a set order
        std::string newShaderName;
        std::string component;
        for(int i = 0; i < Shader::MAX_COMPONENTS; ++i)
        {
            component = Shader::ComponentAsString(i);
            if(boost::algorithm::icontains(shader, component))
            {
                newShaderName += component;
                boost::algorithm::ireplace_all(shader, component, "");
            }
        }
	
        // Add any non-component text to the ordered components
        shader += boost::algorithm::to_lower_copy(newShaderName);
        shaderIndex = GetShaderIndex(linker, shader, mesh.Name());
    }

    mesh.SetShaderID(m_shaders[shaderIndex].Name(), shaderIndex);
}

int Scene::GetShaderIndex(const std::string& shadername)
{
    const auto itr = SPECIAL_SHADERS.find(shadername);
    return itr != SPECIAL_SHADERS.end() ? itr->second : NO_INDEX;
}

int Scene::GetShaderIndex(FragmentLinker& linker, 
                          const std::string& shaderName, 
                          const std::string& meshName)
{
    std::string name = shaderName;

    // Determine if shader with those components already exists and reuse if so
    auto shaderItr = std::find_if(m_shaders.begin(), m_shaders.end(), 
        [&name](const Shader& shader){ return shader.Name() == name; });
        
    if(shaderItr == m_shaders.end())
    {
        // Shader does not exist, create from fragments
        const int index = m_shaders.size();
        m_shaders.emplace_back(name, index);
        Shader& shader = m_shaders.at(index);

        if(!linker.GenerateWithFragments(shader))
        {
            Logger::LogError("Shader name " + shader.Name() +
                " for " + meshName + " is an invalid combination");
        }
        else
        {
            Logger::LogInfo("Shader: " + shader.Name() + " created");
        }
        return shader.ID();
    }
    else
    {
        return shaderItr->ID();
    }
}

int Scene::AddTexture(const std::string& name)
{
    if(name.empty())
    {
        return NO_INDEX;
    }

    for(int i = 0; i < static_cast<int>(m_textures.size()); ++i)
    {
        if(name == m_textures[i].Name())
        {
            return i;
        }
    }

    const int index = m_textures.size();
    m_textures.emplace_back(name, TEXTURE_PATH + "//" + name);
    return index;
}

const std::vector<Water>& Scene::Waters() const
{
    return m_water;
}

const std::vector<Mesh>& Scene::Meshes() const
{
    return m_meshes;
}

const std::vector<Shader>& Scene::Shaders() const
{
    return m_shaders;
}

const std::vector<Light>& Scene::Lights() const
{
    return m_lights;
}

const std::vector<Texture>& Scene::Textures() const
{
    return m_textures;
}

const std::vector<Emitter>& Scene::Emitters() const
{
    return m_emitters;
}

const PostProcessing& Scene::Post() const
{
    return *m_postProcessing;
}

Emitter& Scene::GetEmitter(int index)
{
    return m_emitters[index];
}

Light& Scene::GetLight(int index)
{
    return m_lights[index];
}

std::string Scene::GetTexture(int index)
{
    return index == NO_INDEX ? "None" : m_textures[index].Name();
}

Mesh& Scene::GetMesh(int index)
{
    return m_meshes[index];
}

Water& Scene::GetWater(int index)
{
    return m_water[index];
}

Shader& Scene::GetShader(int index)
{
    return m_shaders[index];
}

PostProcessing& Scene::GetPost()
{
    return *m_postProcessing;
}

Diagnostic& Scene::GetDiagnostics()
{
    return *m_diagnostic;
}

void Scene::SetPostMap(int index)
{
    m_postProcessing->SetPostMap(static_cast<PostProcessing::Map>(index));
}

int Scene::GetMeshCount() const
{
    return static_cast<int>(m_meshes.size());
}

int Scene::GetEmitterCount() const
{
    return static_cast<int>(m_emitters.size());
}

int Scene::GetWaterCount() const
{
    return static_cast<int>(m_water.size());
}

int Scene::GetLightCount() const
{
    return static_cast<int>(m_lights.size());
}

std::vector<std::string> Scene::GetLightNames() const
{
    std::vector<std::string> lights;
    for(const Light& light : m_lights)
    {
        lights.push_back(light.Name());
    }
    return lights;
}

std::vector<std::string> Scene::GetEmitterNames() const
{
    std::vector<std::string> emitters;
    for(const Emitter& emitter : m_emitters)
    {
        emitters.push_back(emitter.Name());
    }
    return emitters;
}

std::vector<std::string> Scene::GetMeshNames() const
{
    std::vector<std::string> meshes;
    for(const Mesh& mesh : m_meshes)
    {
        meshes.push_back(mesh.Name());
    }
    return meshes;
}

std::vector<std::string> Scene::GetWaterNames() const
{
    std::vector<std::string> waters;
    for(const Water& water : m_water)
    {
        waters.push_back(water.Name());
    }
    return waters;
}

std::vector<std::string> Scene::GetPostMapNames() const
{
    std::vector<std::string> maps;
    for (int i = 0; i < PostProcessing::MAX_MAPS; ++i)
    {
        maps.push_back(PostProcessing::GetMapName(
            static_cast<PostProcessing::Map>(i)));
    }
    return maps;
}

std::vector<std::string> Scene::GetShaderNames() const
{
    std::vector<std::string> shaders;
    for(const Shader& shader : m_shaders)
    {
        shaders.push_back(shader.Name());
    }
    return shaders;
}

void Scene::SaveSceneToFile()
{
    SaveParticlesToFile();
    SaveMeshesToFile();
    SaveLightsToFile();
    SavePostProcessingtoFile();
}

void Scene::SaveParticlesToFile()
{
    boost::property_tree::ptree root, tree;
    std::vector<boost::property_tree::ptree> entries;
	
    for (const Emitter& emitter : m_emitters)
    {
        boost::property_tree::ptree entry;
        emitter.Write(entry);
        entries.push_back(entry);
        tree.add_child("Emitter", entries[entries.size()-1]);
    }

    SaveXMLFile(root, tree, EMITTERS_NAME, EMITTERS_PATH + SAVED + XML);
}

void Scene::SaveMeshesToFile()
{
    boost::property_tree::ptree root, tree;
    std::vector<boost::property_tree::ptree> entries;
	
    for(const Mesh& mesh : m_meshes)
    {
        boost::property_tree::ptree entry;
        mesh.Write(entry);
        entries.push_back(entry);
        tree.add_child("Mesh", entries[entries.size()-1]);
    }

    auto createNode = [&entries]() -> boost::property_tree::ptree&
    { 
        entries.emplace_back(); 
        return entries.at(entries.size()-1); 
    };

    for(const Water& water : m_water)
    {
        boost::property_tree::ptree entry;
        water.Write(entry, createNode);
        entries.push_back(entry);
        tree.add_child("Water", entries[entries.size()-1]);
    }

    SaveXMLFile(root, tree, MESHES_NAME, MESHES_PATH + SAVED + XML);
}

void Scene::SaveLightsToFile()
{
    boost::property_tree::ptree root, tree;
    std::vector<boost::property_tree::ptree> entries;
	
    for(const Light& light : m_lights)
    {
        boost::property_tree::ptree entry;
        light.Write(entry);
        entries.push_back(entry);
        tree.add_child("Light", entries[entries.size()-1]);
    }
    SaveXMLFile(root, tree, LIGHTS_NAME, LIGHTS_PATH + SAVED + XML);
}

void Scene::SavePostProcessingtoFile()
{
    boost::property_tree::ptree root, tree;
    m_postProcessing->Write(tree);
    SaveXMLFile(root, tree, POST_NAME, POST_PATH + SAVED + XML);
}

void Scene::Tick(float deltatime)
{
    m_diagnostic->Tick();

    for (Emitter& emitter : m_emitters)
    {
        emitter.Tick(deltatime);
    }

    // Temporary to test specularity
    static float timePassed = 0.0f;
    timePassed += deltatime;
    m_meshes[1].Instances()[0].position.y += cos(timePassed) * 0.05f;
}