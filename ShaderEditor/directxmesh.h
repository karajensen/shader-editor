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

    UINT m_indexCount = 0;                   ///< Number of indices for the mesh
    UINT m_vertexCount = 0;                  ///< Number of vertices for the mesh
    UINT m_vertexStride = 0;                 ///< Size of the vertex structure
    ID3D11Buffer* m_vertexBuffer = nullptr;  ///< Buffer of vertex data for the mesh
    ID3D11Buffer* m_indexBuffer = nullptr;   ///< Buffer of index data for the mesh
    std::vector<float> m_vertices;           ///< Mesh Vertex information
    std::vector<DWORD> m_indices;            ///< Mesh Index information
    std::string m_name;                      ///< Name of the mesh
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
    * @param preRender a callback to setup rendering
    */
    DxQuad(const std::string& name, PreRenderQuad preRender = nullptr);

    /**
    * Renders the data
    * @param context Direct3D device context
    */
    virtual void Render(ID3D11DeviceContext* context) override;

    /**
    * Sets the id of the texture used
    */
    void SetTexture(int ID);

private:

    int m_texture = NO_INDEX;            ///< Index for the quad texture
    PreRenderQuad m_preRender = nullptr; ///< Callback to pre-render the quad
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
    * @param preRender a callback to setup rendering
    */
    DxMesh(const Mesh& mesh, PreRenderMesh preRender);

    /**
    * Renders the data
    * @param context Direct3D device context
    */
    virtual void Render(ID3D11DeviceContext* context) override;

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
* DirectX Water mesh
*/
class DxWater : public DxMesh
{
public:

    /**
    * Constructor for a complex mesh
    * @param mesh The mesh to use as a template
    * @param preRender a callback to setup rendering
    */
    DxWater(const Water& water, PreRenderMesh preRender);

    /**
    * @return the water information for the mesh
    */
    const Water& GetWater() const;

private:

    const Water& m_water; ///< Water information
};
                                                                             