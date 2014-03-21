////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directxmesh.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "directxmesh.h"

DxMesh::DxMesh(ID3D11Device* device, ID3D11DeviceContext* context, const Mesh& mesh) :
    m_vertexBuffer(nullptr),
    m_vertexStride(0),
    m_vertexCount(0)
{
    struct Vertex
    {
        FLOAT X, Y, Z; 
        D3DXCOLOR Color;
    };
    m_vertexStride = sizeof(Vertex);

    // Copy the mesh vertices to a format DirectX can map
    m_vertexCount = mesh.position.size();
    Vertex* vertices = new Vertex[m_vertexCount];
    for(unsigned int i = 0; i < m_vertexCount; ++i)
    {
        vertices[i].X = mesh.position[i].x;
        vertices[i].Y = mesh.position[i].y;
        vertices[i].Z = mesh.position[i].z;

        vertices[i].Color.r = 1.0f;
        vertices[i].Color.g = 1.0f;
        vertices[i].Color.b = 1.0f;
        vertices[i].Color.a = 1.0f;
    }

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

    delete [] vertices;
}

DxMesh::~DxMesh()
{
}

void DxMesh::Render(ID3D11DeviceContext* context)
{
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &m_vertexStride, &offset);
    context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    context->Draw(m_vertexCount, 0);
}