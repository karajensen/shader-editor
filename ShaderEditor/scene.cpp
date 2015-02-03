////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - scene.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "scene.h"
#include "ptree_utilities.h"
#include <algorithm>
#include "boost/filesystem.hpp"
#include "boost/property_tree/xml_parser.hpp"
#include "boost/algorithm/string.hpp"
#include "fragmentlinker.h"

namespace
{
    const bool OVERRIDE_SHADERS = false; ///< Whether to do a global override of shader names
    const std::string GLOBAL_SHADER("Bump"); ///< Shader to override all meshes
}

bool Scene::Initialise()
{
    FragmentLinker linker;
    if (InitialisePost() &&
        InitialiseLighting() &&
        linker.Initialise(m_lights.size()) &&
        InitialiseShaders(linker) &&
        InitialiseMeshes(linker) &&
        InitialiseEmitters())
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

void Scene::SaveXMLFile(boost::property_tree::ptree& root,
                       boost::property_tree::ptree& tree,
                       const std::string& name)
{
    root.add_child(name, tree);
    boost::filesystem::path filePath(ASSETS_PATH + name + "_saved.xml");
    boost::property_tree::xml_parser::xml_writer_settings<char> settings('\t', 1);
    boost::property_tree::write_xml(filePath.generic_string(), root, std::locale(), settings);
}

boost::property_tree::ptree Scene::ReadXMLFile(const std::string& name)
{
    boost::property_tree::ptree root;
    boost::property_tree::xml_parser::read_xml(ASSETS_PATH + name + ".xml", 
        root, boost::property_tree::xml_parser::trim_whitespace);
    return root.get_child(name);
}

bool Scene::InitialiseShaders(FragmentLinker& linker)
{
    bool success = true;

    auto CreateShader = [this, &linker, &success](const std::string& name, int index)
    {
        assert(index == m_shaders.size());
        Shader shader;
        shader.index = index;
        shader.name = name;
        if (!linker.GenerateFromFile(shader))
        {
            Logger::LogError("Could not generate shader " + name);
            success = false;
        }
        else
        {
            Logger::LogInfo("Shader: " + name + " loaded");
            m_shaders.push_back(shader);
        }
    };

    CreateShader(POST_SHADER, POST_SHADER_INDEX);
    CreateShader(BLUR_SHADER, BLUR_SHADER_INDEX);
    CreateShader(WATER_SHADER, WATER_SHADER_INDEX);
    CreateShader(WATER_NORMAL_SHADER, WATER_NORMAL_SHADER_INDEX);
    CreateShader(PARTICLE_SHADER, PARTICLE_SHADER_INDEX);
    CreateShader(PARTICLE_NORMAL_SHADER, PARTICLE_NORMAL_SHADER_INDEX);

    return success;
}

bool Scene::InitialisePost()
{
    boost::property_tree::ptree tree = ReadXMLFile("PostProcessing");
    m_postProcessing.blurAmount = GetValue<float>(tree, "BlurAmount");
    m_postProcessing.blurStep = GetValue<float>(tree, "BlurStep");
    m_postProcessing.depthFar = GetValue<float>(tree, "DepthFar");
    m_postProcessing.depthNear = GetValue<float>(tree, "DepthNear");
    m_postProcessing.dofDistance = GetValue<float>(tree, "DOFDistance");
    m_postProcessing.dofFade = GetValue<float>(tree, "DOFFade");
    m_postProcessing.fogColour.r = GetValue<float>(tree, "FogColourR");
    m_postProcessing.fogColour.g = GetValue<float>(tree, "FogColourG");
    m_postProcessing.fogColour.b = GetValue<float>(tree, "FogColourB");
    m_postProcessing.fogDistance = GetValue<float>(tree, "FogDistance");
    m_postProcessing.fogFade = GetValue<float>(tree, "FogFade");
    m_postProcessing.glowAmount = GetValue<float>(tree, "GlowAmount");
    m_postProcessing.contrast = GetValue<float>(tree, "Contrast");
    m_postProcessing.saturation = GetValue<float>(tree, "Saturation");
    m_postProcessing.maximumColour.r = GetValue<float>(tree, "MaximumColourR");
    m_postProcessing.maximumColour.g = GetValue<float>(tree, "MaximumColourG");
    m_postProcessing.maximumColour.b = GetValue<float>(tree, "MaximumColourB");
    m_postProcessing.minimumColour.r = GetValue<float>(tree, "MinimumColourR");
    m_postProcessing.minimumColour.g = GetValue<float>(tree, "MinimumColourG");
    m_postProcessing.minimumColour.b = GetValue<float>(tree, "MinimumColourB");

    Logger::LogInfo("PostProcessing: Successfully initialised");
    return true;
}

bool Scene::InitialiseLighting()
{
    boost::property_tree::ptree tree = ReadXMLFile("Lights");
    boost::property_tree::ptree::iterator it;
    for (it = tree.begin(); it != tree.end(); ++it)
    {
        Light light;
        light.name = GetValue<std::string>(it, "Name");

        light.diffuse.a = 1.0f;
        light.diffuse.r = GetValue<float>(it, "R");
        light.diffuse.b = GetValue<float>(it, "B");
        light.diffuse.g = GetValue<float>(it, "G");

        light.specular.a = 1.0f;
        light.specular.r = GetValue<float>(it, "SR");
        light.specular.b = GetValue<float>(it, "SB");
        light.specular.g = GetValue<float>(it, "SG");

        light.position.x = GetValue<float>(it, "X");
        light.position.y = GetValue<float>(it, "Y");
        light.position.z = GetValue<float>(it, "Z");

        light.specularity = GetValue<float>(it, "Specularity");
        light.attenuation.x = GetValue<float>(it, "AttX");
        light.attenuation.y = GetValue<float>(it, "AttY");
        light.attenuation.z = GetValue<float>(it, "AttZ");

        m_lights.push_back(light);
    }

    Logger::LogInfo("Lighting: Successfully initialised");
    return true;
}

bool Scene::InitialiseEmitters()
{
    boost::property_tree::ptree tree = ReadXMLFile("Emitters");
    boost::property_tree::ptree::iterator it;
    for (it = tree.begin(); it != tree.end(); ++it)
    {
        Emitter emitter;
        emitter.Resize(GetValue<int>(it, "Amount"));
        emitter.width = GetValue<float>(it, "Width");
        emitter.length = GetValue<float>(it, "Length");
        emitter.lifeTime = GetValue<float>(it, "LifeTime");
        emitter.speed = GetValue<float>(it, "Speed");
        emitter.speedVariation = GetValue<float>(it, "SpeedVariation");
        emitter.size = GetValue<float>(it, "Size");
        emitter.sizeVariation = GetValue<float>(it, "SizeVariation");
        emitter.name = GetValue<std::string>(it, "Name");
        emitter.position.x = GetAttribute<float>(it, "Position", "x");
        emitter.position.y = GetAttribute<float>(it, "Position", "y");
        emitter.position.z = GetAttribute<float>(it, "Position", "z");
        emitter.direction.x = GetAttribute<float>(it, "Direction", "x");
        emitter.direction.y = GetAttribute<float>(it, "Direction", "y");
        emitter.direction.z = GetAttribute<float>(it, "Direction", "z");
        emitter.tint.r = GetAttribute<float>(it, "Tint", "r");
        emitter.tint.g = GetAttribute<float>(it, "Tint", "g");
        emitter.tint.b = GetAttribute<float>(it, "Tint", "b");
        emitter.shaderIndex = PARTICLE_SHADER_INDEX;
        emitter.normalIndex = PARTICLE_NORMAL_SHADER_INDEX;

        for (auto child = it->second.begin(); child != it->second.end(); ++child)
        {
            if (child->first == "Texture")
            {
                emitter.textures.push_back(AddTexture(child->second.data()));
            }
        }

        m_emitters.push_back(emitter);
    }

    Logger::LogInfo("Particles: Successfully initialised");
    return true;
}

bool Scene::InitialiseMeshes(FragmentLinker& linker)
{	
    boost::property_tree::ptree tree = ReadXMLFile("Meshes");
    boost::property_tree::ptree::iterator it;
    for(it = tree.begin(); it != tree.end(); ++it)
    {
        if (it->first == "Water")
        {
            Water water;
            InitialiseMesh(water, it);
            InitialiseWater(water, it);
            if (water.Initialise(false, false))
            {
                m_water.push_back(water);
            }
        }
        else if (it->first == "Mesh")
        {
            Mesh mesh;
            InitialiseMesh(mesh, it);
            InitialiseMeshShader(mesh, linker, it);
            const auto& shader = m_shaders[mesh.shaderIndex];
            if (mesh.Initialise(true, shader.HasComponent(Shader::BUMP)))
            {
                m_meshes.push_back(mesh);
            }
        }
    }

    return true;
}

void Scene::InitialiseWater(Water& water, boost::property_tree::ptree::iterator& it)
{
    water.bumpVelocity.x = GetAttribute<float>(it, "BumpVelocity", "x");
    water.bumpVelocity.y = GetAttribute<float>(it, "BumpVelocity", "y");
    water.fresnal.x = GetAttribute<float>(it, "Fresnal", "scale");
    water.fresnal.y = GetAttribute<float>(it, "Fresnal", "bias");
    water.fresnal.z = GetAttribute<float>(it, "Fresnal", "power");
    water.reflection = GetValue<float>(it, "ReflectionIntensity");
    water.speed = GetValue<float>(it, "Speed");
    water.uvScale.x = GetAttribute<float>(it, "UVScale", "u");
    water.uvScale.y = GetAttribute<float>(it, "UVScale", "v");
    water.deepColour.r = GetAttribute<float>(it, "DeepColour", "r");
    water.deepColour.g = GetAttribute<float>(it, "DeepColour", "g");
    water.deepColour.b = GetAttribute<float>(it, "DeepColour", "b");
    water.deepColour.a = GetAttribute<float>(it, "DeepColour", "a");
    water.shallowColour.r = GetAttribute<float>(it, "ShallowColour", "r");
    water.shallowColour.g = GetAttribute<float>(it, "ShallowColour", "g");
    water.shallowColour.b = GetAttribute<float>(it, "ShallowColour", "b");
    water.shallowColour.a = GetAttribute<float>(it, "ShallowColour", "a");
    water.reflectionTint.r = GetAttribute<float>(it, "ReflectionTint", "r");
    water.reflectionTint.g = GetAttribute<float>(it, "ReflectionTint", "g");
    water.reflectionTint.b = GetAttribute<float>(it, "ReflectionTint", "b");
    water.shaderIndex = WATER_SHADER_INDEX;
    water.normalIndex = WATER_NORMAL_SHADER_INDEX;

    for (auto child = it->second.begin(); child != it->second.end(); ++child)
    {
        if (child->first == "Wave")
        {
            Wave wave;
            wave.amplitude = GetValue<float>(child, "Amplitude");
            wave.frequency = GetValue<float>(child, "Frequency");
            wave.phase = GetValue<float>(child, "Phase");
            wave.directionX = GetAttribute<float>(child, "Direction", "x");
            wave.directionZ = GetAttribute<float>(child, "Direction", "z");
            water.waves.push_back(wave);
        }
    }

    // Currently don't support dynamic arrays in shaders
    if (water.waves.size() != Water::MAX_WAVES)
    {
        Logger::LogError("Water: " + water.name + 
            " Did not have required amount of waves");
    }
}

void Scene::InitialiseMesh(Mesh& mesh, boost::property_tree::ptree::iterator& it)
{
    mesh.name = GetValue<std::string>(it, "Name");
    mesh.specularity = GetValueOptional(it, 5.0f, "Specularity");
    mesh.ambience = GetValueOptional(it, 1.0f, "Ambience");
    mesh.bump = GetValueOptional(it, 1.0f, "Bump");
    mesh.glow = GetValueOptional(it, 1.0f, "Intensity");
    mesh.backfacecull = GetValueOptional(it, true, "BackfaceCulling");
	
    // Get the textures used by the mesh
    assert(static_cast<int>(mesh.textureIDs.size()) == Texture::MAX_TYPES);
    for (int i = 0; i < Texture::MAX_TYPES; ++i)
    {
        mesh.textureIDs[i] = AddTexture(GetValueOptional(
            it, std::string(), Texture::GetTypeDescription(i).c_str()));
    }

    const int unusedTextures = std::count(
        mesh.textureIDs.begin(), mesh.textureIDs.end(), NO_INDEX);
    mesh.maxTextures = mesh.textureIDs.size() - unusedTextures;
}

void Scene::InitialiseMeshShader(Mesh& mesh, 
                                 FragmentLinker& linker,
                                 boost::property_tree::ptree::iterator& it)
{
    // Get the shader used by the mesh
    std::string shader = OVERRIDE_SHADERS ? GLOBAL_SHADER : 
        GetValue<std::string>(it, "Shader");

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

    Texture texture;
    texture.name = name;
    texture.path = TEXTURE_PATH + name;
    m_textures.push_back(texture);
    return m_textures.size()-1;
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
        emitters.push_back(emitter.name);
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
        entry.add("Name", emitter.name.c_str());
        entry.add("Width", emitter.width);
        entry.add("Length", emitter.length);
        entry.add("Amount", emitter.particles.size());
        entry.add("LifeTime", emitter.lifeTime);
        entry.add("Speed", emitter.speed);
        entry.add("SpeedVariation", emitter.speedVariation);
        entry.add("Size", emitter.size);
        entry.add("SizeVariation", emitter.sizeVariation);
        entry.add("Position.<xmlattr>.x", emitter.position.x);
        entry.add("Position.<xmlattr>.y", emitter.position.y);
        entry.add("Position.<xmlattr>.z", emitter.position.z);
        entry.add("Direction.<xmlattr>.x", emitter.direction.x);
        entry.add("Direction.<xmlattr>.y", emitter.direction.y);
        entry.add("Direction.<xmlattr>.z", emitter.direction.z);
        entry.add("Tint.<xmlattr>.r", emitter.tint.r);
        entry.add("Tint.<xmlattr>.g", emitter.tint.g);
        entry.add("Tint.<xmlattr>.b", emitter.tint.b);

        for (int texture : emitter.textures)
        {
            entry.add("Texture", m_textures[texture].name);
        }

        entries.push_back(entry);
        tree.add_child("Emitter", entries[entries.size()-1]);
    }

    SaveXMLFile(root, tree, "Emitters");
}

void Scene::SaveMeshesToFile()
{
    boost::property_tree::ptree root, tree;
    std::vector<boost::property_tree::ptree> entries;
	
    for(const Mesh& mesh : m_meshes)
    {
        boost::property_tree::ptree entry;
        AddMeshToTree(mesh, entry);
        entries.push_back(entry);
        tree.add_child("Mesh", entries[entries.size()-1]);
    }

    for(const Water& water : m_water)
    {
        boost::property_tree::ptree entry;
        AddWaterToTree(water, entries, entry);
        entries.push_back(entry);
        tree.add_child("Water", entries[entries.size()-1]);
    }

    SaveXMLFile(root, tree, "Meshes");
}

void Scene::AddWaterToTree(const Water& water, 
                           std::vector<boost::property_tree::ptree>& entries,
                           boost::property_tree::ptree& entry)
{
    AddMeshToTree(water, entry);
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
    entry.add("Name", mesh.name.c_str());
    entry.add("Bump", mesh.bump);
    entry.add("Intensity", mesh.glow);
    entry.add("Ambience", mesh.ambience);
    entry.add("Specularity", mesh.specularity);
    entry.add("BackfaceCulling", mesh.backfacecull ? 1 : 0);
    entry.add("Shader", m_shaders[mesh.shaderIndex].name);
	
    for (int i = 0; i < Texture::MAX_TYPES; ++i)
    {
        if(mesh.textureIDs[i] != NO_INDEX)
        {
            entry.add(Texture::GetTypeDescription(i), 
                m_textures[mesh.textureIDs[i]].name);
        }
    };
}

void Scene::SaveLightsToFile()
{
    boost::property_tree::ptree root, tree;
    std::vector<boost::property_tree::ptree> entries;
	
    for(const Light& light : m_lights)
    {
        boost::property_tree::ptree entry;
        entry.add("Name", light.name.c_str());
        entry.add("X", light.position.x);
        entry.add("Y", light.position.y);
        entry.add("Z", light.position.z);
        entry.add("R", light.diffuse.r);
        entry.add("G", light.diffuse.g);
        entry.add("B", light.diffuse.b);
        entry.add("SR", light.specular.r);
        entry.add("SG", light.specular.g);
        entry.add("SB", light.specular.b);
        entry.add("AttX", light.attenuation.x);
        entry.add("AttY", light.attenuation.y);
        entry.add("AttZ", light.attenuation.z);
        entry.add("Specularity", light.specularity);
        
        entries.push_back(entry);
        tree.add_child("Light", entries[entries.size()-1]);
    }
    SaveXMLFile(root, tree, "Lights");
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
    tree.add("FogColourR", m_postProcessing.fogColour.r);
    tree.add("FogColourG", m_postProcessing.fogColour.g);
    tree.add("FogColourB", m_postProcessing.fogColour.b);
    tree.add("FogDistance", m_postProcessing.fogDistance);
    tree.add("FogFade", m_postProcessing.fogFade);
    tree.add("GlowAmount", m_postProcessing.glowAmount);
    tree.add("Contrast", m_postProcessing.contrast);
    tree.add("Saturation", m_postProcessing.saturation);
    tree.add("MaximumColourR", m_postProcessing.maximumColour.r);
    tree.add("MaximumColourG", m_postProcessing.maximumColour.g);
    tree.add("MaximumColourB", m_postProcessing.maximumColour.b);
    tree.add("MinimumColourR", m_postProcessing.minimumColour.r);
    tree.add("MinimumColourG", m_postProcessing.minimumColour.g);
    tree.add("MinimumColourB", m_postProcessing.minimumColour.b);
    SaveXMLFile(root, tree, "PostProcessing");
}