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
        ("post", POST_SHADER)
        ("blur", BLUR_SHADER)
        ("water", WATER_SHADER)
        ("water_normal", WATER_NORMAL_SHADER)
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
    const std::string SAVED("_saved");
    const std::string XML(".xml");
}

Scene::Scene() = default;
Scene::~Scene() = default;

bool Scene::Initialise()
{
    FragmentLinker linker;
    if (InitialisePost() &&
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
            return m1.shaderIndex < m2.shaderIndex;
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
        m_shaders[index].index = index;
        m_shaders[index].name = name;
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
    boost::property_tree::ptree tree = ReadXMLFile(POST_NAME, POST_PATH + XML);
    m_postProcessing.blurAmount = GetValue<float>(tree, "BlurAmount");
    m_postProcessing.blurStep = GetValue<float>(tree, "BlurStep");
    m_postProcessing.depthFar = GetValue<float>(tree, "DepthFar");
    m_postProcessing.depthNear = GetValue<float>(tree, "DepthNear");
    m_postProcessing.dofDistance = GetValue<float>(tree, "DOFDistance");
    m_postProcessing.dofFade = GetValue<float>(tree, "DOFFade");
    m_postProcessing.fogColour.r = GetAttribute<float>(tree, "FogColour", "r");
    m_postProcessing.fogColour.g = GetAttribute<float>(tree, "FogColour", "g");
    m_postProcessing.fogColour.b = GetAttribute<float>(tree, "FogColour", "b");
    m_postProcessing.fogDistance = GetValue<float>(tree, "FogDistance");
    m_postProcessing.fogFade = GetValue<float>(tree, "FogFade");
    m_postProcessing.glowAmount = GetValue<float>(tree, "GlowAmount");
    m_postProcessing.contrast = GetValue<float>(tree, "Contrast");
    m_postProcessing.saturation = GetValue<float>(tree, "Saturation");
    m_postProcessing.maximumColour.r = GetAttribute<float>(tree, "MaximumColour", "r");
    m_postProcessing.maximumColour.g = GetAttribute<float>(tree, "MaximumColour", "g");
    m_postProcessing.maximumColour.b = GetAttribute<float>(tree, "MaximumColour", "b");
    m_postProcessing.minimumColour.r = GetAttribute<float>(tree, "MinimumColour", "r");
    m_postProcessing.minimumColour.g = GetAttribute<float>(tree, "MinimumColour", "g");
    m_postProcessing.minimumColour.b = GetAttribute<float>(tree, "MinimumColour", "b");

    Logger::LogInfo("PostProcessing: Successfully initialised");
    return true;
}

bool Scene::InitialiseLighting()
{
    boost::property_tree::ptree tree = ReadXMLFile(LIGHTS_NAME, LIGHTS_PATH + XML);
    boost::property_tree::ptree::iterator it;
    for (it = tree.begin(); it != tree.end(); ++it)
    {
        m_lights.emplace_back();
        Light& light = m_lights[m_lights.size()-1];

        boost::property_tree::ptree& node = it->second;
        light.name = GetValue<std::string>(node, "Name");
        light.diffuse.r = GetAttribute<float>(node, "Diffuse", "r");
        light.diffuse.g = GetAttribute<float>(node, "Diffuse", "g");
        light.diffuse.b = GetAttribute<float>(node, "Diffuse", "b");
        light.specular.r = GetAttribute<float>(node, "Specular", "r");
        light.specular.g = GetAttribute<float>(node, "Specular", "g");
        light.specular.b = GetAttribute<float>(node, "Specular", "b");
        light.position.x = GetAttribute<float>(node, "Position", "x");
        light.position.y = GetAttribute<float>(node, "Position", "y");
        light.position.z = GetAttribute<float>(node, "Position", "z");
        light.attenuation.x = GetAttribute<float>(node, "Attenuation", "x");
        light.attenuation.y = GetAttribute<float>(node, "Attenuation", "y");
        light.attenuation.z = GetAttribute<float>(node, "Attenuation", "z");
        light.specularity = GetValue<float>(node, "Specularity");
    }

    Logger::LogInfo("Lighting: Successfully initialised");
    return true;
}

bool Scene::InitialiseEmitters()
{
    boost::property_tree::ptree tree = ReadXMLFile(EMITTERS_NAME, EMITTERS_PATH + XML);
    boost::property_tree::ptree::iterator it;
    for (it = tree.begin(); it != tree.end(); ++it)
    {
        m_emitters.emplace_back(it->second, PARTICLE_SHADER);
        Emitter& emitter = m_emitters.at(m_emitters.size()-1);

        for (auto child = it->second.begin(); child != it->second.end(); ++child)
        {
            if (child->first == "Texture")
            {
                emitter.AddTexture(AddTexture(child->second.data()));
            }
        }
    }

    Logger::LogInfo("Particles: Successfully initialised");
    return true;
}

bool Scene::InitialiseDiagnostics()
{
    for (Mesh& mesh : m_meshes)
    {
        if (mesh.shaderIndex == DIAGNOSTIC_SHADER)
        {
            m_diagnostic = std::make_unique<Diagnostic>(mesh.instances);
            break;
        }
    }

    if (!m_diagnostic)
    {
        Logger::LogError("Diagnostics: Could not find mesh");
        return false;
    }
    else
    {
        const float scale = 0.25f;
        for (const Light& light : m_lights)
        {
            m_diagnostic->AddInstance(light, scale);
        }
        return true;
    }
}

bool Scene::InitialiseMeshes(FragmentLinker& linker)
{	
    boost::property_tree::ptree tree = ReadXMLFile(MESHES_NAME, MESHES_PATH + XML);
    boost::property_tree::ptree::iterator itr;
    for(itr = tree.begin(); itr != tree.end(); ++itr)
    {
        const std::string& key = itr->first;
        boost::property_tree::ptree node = itr->second;

        if (key == "Water")
        {
            m_water.emplace_back();
            Water& water = m_water.at(m_water.size()-1);

            InitialiseMeshData(water, node);
            InitialiseWater(water, node);
            if (!CreateMesh(water, false))
            {
                return false;
            }
        }
        else if (key == "Mesh")
        {
            const int amount = GetValueOptional(node, 1, "Instances");
            m_meshes.emplace_back(amount);
            Mesh& mesh = m_meshes.at(m_meshes.size()-1);

            InitialiseMeshData(mesh, node);
            InitialiseMesh(mesh, node);
            InitialiseMeshShader(mesh, linker, node);
            if (!CreateMesh(mesh, true))
            {
                return false;
            }
        }
    }

    return true;
}

bool Scene::CreateMesh(MeshData& mesh, bool hasNormals)
{
    return mesh.Initialise(hasNormals, m_shaders[mesh.shaderIndex].HasComponent(Shader::BUMP));
}

void Scene::InitialiseWater(Water& water, const boost::property_tree::ptree& node)
{
    water.bumpVelocity.x = GetAttribute<float>(node, "BumpVelocity", "x");
    water.bumpVelocity.y = GetAttribute<float>(node, "BumpVelocity", "y");
    water.fresnal.x = GetAttribute<float>(node, "Fresnal", "scale");
    water.fresnal.y = GetAttribute<float>(node, "Fresnal", "bias");
    water.fresnal.z = GetAttribute<float>(node, "Fresnal", "power");
    water.reflection = GetValue<float>(node, "ReflectionIntensity");
    water.speed = GetValue<float>(node, "Speed");
    water.uvScale.x = GetAttribute<float>(node, "UVScale", "u");
    water.uvScale.y = GetAttribute<float>(node, "UVScale", "v");
    water.deepColour.r = GetAttribute<float>(node, "DeepColour", "r");
    water.deepColour.g = GetAttribute<float>(node, "DeepColour", "g");
    water.deepColour.b = GetAttribute<float>(node, "DeepColour", "b");
    water.deepColour.a = GetAttribute<float>(node, "DeepColour", "a");
    water.shallowColour.r = GetAttribute<float>(node, "ShallowColour", "r");
    water.shallowColour.g = GetAttribute<float>(node, "ShallowColour", "g");
    water.shallowColour.b = GetAttribute<float>(node, "ShallowColour", "b");
    water.shallowColour.a = GetAttribute<float>(node, "ShallowColour", "a");
    water.reflectionTint.r = GetAttribute<float>(node, "ReflectionTint", "r");
    water.reflectionTint.g = GetAttribute<float>(node, "ReflectionTint", "g");
    water.reflectionTint.b = GetAttribute<float>(node, "ReflectionTint", "b");
    water.shaderIndex = WATER_SHADER;
    water.normalIndex = WATER_NORMAL_SHADER;

    for (auto itr = node.begin(); itr != node.end(); ++itr)
    {
        if (itr->first == "Wave")
        {
            water.waves.emplace_back();
            Wave& wave = water.waves[water.waves.size()-1];

            const boost::property_tree::ptree& child = itr->second;
            wave.amplitude = GetValue<float>(child, "Amplitude");
            wave.frequency = GetValue<float>(child, "Frequency");
            wave.phase = GetValue<float>(child, "Phase");
            wave.directionX = GetAttribute<float>(child, "Direction", "x");
            wave.directionZ = GetAttribute<float>(child, "Direction", "z");
        }
    }

    // Currently don't support dynamic arrays in shaders
    if (water.waves.size() != Water::MAX_WAVES)
    {
        Logger::LogError("Water: " + water.name + 
            " Did not have required amount of waves");
    }
}

void Scene::InitialiseMeshData(MeshData& mesh, const boost::property_tree::ptree& node)
{
    mesh.name = GetValue<std::string>(node, "Name");
    mesh.bump = GetValueOptional<float>(node, 0.0f, "Bump");

    // Get the textures used by the mesh
    assert(static_cast<int>(mesh.textureIDs.size()) == Texture::MAX_TYPES);
    for (int i = 0; i < Texture::MAX_TYPES; ++i)
    {
        mesh.textureIDs[i] = AddTexture(GetValueOptional(
            node, std::string(), Texture::GetTypeDescription(i).c_str()));
    }

    mesh.maxTextures = mesh.textureIDs.size() - 
        std::count(mesh.textureIDs.begin(), mesh.textureIDs.end(), NO_INDEX);
}

void Scene::InitialiseMesh(Mesh& mesh, const boost::property_tree::ptree& node)
{
    mesh.specularity = GetValueOptional<float>(node, 0.0f, "Specularity");
    mesh.ambience = GetValueOptional<float>(node, 1.0f, "Ambience");
    mesh.glow = GetValueOptional<float>(node, 0.0f, "Intensity");
    mesh.backfacecull = GetValueOptional<bool>(node, true, "BackfaceCulling");
}

void Scene::InitialiseMeshShader(Mesh& mesh, 
                                 FragmentLinker& linker,
                                 const boost::property_tree::ptree& node)
{
    // Get the shader used by the mesh
    std::string shader = GetValue<std::string>(node, "Shader");

    // Ensure not asking for a specialised shader
    const int index = GetShaderIndex(shader);
    if (index != NO_INDEX)
    {
        mesh.shaderIndex = index;
        mesh.normalIndex = GetShaderIndex(NORMAL_SHADER + shader);
    }
    else
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
        mesh.shaderIndex = GetShaderIndex(linker, shader, mesh.name);
        mesh.normalIndex = GetShaderIndex(linker, NORMAL_SHADER + shader, mesh.name);
    }
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
    // Optimization for needing only two types of normal shader (bump/non-bump)
    std::string name = shaderName;
    if (boost::icontains(name, NORMAL_SHADER))
    {
        std::string bump = Shader::ComponentAsString(Shader::BUMP);
        name = NORMAL_SHADER + (boost::icontains(name, bump) ? bump : "");
        boost::to_lower(name);
    }

    // Determine if shader with those components already exists and reuse if so
    auto shaderItr = std::find_if(m_shaders.begin(), m_shaders.end(), 
        [&name](const Shader& shader){ return shader.name == name; });
        
    if(shaderItr == m_shaders.end())
    {
        // Shader does not exist, create from fragments
        Shader shader;
        shader.name = name;
        if(!linker.GenerateWithFragments(shader))
        {
            Logger::LogError("Shader name " + shader.name +
                " for " + meshName + " is an invalid combination");
        }

        shader.index = m_shaders.size();
        m_shaders.push_back(shader);
        Logger::LogInfo("Shader: " + shader.name + " created");
        return shader.index;
    }
    else
    {
        return shaderItr->index;
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
        if(name == m_textures[i].name)
        {
            return i;
        }
    }

    const int index = m_textures.size();
    m_textures.emplace_back();
    m_textures[index].name = name;
    m_textures[index].path = TEXTURE_PATH + name;
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
    return m_postProcessing;
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
    return index == NO_INDEX ? "None" : m_textures[index].name;
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
    return m_postProcessing;
}

Diagnostic& Scene::GetDiagnostics()
{
    return *m_diagnostic;
}

void Scene::SetPostMap(int index)
{
    m_postProcessing.SetPostMap(static_cast<PostProcessing::Map>(index));
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
        lights.push_back(light.name);
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
        meshes.push_back(mesh.name);
    }
    return meshes;
}

std::vector<std::string> Scene::GetWaterNames() const
{
    std::vector<std::string> waters;
    for(const Water& water : m_water)
    {
        waters.push_back(water.name);
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
        shaders.push_back(shader.name);
    }
    return shaders;
}

void Scene::SaveSceneToFile()
{
    SaveParticlesToFile();
    SaveMeshesToFile();
    SaveLightsToFile();
}

void Scene::SaveParticlesToFile()
{
    boost::property_tree::ptree root, tree;
    std::vector<boost::property_tree::ptree> entries;
	
    for (const Emitter& emitter : m_emitters)
    {
        boost::property_tree::ptree entry;
        emitter.Write(entry);
     
        for (int texture : emitter.Textures())
        {
            entry.add("Texture", m_textures[texture].name);
        }

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
        AddMeshDataToTree(mesh, entry);
        AddMeshToTree(mesh, entry);
        entries.push_back(entry);
        tree.add_child("Mesh", entries[entries.size()-1]);
    }

    for(const Water& water : m_water)
    {
        boost::property_tree::ptree entry;
        AddMeshDataToTree(water, entry);
        AddWaterToTree(water, entries, entry);
        entries.push_back(entry);
        tree.add_child("Water", entries[entries.size()-1]);
    }

    SaveXMLFile(root, tree, MESHES_NAME, MESHES_PATH + SAVED + XML);
}

void Scene::AddWaterToTree(const Water& water, 
                           std::vector<boost::property_tree::ptree>& entries,
                           boost::property_tree::ptree& entry)
{
    entry.add("Speed", water.speed);
    entry.add("BumpVelocity.<xmlattr>.x", water.bumpVelocity.x);
    entry.add("BumpVelocity.<xmlattr>.y", water.bumpVelocity.y);
    entry.add("Fresnal.<xmlattr>.scale", water.fresnal.x);
    entry.add("Fresnal.<xmlattr>.bias", water.fresnal.y);
    entry.add("Fresnal.<xmlattr>.power", water.fresnal.z);
    entry.add("ReflectionIntensity", water.reflection);
    entry.add("UVScale.<xmlattr>.u", water.uvScale.x);
    entry.add("UVScale.<xmlattr>.v", water.uvScale.y);
    entry.add("DeepColour.<xmlattr>.r", water.deepColour.r);
    entry.add("DeepColour.<xmlattr>.g", water.deepColour.g);
    entry.add("DeepColour.<xmlattr>.b", water.deepColour.b);
    entry.add("DeepColour.<xmlattr>.a", water.deepColour.a);
    entry.add("ShallowColour.<xmlattr>.r", water.shallowColour.r);
    entry.add("ShallowColour.<xmlattr>.g", water.shallowColour.g);
    entry.add("ShallowColour.<xmlattr>.b", water.shallowColour.b);
    entry.add("ShallowColour.<xmlattr>.a", water.shallowColour.a);
    entry.add("ReflectionTint.<xmlattr>.r", water.reflectionTint.r);
    entry.add("ReflectionTint.<xmlattr>.g", water.reflectionTint.g);
    entry.add("ReflectionTint.<xmlattr>.b", water.reflectionTint.b);

    for (const Wave& wave : water.waves)
    {
        boost::property_tree::ptree waveEntry;
        waveEntry.add("Frequency", wave.frequency);
        waveEntry.add("Amplitude", wave.amplitude);
        waveEntry.add("Phase", wave.phase);
        waveEntry.add("Direction.<xmlattr>.x", wave.directionX);
        waveEntry.add("Direction.<xmlattr>.z", wave.directionZ);
        entries.push_back(waveEntry);
        entry.add_child("Wave", entries[entries.size()-1]);
    }
}

void Scene::AddMeshToTree(const Mesh& mesh, boost::property_tree::ptree& entry)
{
    entry.add("Shader", m_shaders[mesh.shaderIndex].name);
    AddValueOptional(entry, "Intensity", mesh.glow, 0.0f);
    AddValueOptional(entry, "Ambience", mesh.ambience, 1.0f);
    AddValueOptional(entry, "Specularity", mesh.specularity, 0.0f);
    AddValueOptional(entry, "BackfaceCulling", mesh.backfacecull ? 1 : 0, 1);
    AddValueOptional(entry, "Instances", mesh.initialInstances, 1);
}

void Scene::AddMeshDataToTree(const MeshData& mesh, boost::property_tree::ptree& entry)
{
    entry.add("Name", mesh.name.c_str());

    for (int i = 0; i < Texture::MAX_TYPES; ++i)
    {
        if (mesh.textureIDs[i] != NO_INDEX)
        {
            entry.add(Texture::GetTypeDescription(i),
                m_textures[mesh.textureIDs[i]].name);
        }
    };

    AddValueOptional(entry, "Bump", mesh.bump, 0.0f);
}

void Scene::SaveLightsToFile()
{
    boost::property_tree::ptree root, tree;
    std::vector<boost::property_tree::ptree> entries;
	
    for(const Light& light : m_lights)
    {
        boost::property_tree::ptree entry;
        entry.add("Name", light.name.c_str());
        entry.add("Position.<xmlattr>.x", light.position.x);
        entry.add("Position.<xmlattr>.y", light.position.y);
        entry.add("Position.<xmlattr>.z", light.position.z);
        entry.add("Attenuation.<xmlattr>.x", light.attenuation.x);
        entry.add("Attenuation.<xmlattr>.y", light.attenuation.y);
        entry.add("Attenuation.<xmlattr>.z", light.attenuation.z);
        entry.add("Diffuse.<xmlattr>.r", light.diffuse.r);
        entry.add("Diffuse.<xmlattr>.g", light.diffuse.g);
        entry.add("Diffuse.<xmlattr>.b", light.diffuse.b);
        entry.add("Specular.<xmlattr>.r", light.specular.r);
        entry.add("Specular.<xmlattr>.g", light.specular.g);
        entry.add("Specular.<xmlattr>.b", light.specular.b);
        entry.add("Specularity", light.specularity);
        entries.push_back(entry);
        tree.add_child("Light", entries[entries.size()-1]);
    }
    SaveXMLFile(root, tree, LIGHTS_NAME, LIGHTS_PATH + SAVED + XML);
}

void Scene::SavePostProcessingtoFile()
{
    boost::property_tree::ptree root, tree;
    tree.add("BlurAmount", m_postProcessing.blurAmount);
    tree.add("BlurStep", m_postProcessing.blurStep);
    tree.add("DepthFar", m_postProcessing.depthFar);
    tree.add("DepthNear", m_postProcessing.depthNear);
    tree.add("DOFDistance", m_postProcessing.dofDistance);
    tree.add("DOFFade", m_postProcessing.dofFade);
    tree.add("FogColour.<xmlattr>.r", m_postProcessing.fogColour.r);
    tree.add("FogColour.<xmlattr>.g", m_postProcessing.fogColour.g);
    tree.add("FogColour.<xmlattr>.b", m_postProcessing.fogColour.b);
    tree.add("FogDistance", m_postProcessing.fogDistance);
    tree.add("FogFade", m_postProcessing.fogFade);
    tree.add("GlowAmount", m_postProcessing.glowAmount);
    tree.add("Contrast", m_postProcessing.contrast);
    tree.add("Saturation", m_postProcessing.saturation);
    tree.add("MaximumColour.<xmlattr>.r", m_postProcessing.maximumColour.r);
    tree.add("MaximumColour.<xmlattr>.g", m_postProcessing.maximumColour.g);
    tree.add("MaximumColour.<xmlattr>.b", m_postProcessing.maximumColour.b);
    tree.add("MinimumColour.<xmlattr>.r", m_postProcessing.minimumColour.r);
    tree.add("MinimumColour.<xmlattr>.g", m_postProcessing.minimumColour.g);
    tree.add("MinimumColour.<xmlattr>.b", m_postProcessing.minimumColour.b);
    SaveXMLFile(root, tree, POST_NAME, POST_PATH + SAVED + XML);
}

void Scene::Tick(float deltatime)
{
    m_diagnostic->Tick();

    for (Emitter& emitter : m_emitters)
    {
        emitter.Tick(deltatime);
    }
}