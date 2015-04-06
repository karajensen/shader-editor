////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - mesh.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "mesh.h"
#include "cache.h"
#include "boost/lexical_cast.hpp"
#include "assimp/include/scene.h"
#include "assimp/include/Importer.hpp"
#include "assimp/include/postprocess.h"

Mesh::Mesh(const boost::property_tree::ptree& node) :
    MeshData(node)
{
    m_initialInstances = GetValueOptional(node, 1, "Instances");
    m_instances.resize(m_initialInstances);

    m_causticsAmount = GetAttributeOptional<float>(node, "Caustics", "amount", 1.0f);
    m_causticsScale = GetAttributeOptional<float>(node, "Caustics", "scale", 1.0f);
    m_specularity = GetValueOptional<float>(node, 0.0f, "Specularity");
    m_ambience = GetValueOptional<float>(node, 1.0f, "Ambience");
    m_bump = GetValueOptional<float>(node, 0.0f, "Bump");
}

void Mesh::Write(boost::property_tree::ptree& node) const
{
    MeshData::Write(node);

    AddValueOptional(node, "Bump", m_bump, 0.0f);
    AddValueOptional(node, "Ambience", m_ambience, 1.0f);
    AddValueOptional(node, "Specularity", m_specularity, 0.0f);
    AddValueOptional(node, "Instances", m_initialInstances, 1);
    AddValueOptional(node, "Caustics.<xmlattr>.amount", m_causticsAmount, 1.0f);
    AddValueOptional(node, "Caustics.<xmlattr>.scale", m_causticsScale, 1.0f);
}

void Mesh::Write(Cache& cache)
{
    cache.Mesh[MESH_CAUSTICS_AMOUNT].SetUpdated(m_causticsAmount);
    cache.Mesh[MESH_CAUSTICS_SCALE].SetUpdated(m_causticsScale);
    cache.Mesh[MESH_BUMP].SetUpdated(m_bump);
    cache.Mesh[MESH_AMBIENCE].SetUpdated(m_ambience);
    cache.Mesh[MESH_SPECULARITY].SetUpdated(m_specularity);
    cache.MeshShader.SetUpdated(ShaderName());
}

void Mesh::Read(Cache& cache)
{
    m_causticsAmount = cache.Mesh[MESH_CAUSTICS_AMOUNT].Get();
    m_causticsScale = cache.Mesh[MESH_CAUSTICS_SCALE].Get();
    m_bump = cache.Mesh[MESH_BUMP].Get();
    m_specularity = cache.Mesh[MESH_SPECULARITY].Get();
    m_ambience = cache.Mesh[MESH_AMBIENCE].Get();
}

const float& Mesh::Bump() const
{
    return m_bump;
}

const float& Mesh::Specularity() const
{
    return m_specularity;
}

const float& Mesh::Ambience() const
{
    return m_ambience;

}

const float& Mesh::CausticsAmount() const
{
    return m_causticsAmount;
}

const float& Mesh::CausticsScale() const
{
    return m_causticsScale;
}

bool Mesh::InitialiseFromFile(const std::string& path, bool requiresNormals, bool requiresTangents)
{
    const int usedTextures = TextureIDs().size() - 
        std::count(TextureIDs().begin(), TextureIDs().end(), NO_INDEX);

    const bool requiresUVs = usedTextures > 0;

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_CalcTangentSpace|
        aiProcess_Triangulate|aiProcess_JoinIdenticalVertices|aiProcess_SortByPType|
        aiProcess_CalcTangentSpace|aiProcess_JoinIdenticalVertices|aiProcess_GenSmoothNormals|
        aiProcess_LimitBoneWeights|aiProcess_RemoveRedundantMaterials|aiProcess_OptimizeMeshes);

    if(!scene)
    {
        Logger::LogError("Assimp import error for mesh " + path + ": " + importer.GetErrorString());
        return false;
    }

    if (requiresTangents && !requiresNormals)
    {
        Logger::LogError(path + " requies normals for bump mapping");
        return false;
    }

    unsigned int numMeshes = scene->mNumMeshes;
    aiMesh** meshes = scene->mMeshes;

    // For each submesh
    bool generatedComponentCount = false;
    for(unsigned int i = 0; i < numMeshes; ++i)
    {
        aiMesh* pMesh = meshes[i];

        const unsigned int indexOffset = m_vertices.size() / m_vertexComponentCount;

        if(!pMesh->HasPositions())
        {
            Logger::LogError(Name() + " requires positions for requested shader");
            return false;
        }
        if(requiresUVs && !pMesh->HasTextureCoords(0))
        {
            Logger::LogError(Name() + " requires uvs for requested shader");
            return false;
        }
        if(requiresNormals && !pMesh->HasNormals())
        {
            Logger::LogError(Name() + " requires normals for requested shader");
            return false;
        }

        // For each vertex
        int componentCount = 0;
        for(unsigned int vert = 0; vert < pMesh->mNumVertices; ++vert)
        {
            m_vertices.push_back(pMesh->mVertices[vert].x);
            m_vertices.push_back(pMesh->mVertices[vert].y);
            m_vertices.push_back(pMesh->mVertices[vert].z);
            componentCount = 3;

            if (requiresUVs)
            {
                m_vertices.push_back(pMesh->mTextureCoords[0][vert].x);
                m_vertices.push_back(pMesh->mTextureCoords[0][vert].y);
                componentCount += 2;
            }

            if (requiresNormals)
            {
                m_vertices.push_back(pMesh->mNormals[vert].x);
                m_vertices.push_back(pMesh->mNormals[vert].y);
                m_vertices.push_back(pMesh->mNormals[vert].z);
                componentCount += 3;
            }

            // Add any bitangents/tangents for the mesh
            if(requiresTangents)
            {
                if(pMesh->HasTangentsAndBitangents())
                {
                    m_vertices.push_back(pMesh->mTangents[vert].x);
                    m_vertices.push_back(pMesh->mTangents[vert].y);
                    m_vertices.push_back(pMesh->mTangents[vert].z);
                    m_vertices.push_back(pMesh->mBitangents[vert].x);
                    m_vertices.push_back(pMesh->mBitangents[vert].y);
                    m_vertices.push_back(pMesh->mBitangents[vert].z);
                    componentCount += 6;
                }
                else
                {
                    Logger::LogError(Name() + " requires tangents for requested shader");
                    return false;
                }
            }
        }

        // Make sure vertex layout is consistant between submeshes
        if(generatedComponentCount)
        {
            if(componentCount != m_vertexComponentCount)
            {
                Logger::LogError("Assimp error for mesh " + path + ": " + 
                    boost::lexical_cast<std::string>(componentCount) + " does not match " +
                    boost::lexical_cast<std::string>(m_vertexComponentCount));
                return false;
            }
        }
        else
        {
            m_vertexComponentCount = componentCount;
            generatedComponentCount = true;
        }

        // For each face
        for(unsigned int face = 0; face < pMesh->mNumFaces; ++face)
        {
            aiFace *pFace = &pMesh->mFaces[face];
            if(pFace->mNumIndices != 3)
            {
                Logger::LogError("Assimp error for mesh " + path + ": not all faces are triangles");
                return false;
            }

            m_indices.push_back(indexOffset + pFace->mIndices[0]);
            m_indices.push_back(indexOffset + pFace->mIndices[1]);
            m_indices.push_back(indexOffset + pFace->mIndices[2]);
        }
    }

    Logger::LogInfo("Mesh: " + Name() + " created");
    return true;
}

