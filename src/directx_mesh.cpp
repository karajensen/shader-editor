////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directx_mesh.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "directx_mesh.h"
#include "logger.h"

DxMeshBuffer::DxMeshBuffer(const std::string& name,
                           const std::vector<float>& vertices,
                           const std::vector<unsigned int>& indices,
                           int vertexStride)
    : m_name(name)
    , m_vertices(vertices)
    , m_indices(indices)
    , m_vertexStride(vertexStride)
{
}

DxMeshData::DxMeshData(const MeshData& mesh, PreRenderMesh preRender)
    : DxMeshBuffer(mesh.Name(), mesh.Vertices(), mesh.Indices(), sizeof(float)* mesh.VertexComponentCount())
    , m_meshdata(mesh)
    , m_preRender(preRender)
{
}

DxMeshData::DxMeshData(const MeshData& mesh,
                       const std::vector<float>& vertices,
                       const std::vector<unsigned int>& indices,
                       int vertexStride,
                       PreRenderMesh preRender)
    : DxMeshBuffer(mesh.Name(), vertices, indices, vertexStride)
    , m_meshdata(mesh)
    , m_preRender(preRender)
{
}

DxMesh::DxMesh(const MeshData& mesh, PreRenderMesh preRender)
    : DxMeshData(mesh, preRender)
{
}

DxMeshBuffer::~DxMeshBuffer()
{
    Release();
}

void DxMeshBuffer::Release()
{
    SafeRelease(&m_vertexBuffer);
    SafeRelease(&m_indexBuffer);
}

bool DxMeshBuffer::FillBuffers(ID3D11DeviceContext* context)
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

void DxMeshData::Initialise(ID3D11Device* device, 
                            ID3D11DeviceContext* context)
{
    DxMeshBuffer::Initialise(device, context);

    m_world.clear();
    m_world.resize(m_meshdata.Instances().size());
    for (auto& world : m_world)
    {
        D3DXMatrixIdentity(&world);
    }
    m_updateInstances = true;
}

void DxMeshBuffer::Initialise(ID3D11Device* device, 
                              ID3D11DeviceContext* context)
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

bool DxMeshBuffer::Reload(ID3D11DeviceContext* context)
{
    return FillBuffers(context);
}

void DxMeshBuffer::Render(ID3D11DeviceContext* context)
{
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &m_vertexStride, &offset);
    context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
    context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    context->DrawIndexed(m_indices.size(), 0, 0);
}

void DxMeshData::Render(ID3D11DeviceContext* context)
{
    const auto& instances = m_meshdata.Instances();
    for (unsigned int i = 0; i < instances.size(); ++i)
    {
        const auto& instance = instances[i];
        if (instance.requiresUpdate || m_updateInstances)
        {
            m_world[i]._11 = instance.world.m11;
            m_world[i]._21 = instance.world.m12;
            m_world[i]._31 = instance.world.m13;
            
            m_world[i]._12 = instance.world.m21;
            m_world[i]._22 = instance.world.m22;
            m_world[i]._32 = instance.world.m23;
            
            m_world[i]._13 = instance.world.m31;
            m_world[i]._23 = instance.world.m32;
            m_world[i]._33 = instance.world.m33;
            
            m_world[i]._41 = instance.world.m14;
            m_world[i]._42 = instance.world.m24;
            m_world[i]._43 = instance.world.m34;
        }

        if (instance.enabled && instance.render)
        {
            m_preRender(m_world[i], instance.colour);
            DxMeshBuffer::Render(context);
        }
    }

    m_updateInstances = false;
}

const MeshData& DxMeshData::GetData() const
{
    return m_meshdata;
}

const Mesh& DxMesh::GetMesh() const
{
    return static_cast<const Mesh&>(GetData());
}

const Water& DxMesh::GetWater() const
{
    return static_cast<const Water&>(GetData());
}

const Terrain& DxMesh::GetTerrain() const
{
    return static_cast<const Terrain&>(GetData());
}

DxQuadData::DxQuadData()
{
    // Top left corner
    vertices.emplace_back(-1.0f); // x
    vertices.emplace_back(1.0f);  // y
    vertices.emplace_back(0.0f);  // z
    vertices.emplace_back(0.0f);  // u
    vertices.emplace_back(0.0f);  // v

    // Top right corner
    vertices.emplace_back(1.0f); // x
    vertices.emplace_back(1.0f); // y
    vertices.emplace_back(0.0f); // z
    vertices.emplace_back(1.0f); // u
    vertices.emplace_back(0.0f); // v

    // Bot right corner
    vertices.emplace_back(1.0f);  // x
    vertices.emplace_back(-1.0f); // y
    vertices.emplace_back(0.0f);  // z
    vertices.emplace_back(1.0f);  // u
    vertices.emplace_back(1.0f);  // v

    // Bot left corner
    vertices.emplace_back(-1.0f); // x
    vertices.emplace_back(-1.0f); // y
    vertices.emplace_back(0.0f);  // z
    vertices.emplace_back(0.0f);  // u
    vertices.emplace_back(1.0f);  // v

    indices.emplace_back(0);
    indices.emplace_back(3);
    indices.emplace_back(1);

    indices.emplace_back(1);
    indices.emplace_back(3);
    indices.emplace_back(2);

    // 3 floats in position, 2 floats in uvs
    stride = sizeof(float) * 5;
}

DxQuad::DxQuad(const std::string& name) :
    DxMeshBuffer(name, vertices, indices, stride)
{
}

DxQuadMesh::DxQuadMesh(const MeshData& mesh, PreRenderMesh preRender) :
    DxMeshData(mesh, vertices, indices, stride, preRender)
{
}