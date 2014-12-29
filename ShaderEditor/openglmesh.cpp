////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - openglmesh.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "openglmesh.h"
#include "elements.h"

GlWater::GlWater(const Water* water) :
    GlMesh(water),
    m_water(water)
{
}

GlMesh::GlMesh(const Mesh* mesh) :
    m_vertexCount(mesh->vertexCount),
    m_indexCount(mesh->indexCount),
    m_mesh(mesh),
    m_vertices(mesh->vertices),
    m_indices(mesh->indices),
    m_name(mesh->name)
{
}

GlMesh::GlMesh(const std::string& name) :
    m_name(name)
{
    // Top left corner
    m_vertices.emplace_back(-1.0); // x
    m_vertices.emplace_back(-1.0);  // y
    m_vertices.emplace_back(0.0);  // z
    m_vertices.emplace_back(0.0); // u
    m_vertices.emplace_back(0.0);  // v

    // Top right corner
    m_vertices.emplace_back(1.0); // x
    m_vertices.emplace_back(-1.0); // y
    m_vertices.emplace_back(0.0); // z
    m_vertices.emplace_back(1.0); // u
    m_vertices.emplace_back(0.0); // v

    // Bot right corner
    m_vertices.emplace_back(1.0);  // x
    m_vertices.emplace_back(1.0); // y
    m_vertices.emplace_back(0.0);  // z
    m_vertices.emplace_back(1.0);  // u
    m_vertices.emplace_back(1.0); // v

    // Bot left corner
    m_vertices.emplace_back(-1.0); // x
    m_vertices.emplace_back(1.0); // y
    m_vertices.emplace_back(0.0);  // z
    m_vertices.emplace_back(0.0); // u
    m_vertices.emplace_back(1.0); // v

    m_indices.emplace_back(0);
    m_indices.emplace_back(3);
    m_indices.emplace_back(1);
              
    m_indices.emplace_back(1);
    m_indices.emplace_back(3);
    m_indices.emplace_back(2);

    m_vertexCount = m_vertices.size();
    m_indexCount = m_indices.size();
}

GlMesh::~GlMesh()
{
    Release();
}

void GlMesh::Release()
{
    if(m_initialised)
    {
        glDeleteBuffers(1, &m_vboID);
        glDeleteBuffers(1, &m_iboID);
        glDeleteBuffers(1, &m_vaoID);
        m_initialised = false;
    }
}

bool GlMesh::Initialise()
{
    glGenVertexArrays(1, &m_vaoID);
    glBindVertexArray(m_vaoID);

    glGenBuffers(1, &m_vboID);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*m_vertices.size(), 
        &m_vertices[0], GL_STREAM_DRAW);

    glGenBuffers(1, &m_iboID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(DWORD)*m_indices.size(), 
        &m_indices[0], GL_STREAM_DRAW);

    if(HasCallFailed())
    {
        Logger::LogError("OpenGL: Failed " + m_name + " buffers");
        return false;
    }

    m_initialised = true;
    return true;
}

void GlMesh::PreRender()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
}

void GlMesh::Render()
{
    assert(m_initialised);
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
}

bool GlMesh::ShouldBackfaceCull() const
{
    assert(m_mesh);
    return m_mesh->backfacecull;
}

const std::vector<int>& GlMesh::GetTextureIDs() const
{
    assert(m_mesh);
    return m_mesh->textureIDs;
}

int GlMesh::GetShaderID() const 
{ 
    assert(m_mesh);
    return m_mesh->shaderIndex;
}

const Mesh& GlMesh::GetMesh() const
{
    assert(m_mesh);
    return *m_mesh;
}

const Water& GlWater::GetWater() const
{
    assert(m_water);
    return *m_water;
}