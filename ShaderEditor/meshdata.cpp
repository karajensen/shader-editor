////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - meshdata.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "meshdata.h"
#include "common.h"
#include "cache.h"
#include "renderdata.h"
#include "boost/lexical_cast.hpp"
#include "assimp/include/scene.h"
#include "assimp/include/Importer.hpp"
#include "assimp/include/postprocess.h"

MeshData::MeshData(const boost::property_tree::ptree& node)
{
    m_textureIDs.resize(Texture::MAX_TYPES);
    m_textureIDs.assign(Texture::MAX_TYPES, NO_INDEX);
    m_name = GetValue<std::string>(node, "Name");

    for (int i = 0; i < Texture::MAX_TYPES; ++i)
    {
        m_textureNames.push_back(GetValueOptional(
            node, std::string(), Texture::GetTypeDescription(i).c_str()));
    }
}

void MeshData::Write(boost::property_tree::ptree& node) const
{
    node.add("Shader", m_shaderName);
    node.add("Name", m_name);

    for (int i = 0; i < Texture::MAX_TYPES; ++i)
    {
        if (!Texture::IsAnimatedType(i) && m_textureIDs[i] != NO_INDEX)
        {
            node.add(Texture::GetTypeDescription(i), m_textureNames[i]);
        }
    }
}

bool MeshData::Initialise(const std::string& path, bool requiresNormals, bool requiresTangents)
{
    const int maxTextures = m_textureIDs.size() - 
        std::count(m_textureIDs.begin(), m_textureIDs.end(), NO_INDEX);

    const bool requiresUVs = maxTextures > 0;

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
            Logger::LogError(m_name + " requires positions for requested shader");
            return false;
        }
        if(requiresUVs && !pMesh->HasTextureCoords(0))
        {
            Logger::LogError(m_name + " requires uvs for requested shader");
            return false;
        }
        if(requiresNormals && !pMesh->HasNormals())
        {
            Logger::LogError(m_name + " requires normals for requested shader");
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
                    Logger::LogError(m_name + " requires tangents for requested shader");
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

    Logger::LogInfo("Mesh: " + m_name + " created");
    return true;
}

const std::string& MeshData::Name() const
{
    return m_name;
}

int MeshData::ShaderID() const
{
    return m_shaderIndex;
}

const std::vector<float>& MeshData::Vertices() const
{
    return m_vertices;
}

const std::vector<unsigned long>& MeshData::Indices() const
{
    return m_indices;
}

const std::vector<int>& MeshData::TextureIDs() const
{
    return m_textureIDs;
}

const std::vector<std::string>& MeshData::TextureNames() const
{
    return m_textureNames;
}

int MeshData::VertexComponentCount() const
{
    return m_vertexComponentCount;
}

void MeshData::SetShaderID(const std::string& shaderName, int shaderID)
{
    m_shaderName = shaderName;
    m_shaderIndex = shaderID;
}

void MeshData::SetTexture(Texture::Type type, int ID)
{
    m_textureIDs[type] = ID;
}

const std::string& MeshData::ShaderName() const
{
    return m_shaderName;
}