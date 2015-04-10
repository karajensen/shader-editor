////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - meshdata.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "meshdata.h"
#include "common.h"
#include "cache.h"
#include "renderdata.h"

MeshData::MeshData(const boost::property_tree::ptree& node)
{
    m_textureIDs.resize(Texture::MAX_TYPES);
    m_textureIDs.assign(Texture::MAX_TYPES, NO_INDEX);
    m_name = GetValue<std::string>(node, "Name");
    m_backfacecull = GetValueOptional<bool>(node, true, "BackfaceCulling");
    m_shaderName = GetValueOptional<std::string>(node, "", "Shader");

    for (int i = 0; i < Texture::MAX_TYPES; ++i)
    {
        m_textureNames.push_back(GetValueOptional(
            node, std::string(), Texture::GetTypeDescription(i).c_str()));
    }
}

void MeshData::Write(boost::property_tree::ptree& node) const
{
    node.add("Name", m_name);
    AddValueOptional(node, "Shader", m_shaderName, std::string());
    AddValueOptional(node, "BackfaceCulling", m_backfacecull ? 1 : 0, 1);

    for (int i = 0; i < Texture::MAX_TYPES; ++i)
    {
        if (!Texture::IsAnimatedType(i) && m_textureIDs[i] != NO_INDEX)
        {
            node.add(Texture::GetTypeDescription(i), m_textureNames[i]);
        }
    }
}

const std::string& MeshData::Name() const
{
    return m_name;
}

int MeshData::ShaderID() const
{
    return m_shaderIndex;
}

bool MeshData::BackfaceCull() const
{
    return m_backfacecull;
}

const std::vector<float>& MeshData::Vertices() const
{
    return m_vertices;
}

const std::vector<unsigned int>& MeshData::Indices() const
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

void MeshData::SetShaderID(int shaderID)
{
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

std::vector<Mesh::Instance>& MeshData::Instances()
{
    return m_instances;
}

const std::vector<Mesh::Instance>& MeshData::Instances() const
{
    return m_instances;
}

const MeshData::Instance& MeshData::GetInstance(int index) const
{
    return m_instances.at(index);
}