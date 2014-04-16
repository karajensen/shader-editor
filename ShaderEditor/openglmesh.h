////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - openglmesh.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "openglcommon.h"
struct Mesh;

/**
* OpenGL Individual mesh
*/
class GlMesh
{
public:

    /**
    * Constructor
    * @param mesh The mesh to use as a template
    */
    GlMesh(const Mesh& mesh);

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
    * Sets up the mesh for rendering
    */
    void PreRender();

    /**
    * Initialises the mesh
    * @param vertexArrayID an unique ID for the opengl mesh
    */
    void Initialise(unsigned int vertexArrayID);

private:

    const Mesh& m_mesh;     ///< Mesh template for this openglmesh
    GLsizei m_vertexCount;  ///< Number of vertices for the mesh
    GLsizei m_indexCount;   ///< Number of indices for the mesh
    GLuint m_vaoID;         ///< An unique ID for Vertex Array Object (VAO)
    GLuint m_vboID;         ///< Unique ID for the Vertex Buffer Object (VBO)   
    bool m_initialised;     ///< Whether the vertex buffer object is initialised or not
};                     