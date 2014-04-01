////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directxmesh.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "directxmesh.h"

DxMesh::DxMesh() :
    m_vertexBuffer(nullptr),
    m_vertexStride(0),
    m_vertexCount(0)
{
}

DxMesh::~DxMesh()
{
    Release();
}

void DxMesh::Release()
{
    if(m_vertexBuffer)
    {
        m_vertexBuffer->Release();
        m_vertexBuffer = nullptr;
    }
}

void DxMesh::Initialise(ID3D11Device* device, ID3D11DeviceContext* context)
{
    //////////////////////////////TO CUSTOMISE
    struct VERTEX
    {
        FLOAT X, Y, Z; 
        D3DXCOLOR Color;
    };

    m_vertexStride = sizeof(VERTEX);
    m_vertexCount = 3;

    VERTEX vertices[] =
    {
        {0.0f, 0.5f, -5.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)},
        {0.45f, -0.5, -5.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f)},
        {-0.45f, -0.5f, -5.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f)}
    };
    //////////////////////////////

    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = m_vertexStride * m_vertexCount;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    device->CreateBuffer(&bd, 0, &m_vertexBuffer);

    // Copy the mesh vertices to the directx mesh
    D3D11_MAPPED_SUBRESOURCE ms;
    context->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
    memcpy(ms.pData, vertices, sizeof(vertices));
    context->Unmap(m_vertexBuffer, 0); 
}

void DxMesh::Render(ID3D11DeviceContext* context)
{
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &m_vertexStride, &offset);
    context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    context->Draw(m_vertexCount, 0);
}