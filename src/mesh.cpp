////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - mesh.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "mesh.h"
#include "cache.h"
#include "logger.h"

#include <boost/lexical_cast.hpp>

#include "assimp/include/scene.h"
#include "assimp/include/Importer.hpp"
#include "assimp/include/postprocess.h"

Mesh::Mesh(const std::string& name, 
           const std::string& shaderName, 
           int shaderID)
    : MeshData(name, shaderName, shaderID)
{
}

void Mesh::Write(Cache& cache)
{
    cache.Mesh[Tweakable::Mesh::CausticsAmount].SetUpdated(m_causticsAmount);
    cache.Mesh[Tweakable::Mesh::CausticsScale].SetUpdated(m_causticsScale);
    cache.Mesh[Tweakable::Mesh::Bump].SetUpdated(m_bump);
    cache.Mesh[Tweakable::Mesh::Ambience].SetUpdated(m_ambience);
    cache.Mesh[Tweakable::Mesh::Specularity].SetUpdated(m_specularity);
    cache.MeshShader.SetUpdated(ShaderName());
}

void Mesh::Read(Cache& cache)
{
    m_causticsAmount = cache.Mesh[Tweakable::Mesh::CausticsAmount].Get();
    m_causticsScale = cache.Mesh[Tweakable::Mesh::CausticsScale].Get();
    m_bump = cache.Mesh[Tweakable::Mesh::Bump].Get();
    m_specularity = cache.Mesh[Tweakable::Mesh::Specularity].Get();
    m_ambience = cache.Mesh[Tweakable::Mesh::Ambience].Get();
}

bool Mesh::InitialiseFromFile(const std::string& path, 
                              const Float2& uvScale, 
                              bool requiresNormals, 
                              bool requiresTangents)
{
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

    bool generatedComponentCount = false;
    m_vertexComponentCount = 1;

    // For each submesh
    for(unsigned int i = 0; i < numMeshes; ++i)
    {
        aiMesh* pMesh = meshes[i];

        const unsigned int indexOffset = m_vertices.size() / m_vertexComponentCount;

        if(!pMesh->HasPositions())
        {
            Logger::LogError(Name() + " requires positions for requested shader");
            return false;
        }
        if(!pMesh->HasTextureCoords(0))
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
            m_vertices.push_back(pMesh->mTextureCoords[0][vert].x * uvScale.x);
            m_vertices.push_back(pMesh->mTextureCoords[0][vert].y * uvScale.y);
            componentCount = 5;

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

    InitialiseMeshData();
    Logger::LogInfo("Mesh: " + Name() + " created");
    return true;
}
