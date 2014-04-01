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
#include "AntTweakBar.h"
#include "fragmentlinker.h"

namespace
{
    /**
    * Anttweakbar button callback for selecting the next light
    * @param clientData The user given data
    */
    void TW_CALL ButtonSelectNextLight(void *clientData)
    {
        (static_cast<Scene*>(clientData))->SelectNextLight();
    }
}

Scene::Scene() :
    m_tweakbar(nullptr),
    m_selectedLight(NO_INDEX),
    m_linker(new FragmentLinker())
{
}

Scene::~Scene()
{
}

void Scene::Update()
{
}

bool Scene::Initialise()
{
    bool success = true;
    success = success ? InitialiseLighting() : false;
    success = success ? InitialiseMeshes() : false;

    // To prevent unnecessary shader switching, sort by shader used
    std::sort(m_meshes.begin(), m_meshes.end(), [](const Mesh& m1, const Mesh& m2)->bool
    {
        return m1.shaderIndex < m2.shaderIndex;
    });

    return success;
}

bool Scene::InitialiseMeshes()
{
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
        mesh.specularity = GetPtreeValue(it,5.0f,"Specularity");
        mesh.backfacecull = GetPtreeValue(it,true,"BackfaceCulling");

        std::string errorBuffer;
        const std::string path = ASSETS_PATH + "Meshes//" + mesh.name;
        if(!CreateMesh(path, errorBuffer, mesh))
        {
            Logger::LogError(path + ": " + errorBuffer);
            return false;
        }

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
            if(!m_linker->InitialiseFromFragments(shader, m_lights.size()))
            {
                Logger::LogError("Shader name " + shadername +
                    " for " + mesh.name + " is an invalid combination");
                return false;
            }
            shader.index = m_shaders.size();
            mesh.shaderIndex = shader.index;
            m_shaders.push_back(shader);
        }
        else
        {
            mesh.shaderIndex = shaderItr->index;
        }

        // Store the mesh in the correct container
        const std::string alpha = Shader::GetComponentDescription(Shader::ALPHA);
        boost::icontains(shadername, alpha) ? m_alpha.push_back(mesh) : m_meshes.push_back(mesh);
    }

    Logger::LogInfo("Mesh: All meshes created successfully");
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
                light.castshadow = GetPtreeValue(it,false,"Shadows");
                light.attenuation.x = GetPtreeValue(it,0.0f,"AttX");
                light.attenuation.y = GetPtreeValue(it,0.0f,"AttY");
                light.attenuation.z = GetPtreeValue(it,0.0f,"AttZ");

                m_selectedLight = m_lights.size();
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

    unsigned int numMeshes = scene->mNumMeshes;
    aiMesh** meshes = scene->mMeshes;

    for(unsigned int i = 0; i < numMeshes; ++i)
    {
        aiMesh* pMesh = meshes[i];
        unsigned int indexOffset = mesh.position.size();
        unsigned int numVerts = pMesh->mNumVertices;
        unsigned int numFaces = pMesh->mNumFaces;

        // For each vertex
        for(unsigned int vert = 0; vert < numVerts; ++vert)
        {
            mesh.position.push_back(Float3(pMesh->mVertices[vert].x,
                pMesh->mVertices[vert].y, pMesh->mVertices[vert].z));

            mesh.normals.push_back(Float3(pMesh->mNormals[vert].x,
                pMesh->mNormals[vert].y, pMesh->mNormals[vert].z));

            mesh.uvs.push_back(Float2(pMesh->mTextureCoords[0][vert].x,
                pMesh->mTextureCoords[0][vert].y));
        }

        // For each face
        for(unsigned int face = 0; face < numFaces; ++face)
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
    Logger::LogInfo("Mesh: " + path + " created");
    return true;
}

void Scene::SelectNextLight()
{
    ++m_selectedLight;
    if(m_selectedLight >= static_cast<int>(m_lights.size()))
    {
        m_selectedLight = 0;
    }
    ReleaseTweakParameters();
    InitialiseTweakParameters();
}

void Scene::ReleaseTweakParameters()
{
    TwRemoveVar(m_tweakbar, "Select Next Light");
    TwRemoveVar(m_tweakbar, "Position X");
    TwRemoveVar(m_tweakbar, "Position Y");
    TwRemoveVar(m_tweakbar, "Position Z");
}

void Scene::InitialiseTweakParameters()
{
    const std::string light = m_selectedLight == NO_INDEX ? "None" : m_lights[m_selectedLight].name;
    const std::string lightGroup = " group='Light: " + light + "' ";
    TwAddButton(m_tweakbar, "Select Next Light", ButtonSelectNextLight, this, lightGroup.c_str());
 
    if(m_selectedLight != NO_INDEX)
    {
        TwAddVarRW(m_tweakbar, "Position X", TW_TYPE_FLOAT, 
            &m_lights[m_selectedLight].position.x, lightGroup.c_str());

        TwAddVarRW(m_tweakbar, "Position Y", TW_TYPE_FLOAT, 
            &m_lights[m_selectedLight].position.y, lightGroup.c_str());

        TwAddVarRW(m_tweakbar, "Position Z", TW_TYPE_FLOAT, 
            &m_lights[m_selectedLight].position.z, lightGroup.c_str());
    }
}

void Scene::InitialiseTweakBar(CTwBar* tweakbar)
{
    m_tweakbar = tweakbar;
    InitialiseTweakParameters();
}

const std::vector<Mesh> Scene::GetMeshes() const
{
    return m_meshes;
}

const std::vector<Mesh> Scene::GetAlpha() const
{
    return m_alpha;
}

const std::vector<Shader> Scene::GetShaders() const
{
    return m_shaders;
}

const std::vector<Light> Scene::GetLights() const
{
    return m_lights;
}