////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkan_mesh.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "vulkan_mesh.h"
#include "vulkan_utils.h"
#include "vulkan_data.h"
#include "terrain.h"
#include "water.h"
#include "mesh.h"

VkMeshBuffer::VkMeshBuffer(VulkanData& info, 
                           const std::string& name,
                           const std::vector<float>& vertices,
                           const std::vector<unsigned int>& indices) :
    m_name(name),
    m_vertices(vertices),
    m_indices(indices),
    m_info(info)
{
    m_vertexMemory = VK_NULL_HANDLE;
    m_vertexBuffer = VK_NULL_HANDLE;
    m_indexMemory = VK_NULL_HANDLE;
    m_indexBuffer = VK_NULL_HANDLE;
    m_indexCount = 0;
}

VkMeshData::VkMeshData(VulkanData& info, 
                       const MeshData& data, 
                       PreRenderMesh preRender) :
    VkMeshBuffer(info, data.Name(), data.Vertices(), data.Indices()),
    m_meshdata(data),
    m_preRender(preRender)
{
}

VkMeshData::VkMeshData(VulkanData& info, 
                       const MeshData& mesh,
                       const std::vector<float>& vertices,
                       const std::vector<unsigned int>& indices,
                       PreRenderMesh preRender) :

    VkMeshBuffer(info, mesh.Name(), vertices, indices),
    m_meshdata(mesh),
    m_preRender(preRender)
{
}

VkMesh::VkMesh(VulkanData& info, 
               const MeshData& mesh,
               PreRenderMesh preRender) :
    VkMeshData(info, mesh, preRender)
{
}

VkMeshBuffer::~VkMeshBuffer()
{
    Release();
}

VkQuadData::VkQuadData()
{
    // Top left corner
    vertices.emplace_back(-1.0f); // x
    vertices.emplace_back(-1.0f); // y
    vertices.emplace_back(0.0f);  // z
    vertices.emplace_back(0.0f);  // u
    vertices.emplace_back(0.0f);  // v

    // Top right corner
    vertices.emplace_back(1.0f);  // x
    vertices.emplace_back(-1.0f); // y
    vertices.emplace_back(0.0f);  // z
    vertices.emplace_back(1.0f);  // u
    vertices.emplace_back(0.0f);  // v

    // Bot right corner
    vertices.emplace_back(1.0f);  // x
    vertices.emplace_back(1.0f);  // y
    vertices.emplace_back(0.0f);  // z
    vertices.emplace_back(1.0f);  // u
    vertices.emplace_back(1.0f);  // v

    // Bot left corner
    vertices.emplace_back(-1.0f); // x
    vertices.emplace_back(1.0f);  // y
    vertices.emplace_back(0.0f);  // z
    vertices.emplace_back(0.0f);  // u
    vertices.emplace_back(1.0f);  // v

    indices.emplace_back(0);
    indices.emplace_back(3);
    indices.emplace_back(1);

    indices.emplace_back(1);
    indices.emplace_back(3);
    indices.emplace_back(2);
}

void VkMeshBuffer::Release()
{
    if (m_vertexBuffer != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(m_info.device, m_vertexBuffer, NULL);
    }

    if (m_vertexMemory != VK_NULL_HANDLE)
    {
        vkFreeMemory(m_info.device, m_vertexMemory, NULL);
    }

    if (m_indexBuffer != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(m_info.device, m_indexBuffer, NULL);
    }

    if (m_indexMemory != VK_NULL_HANDLE)
    {
        vkFreeMemory(m_info.device, m_indexMemory, NULL);
    }

    m_vertexMemory = VK_NULL_HANDLE;
    m_vertexBuffer = VK_NULL_HANDLE;
    m_indexMemory = VK_NULL_HANDLE;
    m_indexBuffer = VK_NULL_HANDLE;
    m_indexCount = 0;
}

bool VkMeshData::Initialise()
{
    m_updateInstances = true;
    m_world.clear();
    m_world.resize(m_meshdata.Instances().size());
    return VkMeshBuffer::Initialise();
}

bool VkMeshBuffer::Initialise()
{
    uint32_t vertexBufferSize = static_cast<uint32_t>(m_vertices.size()) * sizeof(float);

    m_indexCount = static_cast<uint32_t>(m_indices.size());
    uint32_t indexBufferSize = m_indexCount * sizeof(unsigned int);

    VkMemoryAllocateInfo memAlloc = {};
    memAlloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    VkMemoryRequirements memReqs;
    void* data = nullptr;

    VkBufferCreateInfo vertexBufferInfo = {};
    vertexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    vertexBufferInfo.size = vertexBufferSize;
    vertexBufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

    // Copy vertex data to a buffer visible to the host
    if (CHECK_FAIL(vkCreateBuffer(m_info.device, &vertexBufferInfo, nullptr, &m_vertexBuffer)))
    {
        return false;
    }

    vkGetBufferMemoryRequirements(m_info.device, m_vertexBuffer, &memReqs);
    memAlloc.allocationSize = memReqs.size;

    if (CHECK_FAIL(VulkanUtils::MemoryTypeFromProperties(m_info, memReqs.memoryTypeBits,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
        &memAlloc.memoryTypeIndex)))
    {
        return false;
    }

    if (CHECK_FAIL(vkAllocateMemory(m_info.device, &memAlloc, nullptr, &m_vertexMemory)))
    {
        return false;
    }

    if (CHECK_FAIL(vkMapMemory(m_info.device, m_vertexMemory, 0, memAlloc.allocationSize, 0, &data)))
    {
        return false;
    }

    memcpy(data, m_vertices.data(), vertexBufferSize);
    vkUnmapMemory(m_info.device, m_vertexMemory);
    if (CHECK_FAIL(vkBindBufferMemory(m_info.device, m_vertexBuffer, m_vertexMemory, 0)))
    {
        return false;
    }

    VkBufferCreateInfo indexbufferInfo = {};
    indexbufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    indexbufferInfo.size = indexBufferSize;
    indexbufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;

    // Copy index data to a buffer visible to the host
    if (CHECK_FAIL(vkCreateBuffer(m_info.device, &indexbufferInfo, nullptr, &m_indexBuffer)))
    {
        return false;
    }

    vkGetBufferMemoryRequirements(m_info.device, m_indexBuffer, &memReqs);
    memAlloc.allocationSize = memReqs.size;

    if (CHECK_FAIL(VulkanUtils::MemoryTypeFromProperties(m_info, memReqs.memoryTypeBits,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
        &memAlloc.memoryTypeIndex)))
    {
        return false;
    }

    if (CHECK_FAIL(vkAllocateMemory(m_info.device, &memAlloc, nullptr, &m_indexMemory)))
    {
        return false;
    }

    if (CHECK_FAIL(vkMapMemory(m_info.device, m_indexMemory, 0, indexBufferSize, 0, &data)))
    {
        return false;
    }

    memcpy(data, m_indices.data(), indexBufferSize);
    vkUnmapMemory(m_info.device, m_indexMemory);
    if (CHECK_FAIL(vkBindBufferMemory(m_info.device, m_indexBuffer, m_indexMemory, 0)))
    {
        return false;
    }

    return true;
}

void VkMeshBuffer::Render(VkCommandBuffer cmd)
{
    VkDeviceSize offsets[1] = { 0 };
    vkCmdBindVertexBuffers(cmd, 0, 1, &m_vertexBuffer, offsets);
    vkCmdBindIndexBuffer(cmd, m_indexBuffer, 0, VK_INDEX_TYPE_UINT32);
    vkCmdDrawIndexed(cmd, m_indexCount, 1, 0, 0, 1);
}

const MeshData& VkMeshData::GetData() const
{
    return m_meshdata;
}

const Mesh& VkMesh::GetMesh() const
{
    return static_cast<const Mesh&>(GetData());
}

const Water& VkMesh::GetWater() const
{
    return static_cast<const Water&>(GetData());
}

const Terrain& VkMesh::GetTerrain() const
{
    return static_cast<const Terrain&>(GetData());
}

void VkMeshData::Render(VkCommandBuffer cmd)
{
    const auto& instances = m_meshdata.Instances();
    for (unsigned int i = 0; i < instances.size(); ++i)
    {
        const auto& instance = instances[i];
        if (instance.requiresUpdate || m_updateInstances)
        {
            m_world[i][0][0] = instance.world.m11;
            m_world[i][1][0] = instance.world.m12;
            m_world[i][2][0] = instance.world.m13;
            m_world[i][3][0] = instance.world.m14;

            m_world[i][0][1] = instance.world.m21;
            m_world[i][1][1] = instance.world.m22;
            m_world[i][2][1] = instance.world.m23;
            m_world[i][3][1] = instance.world.m24;

            m_world[i][0][2] = instance.world.m31;
            m_world[i][1][2] = instance.world.m32;
            m_world[i][2][2] = instance.world.m33;
            m_world[i][3][2] = instance.world.m34;
        }

        if (instance.enabled && instance.render)
        {
            m_preRender(cmd, m_world[i], instance.colour);
            VkMeshBuffer::Render(cmd);
        }
    }
    m_updateInstances = false;
}

VkQuad::VkQuad(VulkanData& info, const std::string& name) :
    VkMeshBuffer(info, name, vertices, indices)
{
}

VkQuadMesh::VkQuadMesh(VulkanData& info, const MeshData& mesh, PreRenderMesh preRender) :
    VkMeshData(info, mesh, vertices, indices, preRender)
{
}