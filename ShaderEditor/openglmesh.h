////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - openglmesh.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "openglcommon.h"
class Mesh;

/**
* Base data for any polygons to be rendered
*/
class GlMeshBuffer : boost::noncopyable
{
public:

    /**
    * Constructor
    * @param name The name of the mesh
    * @param vertices The vertex buffer
    * @param indices The index buffer
    */
    GlMeshBuffer(const std::string& name, 
                 const std::vector<float>& vertices,
                 const std::vector<unsigned int>& indices);

    /**
    * Destructor
    */
    virtual ~GlMeshBuffer();

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
    virtual bool Initialise();

    /**
    * Reloads the mesh
    * @return whether reloading was successful
    */
    bool Reload();

protected:

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
};

/**
* OpenGL MeshData with instances
*/
class GlMeshData : public GlMeshBuffer
{
public:

    /**
    * Constructor
    * @param mesh The mesh to use as a template
    * @param preRender Callback to prerender an instance
    */
    GlMeshData(const MeshData& mesh, PreRenderMesh preRender);

    /**
    * Constructor
    * @param mesh The mesh to use as a template
    * @param vertices The vertex buffer
    * @param indices The index buffer
    * @param preRender Callback to prerender an instance
    */
    GlMeshData(const MeshData& mesh,
               const std::vector<float>& vertices,
               const std::vector<unsigned int>& indices,
               PreRenderMesh preRender);

    /**
    * Renders the mesh
    */
    virtual void Render() override;

    /**
    * Initialises the mesh
    * @return whether initialisation succeeded
    */
    virtual bool Initialise() override;

    /**
    * @return the mesh element
    */
    const MeshData& GetData() const;

private:

    const MeshData& m_meshdata;           ///< Data for the mesh
    std::vector<glm::mat4> m_world;       ///< World matrices of the instances
    PreRenderMesh m_preRender = nullptr;  ///< Callback to render instances
    bool m_updateInstances = false;       ///< Whether a full transform update is required
};                                       

/**
* OpenGL mesh with instances
*/
class GlMesh : public GlMeshData
{
public:

    /**
    * Constructor
    * @param mesh The mesh to use as a template
    * @param preRender Callback to prerender an instance
    */
    GlMesh(const MeshData& mesh, PreRenderMesh preRender);

    /**
    * @return the mesh element
    */
    const Mesh& GetMesh() const;

    /**
    * @return the water information for the mesh
    */
    const Water& GetWater() const;

    /**
    * @return the water information for the mesh
    */
    const Terrain& GetTerrain() const;
};             

/**
* OpenGL quad data
*/
struct GlQuadData
{
public:

    /**
    * Constructor
    */
    GlQuadData();

    std::vector<float> vertices;        ///< Vertex information
    std::vector<unsigned int> indices;  ///< Index information
};

/**
* OpenGL instanced quad
*/
class GlQuadMesh : public GlQuadData, public GlMeshData
{
public:

    /**
    * Constructor
    * @param mesh The mesh to use as a template
    * @param preRender Callback to prerender an instance
    */
    GlQuadMesh(const MeshData& mesh, PreRenderMesh preRender);
};

/**
* OpenGL single quad
*/
class GlQuad : public GlQuadData, public GlMeshBuffer
{
public:

    /**
    * Constructor
    * @param name The name of the quad
    */
    GlQuad(const std::string& name);
};