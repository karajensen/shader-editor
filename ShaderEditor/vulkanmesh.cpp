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
    static std::vector<float> vertexBuffer =
    {
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
         -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
         0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
    };

    uint32_t vertexBufferSize = static_cast<uint32_t>(vertexBuffer.size()) * sizeof(float);

    std::vector<uint32_t> indexBuffer = { 0, 1, 2 };
    m_indexCount = static_cast<uint32_t>(indexBuffer.size());
    uint32_t indexBufferSize = m_indexCount * sizeof(uint32_t);

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

    memcpy(data, vertexBuffer.data(), vertexBufferSize);
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

    memcpy(data, indexBuffer.data(), indexBufferSize);
    vkUnmapMemory(m_info.device, m_indexMemory);
    if (CHECK_FAIL(vkBindBufferMemory(m_info.device, m_indexBuffer, m_indexMemory, 0)))
    {
        return false;
    }

    return true;
}

void VkMesh::Bind(VkCommandBuffer cmd)
{
    VkDeviceSize offsets[1] = { 0 };
    vkCmdBindVertexBuffers(cmd, 0, 1, &m_vertexBuffer, offsets);
    vkCmdBindIndexBuffer(cmd, m_indexBuffer, 0, VK_INDEX_TYPE_UINT32);
    vkCmdDrawIndexed(cmd, m_indexCount, 1, 0, 0, 1);
}