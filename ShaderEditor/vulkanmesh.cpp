////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkanmesh.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "vulkanmesh.h"
#include "vulkanutils.h"
#include "vulkandata.h"
#include "meshdata.h"

VkMesh::VkMesh(VulkanData& info, const MeshData& mesh)
    : m_mesh(mesh)
    , m_info(info)
{
    Reset();
}

VkMesh::~VkMesh()
{
    Release();
}

void VkMesh::Reset()
{
    m_vertexMemory = VK_NULL_HANDLE;
    m_vertexBuffer = VK_NULL_HANDLE;
    m_indexMemory = VK_NULL_HANDLE;
    m_indexBuffer = VK_NULL_HANDLE;
    m_indexCount = 0;
}

glm::mat4 VkMesh::GetWorld() const
{
    // TODO: Make this generic and implement instances
    glm::mat4 world;
    const auto& instance = m_mesh.Instances().at(0);
    world[0][0] = instance.world.m11;
    world[1][0] = instance.world.m12;
    world[2][0] = instance.world.m13;
    world[3][0] = instance.world.m14;
    world[0][1] = instance.world.m21;
    world[1][1] = instance.world.m22;
    world[2][1] = instance.world.m23;
    world[3][1] = instance.world.m24;
    world[0][2] = instance.world.m31;
    world[1][2] = instance.world.m32;
    world[2][2] = instance.world.m33;
    world[3][2] = instance.world.m34;
    return world;
}

void VkMesh::Release()
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

    Reset();
}

bool VkMesh::Initialise()
{
    // TODO: Make this generic for scene
    //static std::vector<float> vertexBuffer =
    //{
    //    1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    //    -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    //    0.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f
    //};
    //std::vector<unsigned int> indexBuffer = { 0, 1, 2 };

    auto& vertices = m_mesh.Vertices();
    auto& indices = m_mesh.Indices();
    //auto& vertices = vertexBuffer;
    //auto& indices = indexBuffer;

    uint32_t vertexBufferSize = static_cast<uint32_t>(vertices.size()) * sizeof(float);

    m_indexCount = static_cast<uint32_t>(indices.size());
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

    memcpy(data, vertices.data(), vertexBufferSize);
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

    memcpy(data, indices.data(), indexBufferSize);
    vkUnmapMemory(m_info.device, m_indexMemory);
    if (CHECK_FAIL(vkBindBufferMemory(m_info.device, m_indexBuffer, m_indexMemory, 0)))
    {
        return false;
    }

    return true;
}

void VkMesh::Render(VkCommandBuffer cmd)
{
    VkDeviceSize offsets[1] = { 0 };
    vkCmdBindVertexBuffers(cmd, 0, 1, &m_vertexBuffer, offsets);
    vkCmdBindIndexBuffer(cmd, m_indexBuffer, 0, VK_INDEX_TYPE_UINT32);
    vkCmdDrawIndexed(cmd, m_indexCount, 1, 0, 0, 1);
}