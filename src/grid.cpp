////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - grid.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "grid.h"
#include "logger.h"

namespace Vertex
{
    enum Offset
    {
        PosX,
        PosY,
        PosZ,
        TextureU,
        TextureV,
        NormalX, 
        NormalY, 
        NormalZ,
        TangentX,
        TangentY,
        TangentZ,
        BitangentX,
        BitangentY,
        BitangentZ
    };
}

Grid::Grid(const std::string& name, const std::string& shaderName, int shaderID)
    : MeshData(name, shaderName, shaderID)
{
}


bool Grid::CreateGrid(const Float2& uvStretch,
                      float spacing, 
                      int rows, 
                      int columns, 
                      bool normals, 
                      bool tangents)
{
    if (!normals && tangents)
    {
        Logger::LogError("CreateGrid: Tangents require normals");
        return false;
    }

    m_columns = columns;
    m_rows = rows;
    m_spacing = spacing;
    m_vertexComponentCount = tangents ? 14 : (normals ? 8 : 5);
    m_hasNormals = normals;
    m_hasTangents = tangents;
    m_uvStretch = uvStretch;

    ResetGrid();
    return true;
}

void Grid::ResetGrid()
{
    const int vertices = m_rows * m_columns;
    const int trianglesPerQuad = 2;
    const int pointsInFace = 3;
    const int triangleNumber = ((m_rows-1)*(m_columns-1)) * trianglesPerQuad;

    const float uOffset = m_uvStretch.x / static_cast<float>(m_rows-1);
    const float vOffset = m_uvStretch.y / static_cast<float>(m_columns-1);

    m_indices.clear();
    m_vertices.clear();

    m_indices.resize(triangleNumber * pointsInFace);
    m_vertices.resize(m_vertexComponentCount * vertices);

    Float3 initialPosition;
    initialPosition.x -= m_spacing * ((m_rows-1) * 0.5f);
    initialPosition.z -= m_spacing * ((m_columns-1) * 0.5f);

    float u = 0;
    float v = 0;
    int index = 0;

    for(int r = 0; r < m_rows; ++r)
    {
        for(int c = 0; c < m_columns; ++c)
        {
            m_vertices[index + Vertex::PosX] = initialPosition.x + (r * m_spacing);
            m_vertices[index + Vertex::PosY] = initialPosition.y;
            m_vertices[index + Vertex::PosZ] = initialPosition.z + (c * m_spacing);
            m_vertices[index + Vertex::TextureU] = u;
            m_vertices[index + Vertex::TextureV] = v;

            if (m_hasNormals)
            {
                m_vertices[index + Vertex::NormalX] = 0.0f;
                m_vertices[index + Vertex::NormalY] = 1.0f;
                m_vertices[index + Vertex::NormalZ] = 0.0f;
            }

            if (m_hasTangents)
            {
                m_vertices[index + Vertex::TangentX] = 1.0f;
                m_vertices[index + Vertex::TangentY] = 0.0f;
                m_vertices[index + Vertex::TangentZ] = 0.0f;
                m_vertices[index + Vertex::BitangentX] = 0.0f;
                m_vertices[index + Vertex::BitangentY] = 0.0f;
                m_vertices[index + Vertex::BitangentZ] = 1.0f;
            }

            index += m_vertexComponentCount;
            u += uOffset;
        }

        u = 0;
        v += vOffset;
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

float Grid::GetHeight(int row, int column) const
{
    return m_vertices[GetIndex(row, column) + Vertex::PosY];
}

void Grid::SetHeight(int row, int column, float height)
{
    m_vertices[GetIndex(row, column) + Vertex::PosY] = height;
}

int Grid::Rows() const
{
    return m_rows;
}

int Grid::Columns() const
{
    return m_columns;
}

float Grid::Size() const
{
    if (Rows() != Columns())
    {
        Logger::LogError("Grid row and column mismatch");
    }
    return m_spacing * (Rows() - 1);
}

Float3 Grid::GetPosition(int index) const
{
    return Float3(m_vertices[index + Vertex::PosX],
        m_vertices[index + Vertex::PosY], m_vertices[index + Vertex::PosZ]);
}

Float3 Grid::GetNormal(int index) const
{
    return Float3(m_vertices[index + Vertex::NormalX],
        m_vertices[index + Vertex::NormalY], m_vertices[index + Vertex::NormalZ]);
}

Float3 Grid::GetTangent(int index) const
{
    return Float3(m_vertices[index + Vertex::TangentX],
        m_vertices[index + Vertex::TangentY], m_vertices[index + Vertex::TangentZ]);
}

Float2 Grid::GetUVs(int index) const
{
    return Float2(m_vertices[index + Vertex::TextureU],
        m_vertices[index + Vertex::TextureV]);
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

            m_vertices[p1index + Vertex::NormalX] += normal.x;
            m_vertices[p1index + Vertex::NormalY] += normal.y;
            m_vertices[p1index + Vertex::NormalZ] += normal.z;

            m_vertices[p2index + Vertex::NormalX] += normal.x;
            m_vertices[p2index + Vertex::NormalY] += normal.y;
            m_vertices[p2index + Vertex::NormalZ] += normal.z;

            m_vertices[p3index + Vertex::NormalX] += normal.x;
            m_vertices[p3index + Vertex::NormalY] += normal.y;
            m_vertices[p3index + Vertex::NormalZ] += normal.z;

            normal = (p2 - p4).Cross(p3 - p4);
            normal.Normalize();

            m_vertices[p2index + Vertex::NormalX] += normal.x;
            m_vertices[p2index + Vertex::NormalY] += normal.y;
            m_vertices[p2index + Vertex::NormalZ] += normal.z;

            m_vertices[p3index + Vertex::NormalX] += normal.x;
            m_vertices[p3index + Vertex::NormalY] += normal.y;
            m_vertices[p3index + Vertex::NormalZ] += normal.z;

            m_vertices[p4index + Vertex::NormalX] += normal.x;
            m_vertices[p4index + Vertex::NormalY] += normal.y;
            m_vertices[p4index + Vertex::NormalZ] += normal.z;
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

                m_vertices[p0index + Vertex::TangentX] += tangent.x;
                m_vertices[p0index + Vertex::TangentY] += tangent.y;
                m_vertices[p0index + Vertex::TangentZ] += tangent.z;

                m_vertices[p1index + Vertex::TangentX] += tangent.x;
                m_vertices[p1index + Vertex::TangentY] += tangent.y;
                m_vertices[p1index + Vertex::TangentZ] += tangent.z;

                m_vertices[p2index + Vertex::TangentX] += tangent.x;
                m_vertices[p2index + Vertex::TangentY] += tangent.y;
                m_vertices[p2index + Vertex::TangentZ] += tangent.z;
            }
        }

        for (unsigned int vertex = 0; vertex < m_vertices.size(); vertex += m_vertexComponentCount)
        {
            const Float3 normal = GetNormal(vertex).GetNormalized();
            m_vertices[vertex + Vertex::NormalX] = normal.x;
            m_vertices[vertex + Vertex::NormalY] = normal.y;
            m_vertices[vertex + Vertex::NormalZ] = normal.z;                

            if (m_hasTangents)
            {
                const Float3 tangent = GetTangent(vertex).GetNormalized();
                m_vertices[vertex + Vertex::TangentX] = tangent.x;
                m_vertices[vertex + Vertex::TangentY] = tangent.y;
                m_vertices[vertex + Vertex::TangentZ] = tangent.z;                

                // Bitangent is orthogonal to the normal/tangent
                const Float3 bitangent = normal.Cross(tangent).GetNormalized();
                m_vertices[vertex + Vertex::BitangentX] = bitangent.x;
                m_vertices[vertex + Vertex::BitangentY] = bitangent.y;
                m_vertices[vertex + Vertex::BitangentZ] = bitangent.z;
            }
        }
    }
}

float Grid::Spacing() const
{
    return m_spacing;
}

Float3 Grid::GetPosition(int row, int column) const
{
    return GetPosition(GetIndex(row, column));
}

const Float2& Grid::GetUVStretch() const
{
    return m_uvStretch;
}

void Grid::SetUVStretch(const Float2& value)
{
    m_uvStretch = value;
}