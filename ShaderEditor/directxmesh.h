////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directxmesh.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "directxcommon.h"

struct Mesh;

/**
* DirectX Individual mesh
*/
class DxMesh : boost::noncopyable
{
public:

    /**
    * Constructor for a complex mesh
    * @param mesh The mesh to use as a template
    */
    DxMesh(const Mesh* mesh);

    /**
    * Constructor for a 2D screen quad
    * @param name The name of the mesh
    */
    DxMesh(const std::string& name);

    /**
    * Destructor
    */
    ~DxMesh();

    /**
    * Releases the directx mesh
    */
    void Release();

    /**
    * Renders the mesh
    * @param context Direct3D device context
    */
    void Render(ID3D11DeviceContext* context);

    /**
    * Initialises the mesh
    * @param device The DirectX device interface
    * @param context Direct3D device context
    */
    void Initialise(ID3D11Device* device, ID3D11DeviceContext* context);

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
    * @return Maximum textures used for this mesh
    */
    int GetMaxTextures() const;

private:

    UINT m_indexCount;              ///< Number of indices for the mesh
    UINT m_vertexCount;             ///< Number of vertices for the mesh
    UINT m_vertexStride;            ///< Size of the vertex structure
    ID3D11Buffer* m_vertexBuffer;   ///< Buffer of vertex data for the mesh
    ID3D11Buffer* m_indexBuffer;    ///< Buffer of index data for the mesh
    std::vector<float> m_vertices;  ///< Mesh Vertex information
    std::vector<DWORD> m_indices;   ///< Mesh Index information
    const Mesh* m_mesh;             ///< Mesh information or null if a quad
    std::string m_name;             ///< Name of the mesh
};                     