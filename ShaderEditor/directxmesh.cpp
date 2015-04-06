////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directxmesh.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "directxmesh.h"

DxMeshData::DxMeshData(const MeshData& data, PreRenderMesh preRender) :
    m_name(data.Name()),
    m_vertexStride(sizeof(float) * data.VertexComponentCount()),
    m_vertices(data.Vertices()),
    m_indices(data.Indices()),
    m_preRender(preRender)
{
}

DxMeshData::DxMeshData(const std::string& name,
                       const std::vector<float>& vertices,
                       const std::vector<unsigned int>& indices) :
    m_name(name),
    m_vertices(vertices),
    m_indices(indices)
{
}

DxWater::DxWater(const Water& water, PreRenderMesh preRender) :
    DxMeshData(water, preRender),
    m_water(water)
{
}

DxTerrain::DxTerrain(const Terrain& terrain, PreRenderMesh preRender) :
    DxMeshData(terrain, preRender),
    m_terrain(terrain)
{
}

DxMesh::DxMesh(const Mesh& mesh, PreRenderMesh preRender) :
    DxMeshData(mesh, preRender),
    m_mesh(mesh)
{
}

DxQuad::DxQuad(const std::string& name) :
    DxMeshData(name, m_vertices, m_indices)
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

bool DxMeshData::FillBuffers(ID3D11DeviceContext* context)
{
    // Copy the mesh vertices to the directx mesh
    D3D11_MAPPED_SUBRESOURCE vms;
    if (FAILED(context->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &vms)))
    {
        return false;
    }
    memcpy(vms.pData, &m_vertices[0], sizeof(float)*m_vertices.size());
    context->Unmap(m_vertexBuffer, 0); 
    
    // Copy the mesh indicies to the directx mesh
    D3D11_MAPPED_SUBRESOURCE ims;
    if(FAILED(context->Map(m_indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ims)))
    {
        return false;
    }
    memcpy(ims.pData, &m_indices[0], sizeof(DWORD)*m_indices.size());
    context->Unmap(m_indexBuffer, 0);

    return true;
}

void DxMeshData::Initialise(ID3D11Device* device, ID3D11DeviceContext* context)
{
    // Create the vertex buffer
    D3D11_BUFFER_DESC vbd;
    ZeroMemory(&vbd, sizeof(vbd));
    vbd.Usage = D3D11_USAGE_DYNAMIC;
    vbd.ByteWidth = m_vertexStride * m_vertices.size();
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    device->CreateBuffer(&vbd, 0, &m_vertexBuffer);
    SetDebugName(m_vertexBuffer, m_name + "_VertexBuffer");

    // Create the index buffer
    D3D11_BUFFER_DESC ibd;
    ibd.Usage = D3D11_USAGE_DYNAMIC;
    ibd.ByteWidth = sizeof(DWORD) * m_indices.size();
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    ibd.MiscFlags = 0;
    device->CreateBuffer(&ibd, 0, &m_indexBuffer);
    SetDebugName(m_indexBuffer, m_name + "_IndexBuffer");

    if (!FillBuffers(context))
    {
        Logger::LogError("DirectX: Could not load mesh buffers");
    }
}

const Mesh& DxMesh::GetMesh() const
{
    return m_mesh;
}

const Water& DxWater::GetWater() const
{
    return m_water;
}

const Terrain& DxTerrain::GetTerrain() const
{
    return m_terrain;
}

bool DxTerrain::Reload(ID3D11DeviceContext* context)
{
    return FillBuffers(context);
}

void DxMeshData::Render(ID3D11DeviceContext* context)
{
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &m_vertexStride, &offset);
    context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
    context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    context->DrawIndexed(m_indices.size(), 0, 0);
}

void DxMesh::Render(ID3D11DeviceContext* context)
{
    RenderInstances(context, m_mesh.Instances());
}

void DxTerrain::Render(ID3D11DeviceContext* context)
{
    RenderInstances(context, m_terrain.Instances());
}

void DxWater::Render(ID3D11DeviceContext* context)
{
    RenderInstances(context, m_water.Instances());
}

void DxMeshData::RenderInstances(ID3D11DeviceContext* context,
                                 const std::vector<MeshData::Instance>& instances)
{
    for (const Mesh::Instance& instance : instances)
    {
        if (instance.shouldRender)
        {
            D3DXMATRIX scale;
            D3DXMatrixIdentity(&scale);
            scale._11 = instance.scale.x;
            scale._22 = instance.scale.y;
            scale._33 = instance.scale.z;

            D3DXMATRIX translate;
            D3DXMatrixIdentity(&translate);
            translate._41 = instance.position.x;
            translate._42 = instance.position.y;
            translate._43 = instance.position.z;

            D3DXMATRIX rotate;
            D3DXMatrixIdentity(&rotate);
            if (!instance.rotation.IsZero())
            {
                D3DXMATRIX rotateX, rotateY, rotateZ;
                D3DXMatrixRotationX(&rotateX, DegToRad(instance.rotation.x)); 
                D3DXMatrixRotationY(&rotateY, DegToRad(instance.rotation.y)); 
                D3DXMatrixRotationZ(&rotateZ, DegToRad(instance.rotation.z)); 
                rotate = rotateZ * rotateX * rotateY;
            }
            
            m_preRender(scale * rotate * translate);
            Render(context);
        }
    }
}