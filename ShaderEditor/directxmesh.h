////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directxmesh.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "directxcommon.h"

/**
* Base data for any polygons to be rendered
*/
class DxMeshData : boost::noncopyable
{
public:

    /**
    * Constructor for predefined buffers
    * @param data Information for the mesh buffers
    * @param preRender Callback to prerender an instance
    */
    DxMeshData(const MeshData& data,
               PreRenderMesh preRender);

    /**
    * Constructor for empty buffers
    * @param name The name of the mesh
    * @param vertices The vertex buffer
    * @param indices The index buffer
    */
    DxMeshData(const std::string& name, 
               const std::vector<float>& vertices,
               const std::vector<unsigned int>& indices);

    /**
    * Destructor
    */
    virtual ~DxMeshData();

    /**
    * Releases the data
    */
    void Release();

    /**
    * Renders the data
    * @param context Direct3D device context
    */
    virtual void Render(ID3D11DeviceContext* context);

    /**
    * Initialises the data
    * @param device The DirectX device interface
    * @param context Direct3D device context
    */
    void Initialise(ID3D11Device* device, ID3D11DeviceContext* context);

protected:

    /**
    * Renders the data for each instance
    * @param context Direct3D device context
    * @param instances The mesh instances to render
    */
    void RenderInstances(ID3D11DeviceContext* context,
                         const std::vector<MeshData::Instance>& instances);

    /**
    * Fills the vertex and index buffers
    * @param context Direct3D device context
    * @return whether call was successful
    */
    bool FillBuffers(ID3D11DeviceContext* context);

    UINT m_vertexStride = 0;                    ///< Size of the vertex structure
    ID3D11Buffer* m_vertexBuffer = nullptr;     ///< Buffer of vertex data for the mesh
    ID3D11Buffer* m_indexBuffer = nullptr;      ///< Buffer of index data for the mesh
    std::string m_name;                         ///< Name of the mesh
    const std::vector<float>& m_vertices;       ///< Vertex buffer data
    const std::vector<unsigned int>& m_indices; ///< Index buffer data
    std::vector<D3DXMATRIX> m_world;            ///< World matrices of the instances
    PreRenderMesh m_preRender = nullptr;        ///< Callback to render a single mesh instance
};

/**
* DirectX screen quad
*/
class DxQuad : public DxMeshData
{
public:

    /**
    * Constructor for a 2D screen quad
    * @param name The name of the mesh
    */
    DxQuad(const std::string& name);

private:

    std::vector<float> m_vertices;        ///< Vertex information
    std::vector<unsigned int> m_indices;  ///< Index information
};

/**
* DirectX complex mesh
*/
class DxMesh : public DxMeshData
{
public:

    /**
    * Constructor for a complex mesh
    * @param mesh The mesh to use as a template
    * @param preRender Callback to prerender an instance
    */
    DxMesh(const Mesh& mesh, PreRenderMesh preRender);

    /**
    * @return the mesh element
    */
    const Mesh& GetMesh() const;

    /**
    * Renders the data
    * @param context Direct3D device context
    */
    virtual void Render(ID3D11DeviceContext* context) override;

private:

    const Mesh& m_mesh;               ///< Mesh information
};                                           

/**
* DirectX Water mesh
*/
class DxWater : public DxMeshData
{
public:

    /**
    * Constructor for a complex mesh
    * @param mesh The mesh to use as a template
    * @param preRender Callback to prerender an instance
    */
    DxWater(const Water& water, PreRenderMesh preRender);

    /**
    * @return the water information for the mesh
    */
    const Water& GetWater() const;

    /**
    * Renders the data
    * @param context Direct3D device context
    */
    virtual void Render(ID3D11DeviceContext* context) override;

private:

    const Water& m_water; ///< Water information
};

/**
* DirectX Terrain mesh
*/
class DxTerrain : public DxMeshData
{
public:

    /**
    * Constructor for a complex mesh
    * @param mesh The mesh to use as a template
    * @param preRender Callback to prerender an instance
    */
    DxTerrain(const Terrain& terrain, PreRenderMesh preRender);

    /**
    * @return the water information for the mesh
    */
    const Terrain& GetTerrain() const;

    /**
    * Renders the data
    * @param context Direct3D device context
    */
    virtual void Render(ID3D11DeviceContext* context) override;

    /**
    * Reloads the terrain
    * @param context The direct3D context
    * @return whether reloading was successful
    */
    bool Reload(ID3D11DeviceContext* context);

private:

    const Terrain& m_terrain; ///< Terrain information
};
                                                                             