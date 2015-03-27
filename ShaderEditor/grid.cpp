////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - grid.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "grid.h"
#include "common.h"

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

void Grid::CreateGrid(bool normals)
{
    m_vertexComponentCount = normals ? 8 : 5;
    m_hasNormals = normals;

    ResetGrid();
}
void Grid::ResetGrid()
{
    const int vertices = m_rows * m_columns;
    const int trianglesPerQuad = 2;
    const int pointsInFace = 3;
    const int triangleNumber = ((m_rows-1)*(m_columns-1)) * trianglesPerQuad;

    m_heights.clear();
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
        m_heights.emplace_back();
        for(int c = 0; c < m_columns; ++c)
        {
            m_vertices[index] = initialPosition.x + (r * m_spacing);
            m_vertices[index + 1] = initialPosition.y;
            m_vertices[index + 2] = initialPosition.z + (c * m_spacing);
            m_vertices[index + 3] = u;
            m_vertices[index + 4] = v;

            if (m_hasNormals)
            {
                m_vertices[index + 5] = 0.0f;
                m_vertices[index + 6] = 1.0f;
                m_vertices[index + 7] = 0.0f;
            }

            m_heights[r].push_back(&m_vertices[index + 1]);

            index += m_vertexComponentCount;
            u += 0.5;
        }

        assert(m_heights[r].size() == m_columns);
        u = 0;
        v += 0.5;
    }

    assert(m_heights.size() == m_rows);

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

void Grid::Set(int row, int column, float height)
{
    *m_heights[row][column] = height;
}

float Grid::Get(int row, int column) const
{
    return *m_heights[row][column];
}

bool Grid::Valid(int row, int column) const
{
    return row < static_cast<int>(m_heights.size()) && row >= 0 &&
        column < static_cast<int>(m_heights[row].size()) && column >= 0;
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
    for(int r = 0; r < m_rows; ++r)
    {
        for(int c = 0; c < m_columns; ++c)
        {
            const int offset = c * m_vertexComponentCount;
            const int p1index = (r * m_columns) + offset;
            const int p2index = ((r + 1) * m_columns) + offset;
            const int p3index = (r * m_columns) + offset + 1;
            const int p4index = ((r + 1) * m_columns) + offset + 1;

            const Float3 p1(m_vertices[p1index], 
                m_vertices[p1index+1], m_vertices[p1index+2]);

            const Float3 p2(m_vertices[p2index], 
                m_vertices[p2index+1], m_vertices[p2index+2]);

            const Float3 p3(m_vertices[p3index], 
                m_vertices[p3index+1], m_vertices[p3index+2]);

            const Float3 p4(m_vertices[p4index], 
                m_vertices[p4index+1], m_vertices[p4index+2]);

            normal = (p1 - p2).Cross(p3 - p2);
            normal.Normalize();

            m_vertices[p1index + 5] += normal.x;
            m_vertices[p1index + 6] += normal.y;
            m_vertices[p1index + 7] += normal.z;

            m_vertices[p2index + 5] += normal.x;
            m_vertices[p2index + 6] += normal.y;
            m_vertices[p2index + 7] += normal.z;

            m_vertices[p3index + 5] += normal.x;
            m_vertices[p3index + 6] += normal.y;
            m_vertices[p3index + 7] += normal.z;

            normal = (p2 - p4).Cross(p3 - p4);
            normal.Normalize();

            m_vertices[p2index + 5] += normal.x;
            m_vertices[p2index + 6] += normal.y;
            m_vertices[p2index + 7] += normal.z;

            m_vertices[p3index + 5] += normal.x;
            m_vertices[p3index + 6] += normal.y;
            m_vertices[p3index + 7] += normal.z;

            m_vertices[p4index + 5] += normal.x;
            m_vertices[p4index + 6] += normal.y;
            m_vertices[p4index + 7] += normal.z;
        }
    }
}