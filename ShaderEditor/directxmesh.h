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
    void Render(ID3D11DeviceContext* context);

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
    */
    DxQuad(const std::string& name);
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
    */
    DxMesh(const Mesh& mesh);

    /**
    * @return the unique ID for the mesh shader
    */
    int GetShaderID() const;

    /**
    * @return the mesh element
    */
    const Mesh& GetMesh() const;

private:

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
    */
    DxWater(const Water& water);

    /**
    * @return the water information for the mesh
    */
    const Water& GetWater() const;

private:

    const Water& m_water; ///< Water information
};
                                                                             