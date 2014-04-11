////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directxmesh.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "directxcommon.h"
struct Mesh;

/**
* DirectX Individual mesh
*/
class DxMesh
{
public:

    /**
    * Constructor
    * @param mesh The mesh to use as a template
    */
    DxMesh(const Mesh& mesh);

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
    * @param device Direct3D device interface
    * @param context Direct3D device context
    */
    void Initialise(ID3D11Device* device, ID3D11DeviceContext* context);

private:

    const Mesh& m_mesh;             ///< Mesh template for this directxmesh
    UINT m_vertexCount;             ///< Number of vertices for the mesh
    UINT m_vertexStride;            ///< Size of the vertex structure
    ID3D11Buffer* m_vertexBuffer;   ///< Buffer of vertex data for the mesh
};                     