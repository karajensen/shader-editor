////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - openglmesh.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "openglmesh.h"
#include "elements.h"

GlMesh::GlMesh(const Mesh& mesh) :
    m_vertexCount(0),
    m_vaoID(0),
    m_mesh(mesh),
    m_vboID(0),
    m_initialised(false)
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
        m_initialised = false;
    }
}

void GlMesh::Initialise(unsigned int vertexArrayID)
{
    //////////////////////////////TO CUSTOMISE
    struct VERTEX
    {
        GLfloat X, Y, Z, R, G, B;
    };

    VERTEX vertices[] =
    {
        {0.0f, 0.5f, -5.0f, 1.0f, 0.0f, 0.0f},
        {0.45f, -0.5, -5.0f, 0.0f, 1.0f, 0.0f},
        {-0.45f, -0.5f, -5.0f, 0.0f, 0.0f, 1.0f}
    };

    m_vertexCount = 3;
    //////////////////////////////

    m_vaoID = vertexArrayID;
    glBindVertexArray(m_vaoID);

    glGenBuffers(1, &m_vboID);
    m_initialised = true;

    glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void GlMesh::PreRender()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_vboID);

    //m_mesh.backfacecull ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
    glDisable(GL_CULL_FACE);
}

void GlMesh::Render()
{
    glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
}