////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - openglmesh.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "openglmesh.h"
#include "elements.h"

GlMesh::GlMesh(const Mesh& mesh) :
    m_vertexCount(mesh.vertexCount),
    m_indexCount(mesh.indexCount),
    m_vaoID(0),
    m_vboID(0),
    m_iboID(0),
    m_initialised(false),
    m_mesh(mesh)
{
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
        m_initialised = false;
    }
}

void GlMesh::Initialise(unsigned int vertexArrayID)
{
    m_vaoID = vertexArrayID;
    glBindVertexArray(m_vaoID);

    glGenBuffers(1, &m_vboID);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*m_mesh.vertices.size(), 
        &m_mesh.vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &m_iboID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(DWORD)*m_mesh.indices.size(), 
        &m_mesh.indices[0], GL_STATIC_DRAW);

    if(HasCallFailed())
    {
        Logger::LogError("OpenGL: Failed " + m_mesh.name + " buffers");
    }

    m_initialised = true;
}

void GlMesh::PreRender()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
}

void GlMesh::Render()
{
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
}

bool GlMesh::ShouldBackfaceCull() const
{
    return m_mesh.backfacecull;
}

const std::vector<int>& GlMesh::GetTextureIDs() const
{
    return m_mesh.textureIDs;
}

int GlMesh::GetShaderID() const 
{ 
    return m_mesh.shaderIndex;
}