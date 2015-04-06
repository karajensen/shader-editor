////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - sceneBuilder.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "sceneBuilder.h"
#include "sceneData.h"
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
    const std::string GENERATED_TEXTURES(TEXTURE_PATH + "//Generated//");
    const std::string SAVED("_saved");
    const std::string XML(".xml");
}

SceneBuilder::SceneBuilder(SceneData& data) :
    m_data(data)
{
}

SceneBuilder::~SceneBuilder() = default;

bool SceneBuilder::Initialise()
{
    FragmentLinker linker;
    return InitialiseTextures() &&
           InitialisePost() &&
           InitialiseLighting() &&
           InitialiseShaders(linker) &&
           InitialiseMeshes(linker) &&
           InitialiseEmitters() && 
           InitialiseDiagnostics();
}

bool SceneBuilder::InitialiseShaders(FragmentLinker& linker)
{
    if (!linker.Initialise(m_data.lights.size(), *m_data.post))
    {
        return false;
    }

    assert(m_data.shaders.empty());
    m_data.shaders.resize(SPECIAL_SHADERS.size());

    auto CreateShader = [this, &linker](const std::string& name, int index) -> bool
    {
        m_data.shaders[index] = std::make_unique<Shader>();
        m_data.shaders[index]->Set(name, index);
        if (!linker.GenerateFromFile(*m_data.shaders[index]))
        {
            Logger::LogError("Could not generate shader " + name);
            return false;
        }
        else
        {
            Logger::LogInfo("Shader: " + name + " loaded");
            return true;
        }
    };

    bool success = true;
    for (auto& pair : SPECIAL_SHADERS)
    {
        success &= CreateShader(pair.first, pair.second);
    }
    return success;
}

bool SceneBuilder::InitialisePost()
{
    const boost::property_tree::ptree tree = ReadXMLFile(POST_NAME, POST_PATH + XML);
    m_data.post = std::make_unique<PostProcessing>(tree);
    Logger::LogInfo("PostProcessing: Successfully initialised");
    return true;
}

bool SceneBuilder::InitialiseLighting()
{
    boost::property_tree::ptree tree = ReadXMLFile(LIGHTS_NAME, LIGHTS_PATH + XML);
    for (boost::property_tree::ptree::iterator it = tree.begin(); it != tree.end(); ++it)
    {
        m_data.lights.push_back(std::make_unique<Light>(it->second));
    }

    Logger::LogInfo("Lighting: Successfully initialised");
    return true;
}

bool SceneBuilder::InitialiseEmitters()
{
    boost::property_tree::ptree tree = ReadXMLFile(EMITTERS_NAME, EMITTERS_PATH + XML);
    for (boost::property_tree::ptree::iterator it = tree.begin(); it != tree.end(); ++it)
    {
        m_data.emitters.push_back(std::make_unique<Emitter>(it->second, PARTICLE_SHADER));
        auto& emitter = *m_data.emitters[m_data.emitters.size()-1];

        for (const auto& name : emitter.TextureNames())
        {
            emitter.AddTexture(AddTexture(name));
        }
    }

    Logger::LogInfo("Particles: Successfully initialised");
    return true;
}

bool SceneBuilder::InitialiseDiagnostics()
{
    bool created = false;
    for (const auto& mesh : m_data.meshes)
    {
        if (mesh->ShaderID() == DIAGNOSTIC_SHADER)
        {
            created = true;
            m_data.diagnostics = std::make_unique<Diagnostic>(mesh->Instances());
            break;
        }
    }

    if (!created)
    {
        Logger::LogError("Diagnostics: Could not find mesh");
        return false;
    }
    return true;
}

bool SceneBuilder::InitialiseTextures()
{
    assert(m_data.textures.empty());
    m_data.textures.resize(MAX_TEXTURES);

    m_data.textures[BLANK_TEXTURE_ID] = std::make_unique<Texture>(
        "blank", TEXTURE_PATH + "//blank.png", Texture::NEAREST);

    if (!boost::filesystem::exists(GENERATED_TEXTURES))
    {
        boost::filesystem::create_directory(GENERATED_TEXTURES);
    }

    auto MakeProcedural = [this](const std::string& name,
                                 Texture::Filter filter,
                                 ProceduralTexture::Type type,
                                 int size)
    {
        m_data.proceduralTextures.push_back(m_data.textures.size());
        m_data.textures.push_back(std::make_unique<ProceduralTexture>(name, 
            GENERATED_TEXTURES + "//" + name + ".bmp", size, type, filter));
    };

    MakeProcedural("heightmap", Texture::NEAREST, 
        ProceduralTexture::DIAMOND_SQUARE, 256);

    return InitialiseCaustics();
}

bool SceneBuilder::InitialiseCaustics()
{
    m_data.caustics = std::make_unique<AnimatedTexture>(
        TEXTURE_PATH + "//Caustics//", "Caustics_0", ".bmp");

    for (const std::string& path : m_data.caustics->Paths())
    {
        m_data.caustics->AddFrame(static_cast<int>(m_data.textures.size()));
        m_data.textures.push_back(std::make_unique<Texture>(path, path, Texture::LINEAR));
    }
    return true;
}

bool SceneBuilder::InitialiseMeshes(FragmentLinker& linker)
{	
    boost::property_tree::ptree tree = ReadXMLFile(MESHES_NAME, MESHES_PATH + XML);
    for(boost::property_tree::ptree::iterator itr = tree.begin(); itr != tree.end(); ++itr)
    {
        const boost::property_tree::ptree node = itr->second;

        if (itr->first == "Water")
        {
            if (!InitialiseWater(node))
            {
                return false;
            }
        }
        else if (itr->first == "Mesh")
        {
            if(!InitialiseMesh(node, linker))
            {
                return false;
            }
        }
        else if (itr->first == "Sand" || itr->first == "Terrain")
        {
            if (itr->first == "Sand")
            {
                m_data.sandIndex = m_data.terrain.size();
            }
            
            if(!InitialiseTerrain(node, linker))
            {
                return false;
            }
        }
    }
    return true;
}

bool SceneBuilder::InitialiseWater(const boost::property_tree::ptree& node)
{
    const auto index = m_data.water.size();
    m_data.water.push_back(std::make_unique<Water>(node));
    m_data.water[index]->SetShaderID(WATER_SHADER);
    InitialiseMeshTextures(*m_data.water[index]);
    return true;
}

bool SceneBuilder::InitialiseTerrain(const boost::property_tree::ptree& node, 
                                     FragmentLinker& linker)
{                
    const std::string map = boost::to_lower_copy(GetValue<std::string>(node, "HeightMap"));

    for (const auto& texture : m_data.textures)
    {
        if (boost::iequals(texture->Name(), map))
        {
            const auto index = m_data.terrain.size();
            m_data.terrain.push_back(std::make_unique<Terrain>(node, texture->Pixels(), map));
            auto& terrain = *m_data.terrain[index];

            InitialiseMeshTextures(terrain);
            InitialiseMeshShader(terrain, linker);
            return terrain.Initialise(true, 
                m_data.shaders[terrain.ShaderID()]->HasComponent(Shader::BUMP));
        }
    }

    Logger::LogError("Could not find height map " + map);
    return false;         
}

bool SceneBuilder::InitialiseMesh(const boost::property_tree::ptree& node, FragmentLinker& linker)
{                            
    m_data.meshes.push_back(std::make_unique<Mesh>(node));
    auto& mesh = *m_data.meshes[m_data.meshes.size()-1];

    InitialiseMeshTextures(mesh);
    InitialiseMeshShader(mesh, linker);
    return mesh.InitialiseFromFile(MESHES_PATH + "//" + mesh.Name(), true, 
        m_data.shaders[mesh.ShaderID()]->HasComponent(Shader::BUMP));
}

void SceneBuilder::InitialiseMeshTextures(MeshData& mesh)
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

void SceneBuilder::InitialiseMeshShader(MeshData& mesh, FragmentLinker& linker)
{
    // Get the shader used by the mesh
    std::string shader = mesh.ShaderName();
    boost::algorithm::replace_all(shader, "|", "");

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

    mesh.SetShaderID(shaderIndex);
}

int SceneBuilder::GetShaderIndex(const std::string& shadername)
{
    const auto itr = SPECIAL_SHADERS.find(shadername);
    return itr != SPECIAL_SHADERS.end() ? itr->second : NO_INDEX;
}

int SceneBuilder::GetShaderIndex(FragmentLinker& linker, 
                                 const std::string& shaderName, 
                                 const std::string& meshName)
{
    std::string name = shaderName;

    // Determine if shader with those components already exists and reuse if so
    auto shaderItr = std::find_if(m_data.shaders.begin(), m_data.shaders.end(), 
        [&name](const std::unique_ptr<Shader>& shader)
        { 
            return shader->Name() == name; 
        });
        
    if(shaderItr == m_data.shaders.end())
    {
        // Shader does not exist, create from fragments
        const int index = m_data.shaders.size();

        m_data.shaders.push_back(std::make_unique<Shader>(name, index));
        Shader& shader = *m_data.shaders[m_data.shaders.size()-1];

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
        return (*shaderItr)->ID();
    }
}

int SceneBuilder::AddTexture(const std::string& name)
{
    if(name.empty())
    {
        return NO_INDEX;
    }

    const int size = static_cast<int>(m_data.textures.size());

    for(int i = 0; i < size; ++i)
    {
        if(name == m_data.textures[i]->Name())
        {
            return i;
        }
    }

    const std::string path = TEXTURE_PATH + "//" + name;
    Texture::Filter filter = Texture::IsCubeMap(path) ? 
        Texture::ANISOTROPIC : Texture::LINEAR;

    const int index = size;
    m_data.textures.push_back(std::make_unique<Texture>(name, path, filter));
    return index;
}

void SceneBuilder::SaveSceneToFile()
{
    SaveParticlesToFile();
    SaveMeshesToFile();
    SaveLightsToFile();
    SavePostProcessingtoFile();
}

void SceneBuilder::SaveParticlesToFile()
{
    boost::property_tree::ptree root, tree;
    std::vector<boost::property_tree::ptree> entries;
	
    for (const auto& emitter : m_data.emitters)
    {
        boost::property_tree::ptree entry;
        emitter->Write(entry);
        entries.push_back(entry);
        tree.add_child("Emitter", entries[entries.size()-1]);
    }

    SaveXMLFile(root, tree, EMITTERS_NAME, EMITTERS_PATH + SAVED + XML);
}

void SceneBuilder::SaveMeshesToFile()
{
    boost::property_tree::ptree root, tree;
    std::vector<boost::property_tree::ptree> entries;
	
    for(const auto& mesh : m_data.meshes)
    {
        boost::property_tree::ptree entry;
        mesh->Write(entry);
        entries.push_back(entry);
        tree.add_child("Mesh", entries[entries.size()-1]);
    }

    for (unsigned int i = 0; i < m_data.terrain.size(); ++i)
    {
        const bool isSand = m_data.sandIndex == i;
        boost::property_tree::ptree entry;
        m_data.terrain[i]->Write(entry);
        entries.push_back(entry);
        tree.add_child(isSand ? "Sand" : "Terrain", entries[entries.size()-1]);
    }

    if (!m_data.water.empty())
    {
        auto createNode = [&entries]() -> boost::property_tree::ptree&
        { 
            entries.emplace_back(); 
            return entries.at(entries.size()-1); 
        };

        boost::property_tree::ptree entry;
        m_data.water[0]->Write(entry, createNode);
        entries.push_back(entry);
        tree.add_child("Water", entries[entries.size()-1]);
    }

    SaveXMLFile(root, tree, MESHES_NAME, MESHES_PATH + SAVED + XML);
}

void SceneBuilder::SaveLightsToFile()
{
    boost::property_tree::ptree root, tree;
    std::vector<boost::property_tree::ptree> entries;
	
    for(const auto& light : m_data.lights)
    {
        boost::property_tree::ptree entry;
        light->Write(entry);
        entries.push_back(entry);
        tree.add_child("Light", entries[entries.size()-1]);
    }
    SaveXMLFile(root, tree, LIGHTS_NAME, LIGHTS_PATH + SAVED + XML);
}

void SceneBuilder::SavePostProcessingtoFile()
{
    boost::property_tree::ptree root, tree;
    m_data.post->Write(tree);
    SaveXMLFile(root, tree, POST_NAME, POST_PATH + SAVED + XML);
}
