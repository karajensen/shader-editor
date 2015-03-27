////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - sceneBuilder.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "sceneBuilder.h"
#include "scene.h"
#include "textureAnimated.h"
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
    const std::string GENERATED_TEXTURES(TEXTURE_PATH + "//Generated//");
    const std::string SAVED("_saved");
    const std::string XML(".xml");
}

SceneBuilder::SceneBuilder(Scene& scene) :
    m_scene(scene)
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
    if (!linker.Initialise(m_scene.Lights().size(), m_scene.Post()))
    {
        return false;
    }

    bool success = true;
    assert(m_scene.Shaders().empty());
    std::vector<std::unique_ptr<Shader>> shaders(SPECIAL_SHADERS.size());

    auto CreateShader = [&shaders, &linker, &success](const std::string& name, int index)
    {
        shaders[index] = std::make_unique<Shader>();
        shaders[index]->Set(name, index);
        if (!linker.GenerateFromFile(*shaders[index]))
        {
            Logger::LogError("Could not generate shader " + name);
            success = false;
        }
        else
        {
            Logger::LogInfo("Shader: " + name + " loaded");
        }
    };

    for (auto& pair : SPECIAL_SHADERS)
    {
        CreateShader(pair.first, pair.second);
    }

    for (auto& shader : shaders)
    {
        m_scene.Add(std::move(shader));
    }
    shaders.clear();

    return success;
}

bool SceneBuilder::InitialisePost()
{
    const boost::property_tree::ptree tree = ReadXMLFile(POST_NAME, POST_PATH + XML);
    m_scene.Add(std::make_unique<PostProcessing>(tree));
    Logger::LogInfo("PostProcessing: Successfully initialised");
    return true;
}

bool SceneBuilder::InitialiseLighting()
{
    boost::property_tree::ptree tree = ReadXMLFile(LIGHTS_NAME, LIGHTS_PATH + XML);
    for (boost::property_tree::ptree::iterator it = tree.begin(); it != tree.end(); ++it)
    {
        m_scene.Add(std::make_unique<Light>(it->second));
    }

    Logger::LogInfo("Lighting: Successfully initialised");
    return true;
}

bool SceneBuilder::InitialiseEmitters()
{
    boost::property_tree::ptree tree = ReadXMLFile(EMITTERS_NAME, EMITTERS_PATH + XML);
    for (boost::property_tree::ptree::iterator it = tree.begin(); it != tree.end(); ++it)
    {
        m_scene.Add(std::make_unique<Emitter>(it->second, PARTICLE_SHADER));
        Emitter& emitter = m_scene.GetEmitter(m_scene.Emitters().size()-1);
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
    for (const auto& mesh : m_scene.Meshes())
    {
        if (mesh->ShaderID() == DIAGNOSTIC_SHADER)
        {
            created = true;
            m_scene.Add(std::make_unique<Diagnostic>(mesh->Instances()));
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
    assert(m_scene.Textures().empty());

    m_scene.Add(std::make_unique<Texture>("Blank", 
        TEXTURE_PATH + "//blank.png", Texture::NEAREST));

    m_scene.Add(std::make_unique<ProceduralTexture>("Random", 
        GENERATED_TEXTURES + "//random.bmp", RANDOM_TEXTURE_SIZE, 
        ProceduralTexture::RANDOM, Texture::NEAREST), true);

    m_scene.Add(std::make_unique<AnimatedTexture>(
        TEXTURE_PATH + "//Caustics//", "Caustics_0", ".bmp"));

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
            const auto index = m_scene.Add(std::make_unique<Water>(node));
            if (!InitialiseWater(m_scene.GetWater(index), node))
            {
                return false;
            }
        }
        else if (itr->first == "Mesh")
        {
            const auto index = m_scene.Add(std::make_unique<Mesh>(node));
            if(!InitialiseMesh(m_scene.GetMesh(index), linker, node))
            {
                return false;
            }
        }
        else if (itr->first == "Terrain")
        {
            const auto index = m_scene.Add(std::make_unique<Terrain>(node));
            if(!InitialiseTerrain(m_scene.GetTerrain(index), linker, node))
            {
                return false;
            }
        }
    }
    return true;
}

bool SceneBuilder::InitialiseWater(Water& water, 
                                   const boost::property_tree::ptree& node)
{
    InitialiseMeshTextures(water);
    water.SetShaderID(m_scene.GetShader(WATER_SHADER).Name(), WATER_SHADER);
    return true;
}

bool SceneBuilder::InitialiseTerrain(Terrain& terrain, 
                                     FragmentLinker& linker,
                                     const boost::property_tree::ptree& node)
{                                
    InitialiseMeshTextures(terrain);
    InitialiseMeshShader(terrain, linker, node);
    return terrain.Initialise();
}

bool SceneBuilder::InitialiseMesh(Mesh& mesh, 
                                  FragmentLinker& linker,
                                  const boost::property_tree::ptree& node)
{                                
    InitialiseMeshTextures(mesh);
    InitialiseMeshShader(mesh, linker, node);
    return mesh.InitialiseFromFile(MESHES_PATH + "//" + mesh.Name(), true, 
        m_scene.GetShader(mesh.ShaderID()).HasComponent(Shader::BUMP));
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

void SceneBuilder::InitialiseMeshShader(MeshData& mesh, 
                                        FragmentLinker& linker,
                                        const boost::property_tree::ptree& node)
{
    // Get the shader used by the mesh
    std::string shader = GetValue<std::string>(node, "Shader");
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

    mesh.SetShaderID(m_scene.GetShader(shaderIndex).Name(), shaderIndex);
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
    const auto& shaders = m_scene.Shaders();

    // Determine if shader with those components already exists and reuse if so
    auto shaderItr = std::find_if(shaders.begin(), shaders.end(), 
        [&name](const std::unique_ptr<Shader>& shader)
        { 
            return shader->Name() == name; 
        });
        
    if(shaderItr == shaders.end())
    {
        // Shader does not exist, create from fragments
        const int index = shaders.size();
        m_scene.Add(std::make_unique<Shader>(name, index));
        Shader& shader = m_scene.GetShader(index);

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

    const auto& textures = m_scene.Textures();
    const int size = static_cast<int>(textures.size());

    for(int i = 0; i < size; ++i)
    {
        if(name == textures[i]->Name())
        {
            return i;
        }
    }

    const std::string path = TEXTURE_PATH + "//" + name;
    Texture::Filter filter = Texture::IsCubeMap(path) ? 
        Texture::ANISOTROPIC : Texture::LINEAR;

    const int index = size;
    m_scene.Add(std::make_unique<Texture>(name, path, filter));
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
	
    for (const auto& emitter : m_scene.Emitters())
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
	
    for(const auto& mesh : m_scene.Meshes())
    {
        boost::property_tree::ptree entry;
        mesh->Write(entry);
        entries.push_back(entry);
        tree.add_child("Mesh", entries[entries.size()-1]);
    }

    auto createNode = [&entries]() -> boost::property_tree::ptree&
    { 
        entries.emplace_back(); 
        return entries.at(entries.size()-1); 
    };

    for(const auto& water : m_scene.Waters())
    {
        boost::property_tree::ptree entry;
        water->Write(entry, createNode);
        entries.push_back(entry);
        tree.add_child("Water", entries[entries.size()-1]);
    }

    SaveXMLFile(root, tree, MESHES_NAME, MESHES_PATH + SAVED + XML);
}

void SceneBuilder::SaveLightsToFile()
{
    boost::property_tree::ptree root, tree;
    std::vector<boost::property_tree::ptree> entries;
	
    for(const auto& light : m_scene.Lights())
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
    m_scene.Post().Write(tree);
    SaveXMLFile(root, tree, POST_NAME, POST_PATH + SAVED + XML);
}

void SceneBuilder::SaveTextureToFile(int ID)
{
    if (!boost::filesystem::exists(GENERATED_TEXTURES))
    {
        boost::filesystem::create_directory(GENERATED_TEXTURES);
    }
    m_scene.GetProceduralTexture(ID).SaveTexture();
}