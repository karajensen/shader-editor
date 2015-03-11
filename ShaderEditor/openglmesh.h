////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - openglmesh.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "openglcommon.h"
class Mesh;

/**
* Base data for any polygons to be rendered
*/
class GlMeshData : boost::noncopyable
{
public:

    /**
    * Constructor
    */
    GlMeshData(const std::string& name);

    /**
    * Destructor
    */
    virtual ~GlMeshData();

    /**
    * Releases the opengl mesh
    */
    void Release();

    /**
    * Pre-renders the mesh
    */
    void PreRender();

    /**
    * Renders the mesh
    */
    virtual void Render();

    /**
    * Initialises the mesh
    * @return whether initialisation succeeded
    */
    bool Initialise();

protected:

    int m_shaderID = NO_INDEX;      ///< ID of the shader to use
    GLsizei m_vertexCount = 0;      ///< Number of vertices for the mesh
    GLsizei m_indexCount = 0;       ///< Number of indices for the mesh
    GLuint m_vaoID = 0;             ///< An unique ID for Vertex Array Object (VAO)
    GLuint m_vboID = 0;             ///< Unique ID for the Vertex Buffer Object (VBO)   
    GLuint m_iboID = 0;             ///< Unique ID for the Index Buffer Object (IBO)
    bool m_initialised = false;     ///< Whether the vertex buffer object is initialised or not
    std::vector<float> m_vertices;  ///< Mesh Vertex information
    std::vector<DWORD> m_indices;   ///< Mesh Index information
    std::string m_name;             ///< Name of the mesh
};

/**
* OpenGL screen quad
*/
class GlQuad : public GlMeshData
{
public:

    /**
    * Constructor to generate a quad
    * @param name A unique name for the quad
    */
    GlQuad(const std::string& name);
};

/**
* OpenGL complex mesh
*/
class GlMesh : public GlMeshData
{
public:

    /**
    * Constructor to generate a complex mesh
    * @param mesh The mesh to use as a template
    * @param preRender Callback to render a single mesh instance
    */
    GlMesh(const Mesh& mesh, PreRenderMesh preRender);

    /**
    * @return the mesh element
    */
    const Mesh& GetMesh() const;

    /**
    * Renders the mesh
    */
    virtual void Render() override;

private:

    const Mesh& m_mesh;        ///< Mesh information
    PreRenderMesh m_preRender; ///< Callback to render a single mesh instance
};                     

/**
* OpenGL Water mesh
*/
class GlWater : public GlMeshData
{
public:

    /**
    * Constructor for a complex mesh
    * @param mesh The mesh to use as a template
    */
    GlWater(const Water& water);

    /**
    * @return the water information for the mesh
    */
    const Water& GetWater() const;

private:

    const Water& m_water; ///< Water information
};
