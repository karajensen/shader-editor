////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - mesh.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "mesh.h"
#include "common.h"
#include "renderdata.h"
#include "boost/lexical_cast.hpp"
#include "assimp/include/scene.h"
#include "assimp/include/Importer.hpp"
#include "assimp/include/postprocess.h"

Mesh::Mesh()
{
    textureIDs.resize(Texture::MAX_TYPES);
    textureIDs.assign(Texture::MAX_TYPES, NO_INDEX);
}

bool Mesh::Initialise(bool requiresNormals, bool requiresBumpMapping)
{
    const std::string path = MESH_PATH + name;

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

    if (requiresBumpMapping && !requiresNormals)
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

        unsigned int indexOffset = vertices.size() / vertexComponentCount;
        vertexCount += pMesh->mNumVertices;
        faceCount += pMesh->mNumFaces;

        if(!pMesh->HasPositions())
        {
            Logger::LogError(name + " requires positions for requested shader");
            return false;
        }
        if(!pMesh->HasTextureCoords(0))
        {
            Logger::LogError(name + " requires uvs for requested shader");
            return false;
        }
        if(requiresNormals && !pMesh->HasNormals())
        {
            Logger::LogError(name + " requires normals for requested shader");
            return false;
        }

        // For each vertex
        int componentCount = 0;
        for(unsigned int vert = 0; vert < pMesh->mNumVertices; ++vert)
        {
            vertices.push_back(pMesh->mVertices[vert].x);
            vertices.push_back(pMesh->mVertices[vert].y);
            vertices.push_back(pMesh->mVertices[vert].z);
            vertices.push_back(pMesh->mTextureCoords[0][vert].x);
            vertices.push_back(pMesh->mTextureCoords[0][vert].y);
            componentCount = 5;

            if (requiresNormals)
            {
                vertices.push_back(pMesh->mNormals[vert].x);
                vertices.push_back(pMesh->mNormals[vert].y);
                vertices.push_back(pMesh->mNormals[vert].z);
                componentCount = 8;
            }

            // Add any bitangents/tangents for the mesh
            if(requiresBumpMapping)
            {
                if(pMesh->HasTangentsAndBitangents())
                {
                    vertices.push_back(pMesh->mTangents[vert].x);
                    vertices.push_back(pMesh->mTangents[vert].y);
                    vertices.push_back(pMesh->mTangents[vert].z);
                    vertices.push_back(pMesh->mBitangents[vert].x);
                    vertices.push_back(pMesh->mBitangents[vert].y);
                    vertices.push_back(pMesh->mBitangents[vert].z);
                    componentCount += 6;
                }
                else
                {
                    Logger::LogError(name + " requires tangents for requested shader");
                    return false;
                }
            }
        }

        // Make sure vertex layout is consistant between submeshes
        if(generatedComponentCount)
        {
            if(componentCount != vertexComponentCount)
            {
                Logger::LogError("Assimp error for mesh " + path + ": " + 
                    boost::lexical_cast<std::string>(componentCount) + " does not match " +
                    boost::lexical_cast<std::string>(vertexComponentCount));
                return false;
            }
        }
        else
        {
            vertexComponentCount = componentCount;
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

            indices.push_back(indexOffset + pFace->mIndices[0]);
            indices.push_back(indexOffset + pFace->mIndices[1]);
            indices.push_back(indexOffset + pFace->mIndices[2]);
        }
    }

    indexCount = indices.size();
    Logger::LogInfo("Mesh: " + path + " created");
    return true;
}