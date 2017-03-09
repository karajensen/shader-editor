////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directx_mesh.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "directx_common.h"

/**
* Base data for any polygons to be rendered
*/
class DxMeshBuffer : boost::noncopyable
{
public:

    /**
    * Constructor
    * @param name The name of the mesh
    * @param vertices The vertex buffer
    * @param indices The index buffer
    * @param vertexStride Size of the vertex structure
    */
    DxMeshBuffer(const std::string& name, 
                 const std::vector<float>& vertices,
                 const std::vector<unsigned int>& indices,
                 int vertexStride);

    /**
    * Destructor
    */
    virtual ~DxMeshBuffer();

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
    virtual void Initialise(ID3D11Device* device, 
                            ID3D11DeviceContext* context);

    /**
    * Reloads the mesh
    * @param context The direct3D context
    * @return whether reloading was successful
    */
    bool Reload(ID3D11DeviceContext* context);

protected:

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
};

/**
* DirectX MeshData with instances
*/
class DxMeshData : public DxMeshBuffer
{
public:

    /**
    * Constructor
    * @param mesh The mesh to use as a template
    * @param preRender Callback to prerender an instance
    */
    DxMeshData(const MeshData& mesh, PreRenderMesh preRender);

    /**
    * Constructor
    * @param mesh The mesh to use as a template
    * @param vertices The vertex buffer
    * @param indices The index buffer
    * @param vertexStride Size of the vertex structure
    * @param preRender Callback to prerender an instance
    */
    DxMeshData(const MeshData& mesh,
               const std::vector<float>& vertices,
               const std::vector<unsigned int>& indices,
               int vertexStride,
               PreRenderMesh preRender);

    /**
    * @return the mesh element
    */
    const MeshData& GetData() const;

    /**
    * Renders the data
    * @param context Direct3D device context
    */
    virtual void Render(ID3D11DeviceContext* context) override;

    /**
    * Initialises the data
    * @param device The DirectX device interface
    * @param context Direct3D device context
    */
    virtual void Initialise(ID3D11Device* device, 
                            ID3D11DeviceContext* context) override;
private:

    const MeshData& m_meshdata;             ///< Mesh information
    std::vector<D3DXMATRIX> m_world;        ///< World matrices of the instances
    PreRenderMesh m_preRender = nullptr;    ///< Callback to render a single mesh instance
    bool m_updateInstances = false;         ///< Whether a full transform update is required
};

/**
* DirectX mesh with instances
*/
class DxMesh : public DxMeshData
{
public:

    /**
    * Constructor
    * @param mesh The mesh to use as a template
    * @param preRender Callback to prerender an instance
    */
    DxMesh(const MeshData& mesh, PreRenderMesh preRender);

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
* DirectX quad data
*/
struct DxQuadData
{
public:

    /**
    * Constructor
    */
    DxQuadData();

    int stride;                         ///< Size of the vertex structure
    std::vector<float> vertices;        ///< Vertex information
    std::vector<unsigned int> indices;  ///< Index information
};

/**
* DirectX instanced quad
*/
class DxQuadMesh : public DxQuadData, public DxMeshData
{
public:

    /**
    * Constructor
    * @param mesh The mesh to use as a template
    * @param preRender Callback to prerender an instance
    */
    DxQuadMesh(const MeshData& mesh, PreRenderMesh preRender);
};

/**
* DirectX single quad
*/
class DxQuad : public DxQuadData, public DxMeshBuffer
{
public:

    /**
    * Constructor
    * @param name The name of the quad
    */
    DxQuad(const std::string& name);
};