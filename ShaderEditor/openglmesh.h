////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - openglmesh.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "openglcommon.h"

/**
* OpenGL Individual mesh
*/
class GlMesh
{
public:

    /**
    * Constructor
    */
    GlMesh();

    /**
    * Destructor
    */
    ~GlMesh();

    /**
    * Releases the opengl mesh
    */
    void Release();

    /**
    * Renders the mesh
    */
    void Render();

    /**
    * Initialises the mesh
    * @param vertexArrayID an unique ID for the opengl mesh
    */
    void Initialise(unsigned int vertexArrayID);

private:

    GLsizei m_vertexCount;          ///< Number of vertices for the mesh
    unsigned int m_vertexArrayID;   ///< An unique ID for Vertex Array Object (VAO)
};                     