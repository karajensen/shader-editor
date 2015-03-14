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
    node.add("Name", m_name);
    node.add("Shader", m_shaderName);

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

void MeshData::CreateGrid(const Float3& position, float spacing, int rows, int columns)
{
    const int vertices = rows * columns;
    const int trianglesPerQuad = 2;
    const int pointsInFace = 3;
    const int triangleNumber = ((rows-1)*(columns-1)) * trianglesPerQuad;

    m_vertexComponentCount = 5;
    m_indices.resize(triangleNumber * pointsInFace);
    m_vertices.resize(m_vertexComponentCount * vertices);

    Float3 initialPosition = position;
    initialPosition.x -= spacing * (rows * 0.5f);
    initialPosition.z -= spacing * (columns * 0.5f);

    float u = 0;
    float v = 0;
    int index = 0;

    for(int r = 0; r < rows; ++r)
    {
        for(int c = 0; c < columns; ++c)
        {
            m_vertices[index] = initialPosition.x + (r * spacing);
            m_vertices[index + 1] = initialPosition.y;
            m_vertices[index + 2] = initialPosition.z + (c * spacing);
            m_vertices[index + 3] = u;
            m_vertices[index + 4] = v;

            index += m_vertexComponentCount;
            u += 0.5;
        }
        u = 0;
        v += 0.5;
    }

    index = 0;
    for(int r = 0; r < rows-1; ++r)
    {
        for(int c = 0; c < columns-1; ++c)
        {
            m_indices[index] = r * columns + c;
            m_indices[index + 1] = (r + 1) * columns + c;
            m_indices[index + 2] = (r + 1) * columns + (c + 1);

            m_indices[index + 3] = r * columns + c;
            m_indices[index + 4] = (r + 1) * columns + (c + 1);
            m_indices[index + 5] = r * columns + (c + 1);
        
            index += 6;
        }
    }
}