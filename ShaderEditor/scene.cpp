////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - scene.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "scene.h"
#include <algorithm>
#include "boost/filesystem.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/property_tree/xml_parser.hpp"
#include "boost/algorithm/string.hpp"
#include "assimp/include/scene.h"
#include "assimp/include/Importer.hpp"
#include "assimp/include/postprocess.h"
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
        InitialiseMeshes(linker))
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

    auto CreateShader = [&](const std::string& name, int index)
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
            m_shaders.emplace_back(shader);
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

bool Scene::InitialiseMeshes(FragmentLinker& linker)
{	
    boost::property_tree::ptree meshes;
    boost::property_tree::xml_parser::read_xml(ASSETS_PATH+"Meshes.xml", 
        meshes, boost::property_tree::xml_parser::trim_whitespace);
    boost::property_tree::ptree& tree = meshes.get_child("Meshes");

    boost::property_tree::ptree::iterator it;
    for(it = tree.begin(); it != tree.end(); ++it)
    {
        if (it->first == "Water")
        {
            Water water;
            InitialiseMesh(water, it);
            InitialiseWater(water, it);
            CreateMesh(water);
            m_water.emplace_back(water);
        }
        else if (it->first == "Mesh")
        {
            Mesh mesh;
            InitialiseMesh(mesh, it);
            InitialiseMeshShader(mesh, linker, it);
            CreateMesh(mesh);
            m_meshes.emplace_back(mesh);
        }
    }

    return true;
}

void Scene::InitialiseWater(Water& water, boost::property_tree::ptree::iterator& it)
{
    water.shaderIndex = WATER_SHADER_INDEX;
    water.normalIndex = WATER_NORMAL_SHADER_INDEX;
}

void Scene::InitialiseMesh(Mesh& mesh, boost::property_tree::ptree::iterator& it)
{
    mesh.name = it->second.get_child("Name").data();
    mesh.specularity = GetPtreeValue(it, 5.0f, "Specularity");
    mesh.ambience = GetPtreeValue(it, 1.0f, "Ambience");
    mesh.bump = GetPtreeValue(it, 1.0f, "Bump");
    mesh.glow = GetPtreeValue(it, 1.0f, "Intensity");
    mesh.backfacecull = GetPtreeValue(it, true, "BackfaceCulling");
	
    // Get the textures used by the mesh
    assert(static_cast<int>(mesh.textureIDs.size()) == Texture::MAX_TYPES);
    for (int i = 0; i < Texture::MAX_TYPES; ++i)
    {
        mesh.textureIDs[i] = AddTexture(GetPtreeValue(
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
        it->second.get_child("Shader").data();

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
        [&](const Shader& shader){ return shader.name == name; });
        
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

bool Scene::CreateMesh(Mesh& mesh)
{
    std::string errorBuffer;
    const std::string path = ASSETS_PATH + "Meshes//" + mesh.name;
    if (!CreateMesh(path, errorBuffer, mesh))
    {
        Logger::LogError(path + ": " + errorBuffer);
        return false;
    }
    return true;
}

bool Scene::CreateMesh(const std::string& path, std::string& errorBuffer, Mesh& mesh)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_CalcTangentSpace|
        aiProcess_Triangulate|aiProcess_JoinIdenticalVertices|aiProcess_SortByPType|
        aiProcess_CalcTangentSpace|aiProcess_JoinIdenticalVertices|aiProcess_GenSmoothNormals|
        aiProcess_LimitBoneWeights|aiProcess_RemoveRedundantMaterials|aiProcess_OptimizeMeshes);

    if(!scene)
    {
        errorBuffer = "Assimp import error for mesh " + path + ": " + importer.GetErrorString();
        return false;
    }

    const Shader& shader = m_shaders[mesh.shaderIndex];
    const bool usesNormalMapping = shader.HasComponent(Shader::BUMP);

    unsigned int numMeshes = scene->mNumMeshes;
    aiMesh** meshes = scene->mMeshes;

    // For each submesh
    bool generatedComponentCount = false;
    for(unsigned int i = 0; i < numMeshes; ++i)
    {
        aiMesh* pMesh = meshes[i];

        unsigned int indexOffset = mesh.vertices.size() / mesh.vertexComponentCount;
        mesh.vertexCount += pMesh->mNumVertices;
        mesh.faceCount += pMesh->mNumFaces;

        if(!pMesh->HasPositions())
        {
            errorBuffer = mesh.name + " requires positions for requested shader";
            return false;
        }
        if(!pMesh->HasTextureCoords(0))
        {
            errorBuffer = mesh.name + " requires uvs for requested shader";
            return false;
        }
        if(!pMesh->HasNormals())
        {
            errorBuffer = mesh.name + " requires normals for requested shader";
            return false;
        }

        // For each vertex
        int componentCount = 0;
        for(unsigned int vert = 0; vert < pMesh->mNumVertices; ++vert)
        {
            mesh.vertices.push_back(pMesh->mVertices[vert].x);
            mesh.vertices.push_back(pMesh->mVertices[vert].y);
            mesh.vertices.push_back(pMesh->mVertices[vert].z);
            mesh.vertices.push_back(pMesh->mTextureCoords[0][vert].x);
            mesh.vertices.push_back(pMesh->mTextureCoords[0][vert].y);
            mesh.vertices.push_back(pMesh->mNormals[vert].x);
            mesh.vertices.push_back(pMesh->mNormals[vert].y);
            mesh.vertices.push_back(pMesh->mNormals[vert].z);
            componentCount = 8;

            // Add any bitangents/tangents for the mesh
            if(usesNormalMapping)
            {
                if(pMesh->HasTangentsAndBitangents())
                {
                    mesh.vertices.push_back(pMesh->mTangents[vert].x);
                    mesh.vertices.push_back(pMesh->mTangents[vert].y);
                    mesh.vertices.push_back(pMesh->mTangents[vert].z);
                    mesh.vertices.push_back(pMesh->mBitangents[vert].x);
                    mesh.vertices.push_back(pMesh->mBitangents[vert].y);
                    mesh.vertices.push_back(pMesh->mBitangents[vert].z);
                    componentCount += 6;
                }
                else
                {
                    errorBuffer = mesh.name + " requires tangents for requested shader";
                    return false;
                }
            }
        }

        // Make sure vertex layout is consistant between submeshes
        if(generatedComponentCount)
        {
            if(componentCount != mesh.vertexComponentCount)
            {
                errorBuffer = "Assimp error for mesh " + path + ": " + 
                    boost::lexical_cast<std::string>(componentCount) + " does not match " +
                    boost::lexical_cast<std::string>(mesh.vertexComponentCount);
                return false;
            }
        }
        else
        {
            mesh.vertexComponentCount = componentCount;
            generatedComponentCount = true;
        }

        // For each face
        for(unsigned int face = 0; face < pMesh->mNumFaces; ++face)
        {
            aiFace *pFace = &pMesh->mFaces[face];
            if(pFace->mNumIndices != 3)
            {
                errorBuffer = "Assimp error for mesh " + path + ": not all faces are triangles";
                return false;
            }

            mesh.indices.push_back(indexOffset + pFace->mIndices[0]);
            mesh.indices.push_back(indexOffset + pFace->mIndices[1]);
            mesh.indices.push_back(indexOffset + pFace->mIndices[2]);
        }
    }

    mesh.indexCount = mesh.indices.size();
    Logger::LogInfo("Mesh: " + path + " created");
    return true;
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

std::vector<std::string> Scene::GetMeshNames() const
{
    std::vector<std::string> meshes;
    for(const Mesh& mesh : m_meshes)
    {
        meshes.push_back(mesh.name);
    }
    return meshes;
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

void Scene::SaveMeshesToFile()
{
    using namespace boost;
    property_tree::ptree root, tree;
    std::vector<property_tree::ptree> entries;
	
    for(const Mesh& mesh : m_meshes)
    {
        property_tree::ptree entry;
        AddMeshToTree(mesh, entry);
        entries.emplace_back(entry);
        tree.add_child("Mesh", entries[entries.size()-1]);
    }

    for(const Water& water : m_water)
    {
        property_tree::ptree entry;
        AddMeshToTree(water, entry);
        entries.emplace_back(entry);
        tree.add_child("Water", entries[entries.size()-1]);
    }

    root.add_child("Meshes", tree);
    filesystem::path filePath(ASSETS_PATH + "Meshes_saved.xml");
    property_tree::xml_parser::xml_writer_settings<char> settings('\t', 1);
    property_tree::write_xml(filePath.generic_string(), root, std::locale(), settings);
}

void Scene::AddMeshToTree(const Mesh& mesh, boost::property_tree::ptree& entry)
{
    entry.add("Name", mesh.name.c_str());
    entry.add("Bump", mesh.bump);
    entry.add("Intensity", mesh.glow);
    entry.add("Ambience", mesh.ambience);
    entry.add("Specularity", mesh.specularity);
    entry.add("BackfaceCulling", mesh.backfacecull ? 1 : 0);
    entry.add("Specularity", m_shaders[mesh.shaderIndex].name);
	
    for (int i = 0; i < Texture::MAX_TYPES; ++i)
    {
        if(mesh.textureIDs[i] != NO_INDEX)
        {
            entry.add(Texture::GetTypeDescription(i), 
                m_textures[mesh.textureIDs[i]].name);
        }
    };
}

bool Scene::InitialiseLighting()
{
    using namespace boost;
    filesystem::path filePath(ASSETS_PATH + "Lights.xml");
    if(filesystem::exists(filePath))
    {
        try
        {
            property_tree::ptree root;
            property_tree::xml_parser::read_xml(filePath.generic_string(), 
                root, property_tree::xml_parser::trim_whitespace);
            property_tree::ptree& tree = root.get_child("Lights");
	
            boost::property_tree::ptree::iterator it;
            for(it = tree.begin(); it != tree.end(); ++it)
            {
                Light light;
                light.name = GetPtreeValue(it, std::string("UNNAMED"), "Name");
	
                light.diffuse.a = 1.0f;
                light.diffuse.r = GetPtreeValue(it,1.0f,"R");
                light.diffuse.b = GetPtreeValue(it,1.0f,"B");
                light.diffuse.g = GetPtreeValue(it,1.0f,"G");
	
                light.specular.a = 1.0f;
                light.specular.r = GetPtreeValue(it,1.0f,"SR");
                light.specular.b = GetPtreeValue(it,1.0f,"SB");
                light.specular.g = GetPtreeValue(it,1.0f,"SG");
	
                light.position.x = GetPtreeValue(it,0.0f,"X");
                light.position.y = GetPtreeValue(it,0.0f,"Y");
                light.position.z = GetPtreeValue(it,0.0f,"Z");
	
                light.specularity = GetPtreeValue(it,5.0f,"Specularity");
                light.attenuation.x = GetPtreeValue(it,0.0f,"AttX");
                light.attenuation.y = GetPtreeValue(it,0.0f,"AttY");
                light.attenuation.z = GetPtreeValue(it,0.0f,"AttZ");
	
                m_lights.push_back(light);
            }
        }
        catch(const filesystem::filesystem_error& e)
        {
            Logger::LogError(std::string("Lighting: ") + e.what());
            return false;
        }
    }
    else
    {
        Logger::LogError("Lighting: Could not find file");
        return false;
    }    
	
    Logger::LogInfo("Lighting: Initialised successfully");
    return true;
}

void Scene::SaveLightsToFile()
{
    using namespace boost;
    property_tree::ptree root, tree;
    std::vector<property_tree::ptree> entries;
	
    for(const Light& light : m_lights)
    {
        property_tree::ptree entry;
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
        
        entries.emplace_back(entry);
        tree.add_child("Light", entries[entries.size()-1]);
    }
    root.add_child("Lights", tree);
	
    // Writing property tree to xml
    filesystem::path filePath(ASSETS_PATH + "Lights_saved.xml");
    property_tree::xml_parser::xml_writer_settings<char> settings('\t', 1);
    property_tree::write_xml(filePath.generic_string(), root, std::locale(), settings);
}

bool Scene::InitialisePost()
{
    boost::property_tree::ptree post;
    boost::property_tree::xml_parser::read_xml(ASSETS_PATH+"Post.xml", 
        post, boost::property_tree::xml_parser::trim_whitespace);
    boost::property_tree::ptree& tree = post.get_child("PostProcessing");

    auto getValue = [&](const std::string& name)
    {
        return boost::lexical_cast<float>(tree.get_child(name).data());
    };

    m_postProcessing.blurAmount = getValue("BlurAmount");
    m_postProcessing.blurStep = getValue("BlurStep");
    m_postProcessing.depthFar = getValue("DepthFar");
    m_postProcessing.depthNear = getValue("DepthNear");
    m_postProcessing.dofDistance = getValue("DOFDistance");
    m_postProcessing.dofFade = getValue("DOFFade");
    m_postProcessing.fogColour.r = getValue("FogColourR");
    m_postProcessing.fogColour.g = getValue("FogColourG");
    m_postProcessing.fogColour.b = getValue("FogColourB");
    m_postProcessing.fogDistance = getValue("FogDistance");
    m_postProcessing.fogFade = getValue("FogFade");
    m_postProcessing.glowAmount = getValue("GlowAmount");
    m_postProcessing.contrast = getValue("Contrast");
    m_postProcessing.saturation = getValue("Saturation");
    m_postProcessing.maximumColour.r = getValue("MaximumColourR");
    m_postProcessing.maximumColour.g = getValue("MaximumColourG");
    m_postProcessing.maximumColour.b = getValue("MaximumColourB");
    m_postProcessing.minimumColour.r = getValue("MinimumColourR");
    m_postProcessing.minimumColour.g = getValue("MinimumColourG");
    m_postProcessing.minimumColour.b = getValue("MinimumColourB");

    return true;
}

void Scene::SavePostProcessingtoFile()
{
    using namespace boost;
    property_tree::ptree root, tree;

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
    root.add_child("PostProcessing", tree);

    filesystem::path filePath(ASSETS_PATH + "Post_saved.xml");
    property_tree::xml_parser::xml_writer_settings<char> settings('\t', 1);
    property_tree::write_xml(filePath.generic_string(), root, std::locale(), settings);
}