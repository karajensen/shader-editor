////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directxmesh.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "directxcommon.h"
#include "elements.h"

/**
* DirectX Individual mesh
*/
class DxMesh
{
public:

    /**
    * Constructor
    * @param device Direct3D device interface
    * @param context Direct3D device context
    * @param mesh The mesh object to use as a template
    */
    DxMesh(ID3D11Device* device, ID3D11DeviceContext* context, const Mesh& mesh);

    /**
    * Destructor
    */
    ~DxMesh();

    /**
    * Renders the mesh
    * @param context Direct3D device context
    */
    void Render(ID3D11DeviceContext* context);

private:

    UINT m_vertexCount;             ///< Number of vertices for the mesh
    UINT m_vertexStride;            ///< Size of the vertex structure
    ID3D11Buffer* m_vertexBuffer;   ///< Buffer of vertex data for the mesh
};                     