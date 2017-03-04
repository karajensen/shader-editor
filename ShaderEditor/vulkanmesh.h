////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkanmesh.h
////////////////////////////////////////////////////////////////////////////////////////

#include "vulkan.h"
#include "glm\glm.hpp"

class MeshData;
struct VulkanData;

class VkMesh
{
public:
    /**
    * Constructor
    * @param info Data for the Vulkan Engine
    * @param mesh The mesh data to create
    */
    VkMesh(VulkanData& info, const MeshData& mesh);

    /**
    * Destructor
    */
    ~VkMesh();

    /**
    * Initialises the buffers
    */
    bool Initialise();

    /**
    * Releases resources
    */
    void Release();

    /**
    * Binds the mesh for rendering
    */
    void Render(VkCommandBuffer cmd);

    /**
    * TODO: Make this generic
    */
    glm::mat4 GetWorld() const;

private:

    /**
    * Resets resources
    */
    void Reset();

    VulkanData& m_info;             ///< Data for the Vulkan Engine
    const MeshData& m_mesh;         ///< The mesh data to create
    VkDeviceMemory m_vertexMemory;
    VkBuffer m_vertexBuffer;
    VkDeviceMemory m_indexMemory;
    VkBuffer m_indexBuffer;
    uint32_t m_indexCount;
};