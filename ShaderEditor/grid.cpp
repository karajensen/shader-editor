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
        NORMAL_Z
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

            index += m_vertexComponentCount;
            u += 0.5;
        }

        u = 0;
        v += 0.5;
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

void Grid::Set(int row, int column, float height)
{
    m_vertices[GetIndex(row, column)] = height;
}

float Grid::Get(int row, int column) const
{
    return m_vertices[GetIndex(row, column)];
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

void Grid::RecalculateNormals()
{
    // For each triangle add the normal to the vertex
    // No need to renormalise as it is done in the fragment shader

    assert(m_hasNormals);

    Float3 normal;
    for(int r = 0; r < m_rows-1; ++r)
    {
        for(int c = 0; c < m_columns-1; ++c)
        {
            const int p1index = GetIndex(r, c);
            const int p2index = GetIndex(r+1, c);
            const int p3index = GetIndex(r, c+1);
            const int p4index = GetIndex(r+1, c+1);

            const Float3 p1(m_vertices[p1index+POS_X], 
                m_vertices[p1index+POS_Y], m_vertices[p1index+POS_Z]);

            const Float3 p2(m_vertices[p2index+POS_X], 
                m_vertices[p2index+POS_Y], m_vertices[p2index+POS_Z]);

            const Float3 p3(m_vertices[p3index+POS_X], 
                m_vertices[p3index+POS_Y], m_vertices[p3index+POS_Z]);

            const Float3 p4(m_vertices[p4index+POS_X], 
                m_vertices[p4index+POS_Y], m_vertices[p4index+POS_Z]);

            normal = (p1 - p2).Cross(p3 - p2);
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
}