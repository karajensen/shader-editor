////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - grid.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "grid.h"
#include "common.h"

namespace
{
    enum VertexOffset
    {
        POS_X,
        POS_Y,
        POS_Z,
        TEXTURE_U,
        TEXTURE_V,
        NORMAL_X, 
        NORMAL_Y, 
        NORMAL_Z,
        TANGENT_X,
        TANGENT_Y,
        TANGENT_Z,
        BITANGENT_X,
        BITANGENT_Y,
        BITANGENT_Z
    };
}

Grid::Grid(const boost::property_tree::ptree& node) :
    MeshData(node)
{
    m_position.x = GetAttribute<float>(node, "Position", "x");
    m_position.y = GetAttribute<float>(node, "Position", "y");
    m_position.z = GetAttribute<float>(node, "Position", "z");
    m_spacing = GetAttribute<float>(node, "Grid", "spacing");
    m_rows = GetAttribute<int>(node, "Grid", "rows");
    m_columns = GetAttribute<int>(node, "Grid", "columns");
    m_uvStretch.x = GetAttributeOptional<float>(node, "UVStretch", "u", 1.0f);
    m_uvStretch.y = GetAttributeOptional<float>(node, "UVStretch", "v", 1.0f);
}

void Grid::Write(boost::property_tree::ptree& node) const
{
    MeshData::Write(node);

    node.add("Position.<xmlattr>.x", m_position.x);
    node.add("Position.<xmlattr>.y", m_position.y);
    node.add("Position.<xmlattr>.z", m_position.z);
    node.add("Grid.<xmlattr>.spacing", m_spacing);
    node.add("Grid.<xmlattr>.rows", m_rows);
    node.add("Grid.<xmlattr>.columns", m_columns);
    AddValueOptional(node, "UVStretch.<xmlattr>.u", m_uvStretch.x, 1.0f);
    AddValueOptional(node, "UVStretch.<xmlattr>.v", m_uvStretch.y, 1.0f);
}

bool Grid::CreateGrid(bool normals, bool tangents)
{
    if (!normals && tangents)
    {
        Logger::LogError("CreateGrid: Tangents require normals");
        return false;
    }

    m_vertexComponentCount = tangents ? 14 : (normals ? 8 : 5);
    m_hasNormals = normals;
    m_hasTangents = tangents;

    ResetGrid();
    return true;
}
void Grid::ResetGrid()
{
    const int vertices = m_rows * m_columns;
    const int trianglesPerQuad = 2;
    const int pointsInFace = 3;
    const int triangleNumber = ((m_rows-1)*(m_columns-1)) * trianglesPerQuad;

    m_indices.clear();
    m_vertices.clear();

    m_indices.resize(triangleNumber * pointsInFace);
    m_vertices.resize(m_vertexComponentCount * vertices);

    Float3 initialPosition = m_position;
    initialPosition.x -= m_spacing * (m_rows * 0.5f);
    initialPosition.z -= m_spacing * (m_columns * 0.5f);

    float u = 0;
    float v = 0;
    int index = 0;

    for(int r = 0; r < m_rows; ++r)
    {
        for(int c = 0; c < m_columns; ++c)
        {
            m_vertices[index + POS_X] = initialPosition.x + (r * m_spacing);
            m_vertices[index + POS_Y] = initialPosition.y;
            m_vertices[index + POS_Z] = initialPosition.z + (c * m_spacing);
            m_vertices[index + TEXTURE_U] = u;
            m_vertices[index + TEXTURE_V] = v;

            if (m_hasNormals)
            {
                m_vertices[index + NORMAL_X] = 0.0f;
                m_vertices[index + NORMAL_Y] = 1.0f;
                m_vertices[index + NORMAL_Z] = 0.0f;
            }

            if (m_hasTangents)
            {
                m_vertices[index + TANGENT_X] = 1.0f;
                m_vertices[index + TANGENT_Y] = 0.0f;
                m_vertices[index + TANGENT_Z] = 0.0f;
                m_vertices[index + BITANGENT_X] = 0.0f;
                m_vertices[index + BITANGENT_Y] = 0.0f;
                m_vertices[index + BITANGENT_Z] = 1.0f;
            }

            index += m_vertexComponentCount;
            u += 0.5f * m_uvStretch.x;
        }

        u = 0;
        v += 0.5f * m_uvStretch.y;
    }

    index = 0;
    for(int r = 0; r < m_rows-1; ++r)
    {
        for(int c = 0; c < m_columns-1; ++c)
        {
            m_indices[index] = r * m_columns + c;
            m_indices[index + 1] = (r + 1) * m_columns + (c + 1);
            m_indices[index + 2] = (r + 1) * m_columns + c;

            m_indices[index + 3] = r * m_columns + c;
            m_indices[index + 4] = r * m_columns + (c + 1);
            m_indices[index + 5] = (r + 1) * m_columns + (c + 1);
        
            index += 6;
        }
    }
}

unsigned int Grid::GetIndex(int row, int column) const
{
    return (row * m_columns * m_vertexComponentCount)
        + (column * m_vertexComponentCount);
}

void Grid::SetHeight(int row, int column, float height)
{
    m_vertices[GetIndex(row, column) + POS_Y] = m_position.y + height;
}

bool Grid::Valid(int row, int column) const
{
    const auto index = GetIndex(row, column);
    return index >= 0 && index < m_vertices.size();
}

int Grid::Rows() const
{
    return m_rows;
}

int Grid::Columns() const
{
    return m_columns;
}

const Float3& Grid::Position() const
{
    return m_position;
}

Float3 Grid::GetPosition(int index) const
{
    return Float3(m_vertices[index + POS_X],
        m_vertices[index + POS_Y], m_vertices[index + POS_Z]);
}

Float3 Grid::GetNormal(int index) const
{
    return Float3(m_vertices[index + NORMAL_X],
        m_vertices[index + NORMAL_Y], m_vertices[index + NORMAL_Z]);
}

Float3 Grid::GetTangent(int index) const
{
    return Float3(m_vertices[index + TANGENT_X],
        m_vertices[index + TANGENT_Y], m_vertices[index + TANGENT_Z]);
}

Float2 Grid::GetUVs(int index) const
{
    return Float2(m_vertices[index + TEXTURE_U],
        m_vertices[index + TEXTURE_V]);
}

void Grid::RecalculateNormals()
{
    if (!m_hasNormals)
    {
        return;
    }

    // For each triangle add the normal to the vertex
    for (int r = 0; r < m_rows - 1; ++r)
    {
        for (int c = 0; c < m_columns - 1; ++c)
        {
            const int p1index = GetIndex(r, c);
            const int p2index = GetIndex(r + 1, c);
            const int p3index = GetIndex(r, c + 1);
            const int p4index = GetIndex(r + 1, c + 1);

            const Float3 p1(GetPosition(p1index));
            const Float3 p2(GetPosition(p2index));
            const Float3 p3(GetPosition(p3index));
            const Float3 p4(GetPosition(p4index));

            Float3 normal = (p1 - p2).Cross(p3 - p2);
            normal.Normalize();

            m_vertices[p1index + NORMAL_X] += normal.x;
            m_vertices[p1index + NORMAL_Y] += normal.y;
            m_vertices[p1index + NORMAL_Z] += normal.z;

            m_vertices[p2index + NORMAL_X] += normal.x;
            m_vertices[p2index + NORMAL_Y] += normal.y;
            m_vertices[p2index + NORMAL_Z] += normal.z;

            m_vertices[p3index + NORMAL_X] += normal.x;
            m_vertices[p3index + NORMAL_Y] += normal.y;
            m_vertices[p3index + NORMAL_Z] += normal.z;

            normal = (p2 - p4).Cross(p3 - p4);
            normal.Normalize();

            m_vertices[p2index + NORMAL_X] += normal.x;
            m_vertices[p2index + NORMAL_Y] += normal.y;
            m_vertices[p2index + NORMAL_Z] += normal.z;

            m_vertices[p3index + NORMAL_X] += normal.x;
            m_vertices[p3index + NORMAL_Y] += normal.y;
            m_vertices[p3index + NORMAL_Z] += normal.z;

            m_vertices[p4index + NORMAL_X] += normal.x;
            m_vertices[p4index + NORMAL_Y] += normal.y;
            m_vertices[p4index + NORMAL_Z] += normal.z;
        }
    }

    // For each triangle add the tangent to the vertex
    if (m_hasTangents)
    {
        for (int r = 0; r < m_rows - 1; ++r)
        {
            for (int c = 0; c < m_columns - 1; ++c)
            {
                const int p0index = GetIndex(r, c);
                const int p1index = GetIndex(r + 1, c);
                const int p2index = GetIndex(r, c + 1);

                const Float3 p0(GetPosition(p0index));
                const Float3 p1(GetPosition(p1index));
                const Float3 p2(GetPosition(p2index));

                const Float2 uv0(GetUVs(p0index));
                const Float2 uv1(GetUVs(p1index));
                const Float2 uv2(GetUVs(p2index));

                const Float3 q1(p1 - p0);
                const Float3 q2(p2 - p0);
                const float s1 = uv1.x - uv0.x;
                const float s2 = uv2.x - uv0.x;
                const float t1 = uv1.y - uv0.y;
                const float t2 = uv2.y - uv0.y;

                // Mathematics for 3D Game Programming and Computer Graphics p184
                // Plane Equation is p - p0 = sT + tB
                // Solve T using q1 = s1T + t1B and q2 = s2T + t2B
                // Substitute B = (q1 - s1T) / t1 into q2 = s2T + t2B
                // Rearranging T = (q2 - t2q1/t1) / (s2 - (t2s1/t1))
                const Float3 tangent = (q2 - ((q1 * t2) / t1)) / (s2 - ((t2 * s1) / t1));

                m_vertices[p0index + TANGENT_X] += tangent.x;
                m_vertices[p0index + TANGENT_Y] += tangent.y;
                m_vertices[p0index + TANGENT_Z] += tangent.z;

                m_vertices[p1index + TANGENT_X] += tangent.x;
                m_vertices[p1index + TANGENT_Y] += tangent.y;
                m_vertices[p1index + TANGENT_Z] += tangent.z;

                m_vertices[p2index + TANGENT_X] += tangent.x;
                m_vertices[p2index + TANGENT_Y] += tangent.y;
                m_vertices[p2index + TANGENT_Z] += tangent.z;
            }
        }

        for (unsigned int vertex = 0; vertex < m_vertices.size(); vertex += m_vertexComponentCount)
        {
            const Float3 normal = GetNormal(vertex).GetNormalized();
            m_vertices[vertex + NORMAL_X] = normal.x;
            m_vertices[vertex + NORMAL_Y] = normal.y;
            m_vertices[vertex + NORMAL_Z] = normal.z;                

            if (m_hasTangents)
            {
                const Float3 tangent = GetTangent(vertex).GetNormalized();
                m_vertices[vertex + TANGENT_X] = tangent.x;
                m_vertices[vertex + TANGENT_Y] = tangent.y;
                m_vertices[vertex + TANGENT_Z] = tangent.z;                

                // Bitangent is orthogonal to the normal/tangent
                const Float3 bitangent = normal.Cross(tangent).GetNormalized();
                m_vertices[vertex + BITANGENT_X] = bitangent.x;
                m_vertices[vertex + BITANGENT_Y] = bitangent.y;
                m_vertices[vertex + BITANGENT_Z] = bitangent.z;
            }
        }
    }
}