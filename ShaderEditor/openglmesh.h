////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - openglmesh.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "openglcommon.h"
struct Mesh;

/**
* Base data for any polygons to be rendered
*/
class GlMeshData : boost::noncopyable
{
public:

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
    virtual void PreRender();

    /**
    * Renders the mesh
    */
    void Render();

    /**
    * Initialises the mesh
    * @return whether initialisation succeeded
    */
    bool Initialise();

protected:

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
    * @param preRender callback to setup rendering
    */
    GlQuad(const std::string& name, PreRenderQuad preRender = nullptr);

    /**
    * PreRenders the mesh
    */
    virtual void PreRender() override;

    /**
    * Sets the id of the texture used
    */
    void SetTexture(int ID);

private:

    int m_texture = NO_INDEX;            ///< Index for the quad texture
    PreRenderQuad m_preRender = nullptr; ///< Callback to setup rendering
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
    * @param preRender Callback to pre-render the mesh
    */
    GlMesh(const Mesh& mesh, PreRenderMesh preRender);

    /**
    * PreRenders the mesh
    */
    virtual void PreRender() override;

    /**
    * @return the unique ID for the mesh shader
    */
    int GetShaderID() const;

    /**
    * @return the mesh element
    */
    const Mesh& GetMesh() const;

private:

    PreRenderMesh m_preRender = nullptr; ///< Callback to pre-render the mesh
    const Mesh& m_mesh;                  ///< Mesh information
};                     

/**
* OpenGL Water mesh
*/
class GlWater : public GlMesh
{
public:

    /**
    * Constructor for a complex mesh
    * @param mesh The mesh to use as a template
    * @param preRender Callback to pre-render the mesh
    */
    GlWater(const Water& water, PreRenderMesh preRender);

    /**
    * @return the water information for the mesh
    */
    const Water& GetWater() const;

private:

    const Water& m_water; ///< Water information
};
