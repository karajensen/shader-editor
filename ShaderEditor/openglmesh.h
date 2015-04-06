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
    * Constructor for predefined buffers
    * @param data Information for the mesh buffers
    * @param preRender Callback to render instances
    */
    GlMeshData(const MeshData& data, PreRenderMesh preRender);

    /**
    * Constructor for empty buffers
    * @param name The name of the mesh
    * @param vertices The vertex buffer
    * @param indices The index buffer
    */
    GlMeshData(const std::string& name, 
               const std::vector<float>& vertices,
               const std::vector<unsigned int>& indices);

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

    /**
    * Renders the mesh instances
    * @param instances The mesh intances to render
    */
    void RenderInstances(const std::vector<MeshData::Instance>& instances);

    /**
    * Fills the vertex and index buffers
    * @return whether call was successful
    */
    bool FillBuffers();

    int m_shaderID = NO_INDEX;                  ///< ID of the shader to use
    GLuint m_vaoID = 0;                         ///< An unique ID for Vertex Array Object
    GLuint m_vboID = 0;                         ///< Unique ID for the Vertex Buffer Object
    GLuint m_iboID = 0;                         ///< Unique ID for the Index Buffer Object
    bool m_initialised = false;                 ///< Whether the vertex buffer object is initialised or not
    std::string m_name;                         ///< Name of the mesh
    const std::vector<float>& m_vertices;       ///< Vertex buffer data
    const std::vector<unsigned int>& m_indices; ///< Index buffer data
    PreRenderMesh m_preRender = nullptr;        ///< Callback to render instances
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

private:

    std::vector<float> m_vertices;        ///< Vertex information
    std::vector<unsigned int> m_indices;  ///< Index information
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
    * @param preRender Callback to render a single instance
    */
    GlWater(const Water& water, PreRenderMesh preRender);

    /**
    * @return the water information for the mesh
    */
    const Water& GetWater() const;

    /**
    * Renders the mesh
    */
    virtual void Render() override;

private:

    const Water& m_water; ///< Water information
};

/**
* OpenGL Terrain mesh
*/
class GlTerrain : public GlMeshData
{
public:

    /**
    * Constructor for a complex mesh
    * @param mesh The mesh to use as a template
    * @param preRender Callback to render a single instance
    */
    GlTerrain(const Terrain& terrain, PreRenderMesh preRender);

    /**
    * @return the terrain information for the mesh
    */
    const Terrain& GetTerrain() const;

    /**
    * Renders the mesh
    */
    virtual void Render() override;

    /**
    * Reloads the terrain
    * @return whether reloading was successful
    */
    bool Reload();

private:

    const Terrain& m_terrain; ///< Terrain information
};
