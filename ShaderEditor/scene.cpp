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

bool Scene::Initialise()
{
    bool success = true;
    FragmentLinker linker;

    success = success ? InitialiseLighting() : false;
    success = success ? InitialiseMeshes(linker) : false;

    success = success ? linker.GenerateFromFile(SHADER_PATH, "post") : false;
    success = success ? linker.GenerateFromFile(SHADER_PATH, "normal") : false;

    // To prevent unnecessary shader switching, sort by shader used
    std::sort(m_meshes.begin(), m_meshes.end(), [](const Mesh& m1, const Mesh& m2)->bool
    {
        return m1.shaderIndex < m2.shaderIndex;
    });

    return success;
}

bool Scene::InitialiseMeshes(FragmentLinker& linker)
{
    if(!linker.Initialise(m_lights.size()))
    {
        return false;
    }
	
    boost::property_tree::ptree meshes;
    boost::property_tree::xml_parser::read_xml(ASSETS_PATH+"Meshes.xml", 
        meshes, boost::property_tree::xml_parser::trim_whitespace);
    boost::property_tree::ptree& tree = meshes.get_child("Meshes");
	
    m_alpha.reserve(tree.size());
    m_meshes.reserve(tree.size());
	
    boost::property_tree::ptree::iterator it;
    for(it = tree.begin(); it != tree.end(); ++it)
    {
        Mesh mesh;
        mesh.name = it->second.get_child("Name").data();
        mesh.specularity = GetPtreeValue(it, 5.0f, "Specularity");
        mesh.ambience = GetPtreeValue(it, 1.0f, "Ambience");
        mesh.bump = GetPtreeValue(it, 1.0f, "Bump");
        mesh.backfacecull = GetPtreeValue(it, true, "BackfaceCulling");
	
        std::string str;
        mesh.textureIDs[Texture::DIFFUSE] = AddTexture(GetPtreeValue(it, str, "Diffuse"));
        mesh.textureIDs[Texture::SPECULAR] = AddTexture(GetPtreeValue(it, str, "Specular"));
        mesh.textureIDs[Texture::NORMAL] = AddTexture(GetPtreeValue(it, str, "Normal"));
	
        const int unusedTextures = std::count(
            mesh.textureIDs.begin(), mesh.textureIDs.end(), NO_INDEX);
        mesh.maxTextures = mesh.textureIDs.size() - unusedTextures;
	
        // Copy each component featured in the given shader name to a set order
        std::string shadername = it->second.get_child("Shader").data();
        std::string newShaderName;
        std::string component;
        for(int i = 0; i < Shader::MAX_COMPONENTS; ++i)
        {
            component = Shader::GetComponentDescription(i);
            if(boost::algorithm::icontains(shadername, component))
            {
                newShaderName += component;
                boost::algorithm::ireplace_all(shadername, component, "");
            }
        }
	
        // Add any non-component text to the ordered components
        shadername += boost::algorithm::to_lower_copy(newShaderName);
        
        // Determine if shader with those components already exists and reuse if so
        auto shaderItr = std::find_if(m_shaders.begin(), m_shaders.end(), 
            [&](const Shader& shader){ return shader.name == shadername; });
        
        if(shaderItr == m_shaders.end())
        {
            // Shader does not exist, create from fragments
            Shader shader;
            shader.name = shadername;
            if(!linker.GenerateWithFragments(shader))
            {
                Logger::LogError("Shader name " + shadername +
                    " for " + mesh.name + " is an invalid combination");
                return false;
            }
            shader.index = m_shaders.size();
            mesh.shaderIndex = shader.index;
            m_shaders.push_back(shader);
	
            Logger::LogInfo("Shader: " + shader.name + " created");
        }
        else
        {
            mesh.shaderIndex = shaderItr->index;
        }
	
        // Create the mesh. Requires mesh shader to be known before creation.
        std::string errorBuffer;
        const std::string path = ASSETS_PATH + "Meshes//" + mesh.name;
        if(!CreateMesh(path, errorBuffer, mesh))
        {
            Logger::LogError(path + ": " + errorBuffer);
            return false;
        }
	
        // Store the mesh in the correct container
        const std::string alpha = Shader::GetComponentDescription(Shader::ALPHA);
        boost::icontains(shadername, alpha) ? m_alpha.push_back(mesh) : m_meshes.push_back(mesh);
    }

    return true;
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
            //if(usesNormalMapping)
            //{
            //    if(pMesh->HasTangentsAndBitangents())
            //    {
            //        mesh.vertices.push_back(pMesh->mTangents[vert].x);
            //        mesh.vertices.push_back(pMesh->mTangents[vert].y);
            //        mesh.vertices.push_back(pMesh->mTangents[vert].z);
            //        mesh.vertices.push_back(pMesh->mBitangents[vert].x);
            //        mesh.vertices.push_back(pMesh->mBitangents[vert].y);
            //        mesh.vertices.push_back(pMesh->mBitangents[vert].z);
            //        componentCount += 6;
            //    }
            //    else
            //    {
            //        errorBuffer = mesh.name + " requires tangents for requested shader";
            //        return false;
            //    }
            //}
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

const std::vector<Mesh>& Scene::GetAlpha() const
{
    return m_alpha;
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
    return HasTransparency(index) ? 
        m_alpha[index - static_cast<int>(m_meshes.size())] : 
        m_meshes[index];
}

Shader& Scene::GetShader(int index)
{
    return m_shaders[index];
}

int Scene::GetMeshCount() const
{
    return static_cast<int>(m_meshes.size() + m_alpha.size());
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
    for(const Mesh& alpha : m_alpha)
    {
        meshes.push_back(alpha.name);
    }
    return meshes;
}

std::vector<std::string> Scene::GetShaderNames() const
{
    std::vector<std::string> shaders;
    for(const Shader& shader : m_shaders)
    {
        shaders.push_back(shader.name);
    }
    shaders.push_back(POST_NAME);
    shaders.push_back(NORMAL_NAME);
    return shaders;
}

void Scene::WriteToShader(const std::string& name, 
                          const std::string& text, 
                          const std::string& extension)
{
    const std::string filepath = GENERATED_PATH + name + extension;
    std::ofstream file(filepath.c_str(), std::ios_base::out|std::ios_base::trunc);
    
    if(!file.is_open())
    {
        Logger::LogError("Could not open " + filepath);
    }
    else
    {
        file << text << std::endl;
        file.close();
    }
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
        entry.add("Ambience", mesh.ambience);
        entry.add("Specularity", mesh.specularity);
        entry.add("BackfaceCulling", mesh.backfacecull ? 1 : 0);
        entry.add("Specularity", m_shaders[mesh.shaderIndex].name);
	
        auto AddTexture = [&](const std::string& name, Texture::Type type)
        {
            if(mesh.textureIDs[type] != NO_INDEX)
            {
                entry.add(name, m_textures[type].name);
            }
        };
        AddTexture("Diffuse", Texture::DIFFUSE);
        AddTexture("Specular", Texture::SPECULAR);
        AddTexture("Normal", Texture::NORMAL);
	
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
