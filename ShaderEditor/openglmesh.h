////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - openglmesh.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "openglcommon.h"
struct Mesh;

/**
* OpenGL Individual mesh or quad
*/
class GlMesh : boost::noncopyable
{
public:

    /**
    * Constructor to generate a complex mesh
    * @param mesh The mesh to use as a template
    */
    GlMesh(const Mesh* mesh);

    /**
    * Constructor to generate a quad
    * @param name A unique name for the quad
    */
    GlMesh(const std::string& name);

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
    * @return whether initialisation succeeded
    */
    bool Initialise();

    /**
    * @return whether the mesh requires backface culling or not
    */
    bool ShouldBackfaceCull() const;

    /**
    * @return a container of unique IDs for the mesh textures
    */
    const std::vector<int>& GetTextureIDs() const;

    /**
    * @return the unique ID for the mesh shader
    */
    int GetShaderID() const;

    /**
    * @return the mesh element
    */
    const Mesh& GetMesh() const;

private:

    GLsizei m_vertexCount = 0;      ///< Number of vertices for the mesh
    GLsizei m_indexCount = 0;       ///< Number of indices for the mesh
    GLuint m_vaoID = 0;             ///< An unique ID for Vertex Array Object (VAO)
    GLuint m_vboID = 0;             ///< Unique ID for the Vertex Buffer Object (VBO)   
    GLuint m_iboID = 0;             ///< Unique ID for the Index Buffer Object (IBO)
    bool m_initialised = false;     ///< Whether the vertex buffer object is initialised or not
    std::vector<float> m_vertices;  ///< Mesh Vertex information
    std::vector<DWORD> m_indices;   ///< Mesh Index information
    const Mesh* m_mesh = nullptr;   ///< Mesh information or null if a quad
    const std::string m_name;       ///< Name of the mesh
};                     