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

void MeshData::CreateGrid(const Float3& position, float spacing, int rows)
{
    const int vertices = rows * rows;
    const int trianglesPerQuad = 2;
    const int pointsInFace = 3;
    const int triangleNumber = ((rows-1)*(rows-1)) * trianglesPerQuad;

    m_vertexComponentCount = 5;
    m_indices.resize(triangleNumber * pointsInFace);
    m_vertices.resize(m_vertexComponentCount * vertices);

    const int mininum = -rows/2;
    const int maximum = rows/2;
    Float3 currentPosition = position;

    float u = 0;
    float v = 0;
    int index = 0;

    for(int x = mininum; x < maximum; ++x)
    {
        for(int z = mininum; z < maximum; ++z)
        {
            m_vertices[index] = currentPosition.x;
            m_vertices[index+1] = currentPosition.y;
            m_vertices[index+2] = currentPosition.z;
            m_vertices[index+3] = u;
            m_vertices[index+4] = v;

            currentPosition.x += x * spacing;
            currentPosition.z += z * spacing;

            index += m_vertexComponentCount;
            u += 0.5;
        }
        u = 0;
        v += 0.5;
    }

    index = 0;
    for(int x = 0; x < rows-1; ++x)
    {
        for(int y = 0; y < rows-1; ++y)
        {
            m_indices[index] = (x * rows) + y;
            m_indices[index+1] = (x * rows) + y + 1;
            m_indices[index+2] = ((x + 1) * rows) + y;

            m_indices[index+3] = ((x + 1) * rows) + y;
            m_indices[index+4] = (x * rows) + y + 1;
            m_indices[index+5] = ((x + 1) * rows)+ y + 1;
        
            index += 6;
        }
    }
}