////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directxmesh.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "directxmesh.h"
#include "elements.h"

DxWater::DxWater(const Water* water) :
    DxMesh(water),
    m_water(water)
{

}

DxMesh::DxMesh(const Mesh* mesh) :
    m_vertexStride(sizeof(float) * mesh->vertexComponentCount),
    m_vertexCount(mesh->vertexCount),
    m_indexCount(mesh->indexCount),
    m_mesh(mesh),
    m_vertices(mesh->vertices),
    m_indices(mesh->indices),
    m_name(mesh->name)
{
}

DxMesh::DxMesh(const std::string& name) :
    m_vertexBuffer(nullptr),
    m_indexBuffer(nullptr),
    m_name(name)
{
    // Top left corner
    m_vertices.emplace_back(-1.0); // x
    m_vertices.emplace_back(1.0);  // y
    m_vertices.emplace_back(0.0);  // z
    m_vertices.emplace_back(0.0);  // u
    m_vertices.emplace_back(0.0);  // v

    // Top right corner
    m_vertices.emplace_back(1.0); // x
    m_vertices.emplace_back(1.0); // y
    m_vertices.emplace_back(0.0); // z
    m_vertices.emplace_back(1.0); // u
    m_vertices.emplace_back(0.0); // v

    // Bot right corner
    m_vertices.emplace_back(1.0);  // x
    m_vertices.emplace_back(-1.0); // y
    m_vertices.emplace_back(0.0);  // z
    m_vertices.emplace_back(1.0);  // u
    m_vertices.emplace_back(1.0);  // v

    // Bot left corner
    m_vertices.emplace_back(-1.0); // x
    m_vertices.emplace_back(-1.0); // y
    m_vertices.emplace_back(0.0);  // z
    m_vertices.emplace_back(0.0);  // u
    m_vertices.emplace_back(1.0);  // v

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

DxMesh::~DxMesh()
{
    Release();
}

void DxMesh::Release()
{
    SafeRelease(&m_vertexBuffer);
    SafeRelease(&m_indexBuffer);
}

void DxMesh::Initialise(ID3D11Device* device, ID3D11DeviceContext* context)
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

void DxMesh::Render(ID3D11DeviceContext* context)
{
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &m_vertexStride, &offset);
    context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
    context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    context->DrawIndexed(m_indexCount, 0, 0);
}

bool DxMesh::ShouldBackfaceCull() const
{
    assert(m_mesh);
    return m_mesh->backfacecull;
}

const std::vector<int>& DxMesh::GetTextureIDs() const
{
    assert(m_mesh);
    return m_mesh->textureIDs;
}

int DxMesh::GetShaderID() const 
{ 
    assert(m_mesh);
    return m_mesh->shaderIndex;
}

int DxMesh::GetMaxTextures() const 
{ 
    assert(m_mesh);
    return m_mesh->maxTextures; 
}

const Mesh& DxMesh::GetMesh() const
{
    assert(m_mesh);
    return *m_mesh;
}

const Water& DxWater::GetWater() const
{
    assert(m_water);
    return *m_water;
}