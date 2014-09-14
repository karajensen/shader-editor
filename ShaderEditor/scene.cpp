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
    if (!InitialiseLighting())
    {
        return false;
    }

    FragmentLinker linker;
    if(!linker.Initialise(m_lights.size()))
    {
        return false;
    }

    if (!InitialiseShaders(linker) || !InitialiseMeshes(linker))
    {
        return false;
    }

    // To prevent unnecessary shader switching, sort by shader used
    std::sort(m_meshes.begin(), m_meshes.end(), [](const Mesh& m1, const Mesh& m2)->bool
    {
        return m1.shaderIndex < m2.shaderIndex;
    });

    return true;
}

bool Scene::InitialiseShaders(FragmentLinker& linker)
{
    const int postIndex = m_shaders.size();
    assert(POST_SHADER_INDEX == postIndex);
    Shader postShader;
    postShader.index = postIndex;
    postShader.name = POST_SHADER;

    if (!linker.GenerateFromFile(postShader))
    {
        Logger::LogError("Could not generate post shader");
        return false;
    }

    m_shaders.push_back(postShader);
    return true;
}

bool Scene::InitialiseMeshes(FragmentLinker& linker)
{	
    boost::property_tree::ptree meshes;
    boost::property_tree::xml_parser::read_xml(ASSETS_PATH+"Meshes.xml", 
        meshes, boost::property_tree::xml_parser::trim_whitespace);
    boost::property_tree::ptree& tree = meshes.get_child("Meshes");
	
    m_meshes.reserve(tree.size());
	
    boost::property_tree::ptree::iterator it;
    for(it = tree.begin(); it != tree.end(); ++it)
    {
        Mesh mesh;
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
	
        // Get the shader used by the mesh
        std::string shadername = OVERRIDE_SHADERS ? GLOBAL_SHADER :
            it->second.get_child("Shader").data();

        // Copy each component featured in the given shader name to a set order
        std::string newShaderName;
        std::string component;
        for(int i = 0; i < Shader::MAX_COMPONENTS; ++i)
        {
            component = Shader::ComponentAsString(i);
            if(boost::algorithm::icontains(shadername, component))
            {
                newShaderName += component;
                boost::algorithm::ireplace_all(shadername, component, "");
            }
        }
	
        // Add any non-component text to the ordered components
        shadername += boost::algorithm::to_lower_copy(newShaderName);
        mesh.shaderIndex = GetShaderIndex(linker, shadername, mesh.name);
        mesh.normalIndex = GetShaderIndex(linker, NORMAL_SHADER + shadername, mesh.name);
	
        // Create the mesh. Requires mesh shader to be known before creation.
        std::string errorBuffer;
        const std::string path = ASSETS_PATH + "Meshes//" + mesh.name;
        if(!CreateMesh(path, errorBuffer, mesh))
        {
            Logger::LogError(path + ": " + errorBuffer);
            return false;
        }
	
        m_meshes.push_back(mesh);
    }

    return true;
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


const std::vector<Mesh>& Scene::GetMeshes() const
{
    return m_meshes;
}

const std::vector<Shader>& Scene::GetShaders() const
{
    return m_shaders;
}

const std::vector<Light>& Scene::GetLights() const
{
    return m_lights;
}

const std::vector<Texture>& Scene::GetTextures() const
{
    return m_textures;
}

Light& Scene::GetLight(int index)
{
    return m_lights[index];
}

std::string Scene::GetTexture(int index)
{
    return index == NO_INDEX ? "None" : m_textures[index].name;
}

bool Scene::HasTransparency(int index)
{
    return index >= static_cast<int>(m_meshes.size());
}

Mesh& Scene::GetMesh(int index)
{
    return m_meshes[index];
}

Shader& Scene::GetShader(int index)
{
    return m_shaders[index];
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

std::vector<std::string> Scene::GetPostTextureNames() const
{
    std::vector<std::string> textures;
    for (int i = 0; i < PostProcessing::MAX_MAPS; ++i)
    {
        textures.push_back(PostProcessing::GetMapName(
            static_cast<PostProcessing::Map>(i)));
    }
    return textures;
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
                entry.add(Texture::GetTypeDescription(i), m_textures[i].name);
            }
        };
	
        entries.emplace_back(entry);
        tree.add_child("Mesh", entries[entries.size()-1]);
    }
    root.add_child("Meshes", tree);
	
    // Writing property tree to xml
    filesystem::path filePath(ASSETS_PATH + "Meshes_saved.xml");
    property_tree::xml_parser::xml_writer_settings<char> settings('\t', 1);
    property_tree::write_xml(filePath.generic_string(), root, std::locale(), settings);
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
