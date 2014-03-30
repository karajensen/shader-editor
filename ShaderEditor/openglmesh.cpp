////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - openglmesh.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "openglmesh.h"

GlMesh::GlMesh() :
    m_vertexCount(0),
    m_vertexArrayID(0)
{
}

GlMesh::~GlMesh()
{
}

void GlMesh::Initialise(unsigned int vertexArrayID)
{
    m_vertexArrayID = vertexArrayID;
    glBindVertexArray(m_vertexArrayID);

    //////////////////////////////TO CUSTOMISE
    GLfloat vertices[] = {0.0f, 0.5f, -5.0f, 0.45f, -0.5f, -5.0f, -0.45f, -0.5f, -5.0f};
    GLfloat colours[] = {1.0f, 0.0f, 0.0f,0.0f, 1.0f, 0.0f,0.0f, 0.0f, 1.0f};
    unsigned int vertexBufferObjID[2];
    m_vertexCount = 3;

    glGenBuffers(2, vertexBufferObjID);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID[0]);
    glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
 
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID[1]);
    glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), colours, GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
    //////////////////////////////
 
    glBindVertexArray(0);
}

void GlMesh::Render()
{
    glDisable(GL_CULL_FACE);

	glBindVertexArray(m_vertexArrayID);	
    glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
    glBindVertexArray(0);
}