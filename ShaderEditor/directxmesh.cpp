////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directxmesh.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "directxmesh.h"

DxMeshData::DxMeshData(const std::string& name) :
    m_name(name)
{
}

DxWater::DxWater(const Water& water) :
    DxMeshData(water.Name()),
    m_water(water)
{
    m_vertexStride = sizeof(float) * water.VertexComponentCount();
    m_vertexCount = water.Vertices().size();
    m_indexCount = water.Indices().size();
    m_vertices = water.Vertices();
    m_indices = water.Indices();
}

DxMesh::DxMesh(const Mesh& mesh, PreRenderMesh preRender) :
    DxMeshData(mesh.Name()),
    m_mesh(mesh),
    m_preRender(preRender)
{
    m_vertexStride = sizeof(float) * mesh.VertexComponentCount();
    m_vertexCount = mesh.Vertices().size();
    m_indexCount = mesh.Indices().size();
    m_vertices = mesh.Vertices();
    m_indices = mesh.Indices();
}

DxQuad::DxQuad(const std::string& name) :
    DxMeshData(name)
{
    // Top left corner
    m_vertices.emplace_back(-1.0f); // x
    m_vertices.emplace_back(1.0f);  // y
    m_vertices.emplace_back(0.0f);  // z
    m_vertices.emplace_back(0.0f);  // u
    m_vertices.emplace_back(0.0f);  // v

    // Top right corner
    m_vertices.emplace_back(1.0f); // x
    m_vertices.emplace_back(1.0f); // y
    m_vertices.emplace_back(0.0f); // z
    m_vertices.emplace_back(1.0f); // u
    m_vertices.emplace_back(0.0f); // v

    // Bot right corner
    m_vertices.emplace_back(1.0f);  // x
    m_vertices.emplace_back(-1.0f); // y
    m_vertices.emplace_back(0.0f);  // z
    m_vertices.emplace_back(1.0f);  // u
    m_vertices.emplace_back(1.0f);  // v

    // Bot left corner
    m_vertices.emplace_back(-1.0f); // x
    m_vertices.emplace_back(-1.0f); // y
    m_vertices.emplace_back(0.0f);  // z
    m_vertices.emplace_back(0.0f);  // u
    m_vertices.emplace_back(1.0f);  // v

    m_indices.emplace_back(0);
    m_indices.emplace_back(3);
    m_indices.emplace_back(1);

    m_indices.emplace_back(1);
    m_indices.emplace_back(3);
    m_indices.emplace_back(2);

    // 3 floats in position, 2 floats in uvs
    m_vertexStride = sizeof(float) * 5;
    m_vertexCount = m_vertices.size();
    m_indexCount = m_indices.size();
}

DxMeshData::~DxMeshData()
{
    Release();
}

void DxMeshData::Release()
{
    SafeRelease(&m_vertexBuffer);
    SafeRelease(&m_indexBuffer);
}

void DxMeshData::Initialise(ID3D11Device* device, ID3D11DeviceContext* context)
{
    // Create the vertex buffer
    D3D11_BUFFER_DESC vbd;
    ZeroMemory(&vbd, sizeof(vbd));
    vbd.Usage = D3D11_USAGE_DYNAMIC;
    vbd.ByteWidth = m_vertexStride * m_vertexCount;
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    device->CreateBuffer(&vbd, 0, &m_vertexBuffer);

    // Copy the mesh vertices to the directx mesh
    D3D11_MAPPED_SUBRESOURCE vms;
    context->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &vms);
    memcpy(vms.pData, &m_vertices[0], sizeof(float)*m_vertices.size());
    context->Unmap(m_vertexBuffer, 0); 
    
    // Create the index buffer
    D3D11_BUFFER_DESC ibd;
    ibd.Usage = D3D11_USAGE_DYNAMIC;
    ibd.ByteWidth = sizeof(DWORD) * m_indexCount;
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    ibd.MiscFlags = 0;
    device->CreateBuffer(&ibd, 0, &m_indexBuffer);

    // Copy the mesh indicies to the directx mesh
    D3D11_MAPPED_SUBRESOURCE ims;
    context->Map(m_indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ims); 
    memcpy(ims.pData, &m_indices[0], sizeof(DWORD)*m_indices.size());
    context->Unmap(m_indexBuffer, 0);

    SetDebugName(m_indexBuffer, m_name + "_IndexBuffer");
    SetDebugName(m_vertexBuffer, m_name + "_VertexBuffer");
}

void DxMeshData::Render(ID3D11DeviceContext* context)
{
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &m_vertexStride, &offset);
    context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
    context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    context->DrawIndexed(m_indexCount, 0, 0);
}

const Mesh& DxMesh::GetMesh() const
{
    return m_mesh;
}

const Water& DxWater::GetWater() const
{
    return m_water;
}

void DxMesh::Render(ID3D11DeviceContext* context)
{
    for (const Mesh::Instance& instance : m_mesh.Instances())
    {
        if (instance.shouldRender)
        {
            D3DXMATRIX world;
            D3DXMatrixIdentity(&world);  

            world._11 = instance.scale;
            world._22 = instance.scale;
            world._33 = instance.scale;

            world._41 = instance.position.x;
            world._42 = instance.position.y;
            world._43 = instance.position.z;

            m_preRender(world, instance.colour);
            DxMeshData::Render(context);
        }
    }
}