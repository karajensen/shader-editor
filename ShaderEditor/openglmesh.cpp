////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - openglmesh.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "openglmesh.h"
#include "elements.h"

GlWater::GlWater(const Water& water, PreRenderMesh preRender) :
    GlMesh(water, preRender),
    m_water(water)
{
}

GlMesh::GlMesh(const Mesh& mesh, PreRenderMesh preRender) :
    m_mesh(mesh),
    m_preRender(preRender)
{
    m_vertexCount = mesh.vertexCount;
    m_indexCount = mesh.indexCount;
    m_vertices = mesh.vertices;
    m_indices = mesh.indices;
    m_name = mesh.name;
}

GlQuad::GlQuad(const std::string& name, PreRenderQuad preRender) :
    m_preRender(preRender)
{
    m_name = name;

    // Top left corner
    m_vertices.emplace_back(-1.0f); // x
    m_vertices.emplace_back(-1.0f);  // y
    m_vertices.emplace_back(0.0f);  // z
    m_vertices.emplace_back(0.0f); // u
    m_vertices.emplace_back(0.0f);  // v

    // Top right corner
    m_vertices.emplace_back(1.0f); // x
    m_vertices.emplace_back(-1.0f); // y
    m_vertices.emplace_back(0.0f); // z
    m_vertices.emplace_back(1.0f); // u
    m_vertices.emplace_back(0.0f); // v

    // Bot right corner
    m_vertices.emplace_back(1.0f);  // x
    m_vertices.emplace_back(1.0f); // y
    m_vertices.emplace_back(0.0f);  // z
    m_vertices.emplace_back(1.0f);  // u
    m_vertices.emplace_back(1.0f); // v

    // Bot left corner
    m_vertices.emplace_back(-1.0f); // x
    m_vertices.emplace_back(1.0f); // y
    m_vertices.emplace_back(0.0f);  // z
    m_vertices.emplace_back(0.0f); // u
    m_vertices.emplace_back(1.0f); // v

    m_indices.emplace_back(0);
    m_indices.emplace_back(3);
    m_indices.emplace_back(1);
              
    m_indices.emplace_back(1);
    m_indices.emplace_back(3);
    m_indices.emplace_back(2);

    m_vertexCount = m_vertices.size();
    m_indexCount = m_indices.size();
}

GlMeshData::~GlMeshData()
{
    Release();
}

void GlMeshData::Release()
{
    if(m_initialised)
    {
        glDeleteBuffers(1, &m_vboID);
        glDeleteBuffers(1, &m_iboID);
        glDeleteBuffers(1, &m_vaoID);
        m_initialised = false;
    }
}

bool GlMeshData::Initialise()
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

void GlQuad::SetTexture(int ID)
{
    m_texture = ID;
}

void GlQuad::PreRender()
{
    if (m_preRender)
    {
        m_preRender(m_texture);
    }
    GlMeshData::PreRender();
}

void GlMesh::PreRender()
{
    if (m_preRender)
    {
        m_preRender(m_mesh);
    }
    GlMeshData::PreRender();
}

void GlMeshData::PreRender()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
}

void GlMeshData::Render()
{
    assert(m_initialised);
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
}

int GlMesh::GetShaderID() const 
{ 
    return m_mesh.shaderIndex;
}

const Mesh& GlMesh::GetMesh() const
{
    return m_mesh;
}

const Water& GlWater::GetWater() const
{
    return m_water;
}