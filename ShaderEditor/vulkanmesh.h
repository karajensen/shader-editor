////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkanmesh.h
////////////////////////////////////////////////////////////////////////////////////////

#include "vulkanutils.h"
#include "boost/noncopyable.hpp"
#include <functional>

class MeshData;
class Water;
class Mesh;
class Terrain;
struct VulkanData;

/**
* Base data for any polygons to be rendered
*/
class VkMeshBuffer : boost::noncopyable
{
public:

    /**
    * Constructor
    * @param info Data for the Vulkan Engine
    * @param name The name of the mesh
    * @param vertices The vertex buffer
    * @param indices The index buffer
    */
    VkMeshBuffer(VulkanData& info,
                 const std::string& name,
                 const std::vector<float>& vertices,
                 const std::vector<unsigned int>& indices);

    /**
    * Destructor
    */
    virtual ~VkMeshBuffer();

    /**
    * Releases the mesh
    */
    void Release();

    /**
    * Renders the mesh
    */
    virtual void Render(VkCommandBuffer cmd);

    /**
    * Initialises the mesh
    * @return whether initialisation succeeded
    */
    virtual bool Initialise();

protected:

    int m_shaderID = NO_INDEX;                  ///< ID of the shader to use
    std::string m_name;                         ///< Name of the mesh
    const std::vector<float>& m_vertices;       ///< Vertex buffer data
    const std::vector<unsigned int>& m_indices; ///< Index buffer data
    VulkanData& m_info;                         ///< Data for the Vulkan Engine
    VkDeviceMemory m_vertexMemory;
    VkBuffer m_vertexBuffer;
    VkDeviceMemory m_indexMemory;
    VkBuffer m_indexBuffer;
    uint32_t m_indexCount;
};

/**
* Vulkan MeshData with instances
*/
class VkMeshData : public VkMeshBuffer
{
public:

    /**
    * Constructor
    * @param info Data for the Vulkan Engine
    * @param mesh The mesh to use as a template
    * @param preRender Callback to prerender an instance
    */
    VkMeshData(VulkanData& info,
               const MeshData& mesh,
               PreRenderMesh preRender);

    /**
    * Constructor
    * @param info Data for the Vulkan Engine
    * @param mesh The mesh to use as a template
    * @param vertices The vertex buffer
    * @param indices The index buffer
    * @param preRender Callback to prerender an instance
    */
    VkMeshData(VulkanData& info,
               const MeshData& mesh,
               const std::vector<float>& vertices,
               const std::vector<unsigned int>& indices,
               PreRenderMesh preRender);

    /**
    * Renders the mesh
    */
    virtual void Render(VkCommandBuffer cmd) override;

    /**
    * Initialises the mesh
    * @return whether initialisation succeeded
    */
    virtual bool Initialise() override;

    /**
    * @return the mesh element
    */
    const MeshData& GetData() const;

private:

    const MeshData& m_meshdata;           ///< Data for the mesh
    std::vector<glm::mat4> m_world;       ///< World matrices of the instances
    bool m_updateInstances = false;       ///< Whether a full transform update is required
    PreRenderMesh m_preRender = nullptr;  ///< Callback to render instances
};

/**
* Vulkan mesh with instances
*/
class VkMesh : public VkMeshData
{
public:

    /**
    * Constructor
    * @param info Data for the Vulkan Engine
    * @param mesh The mesh to use as a template
    * @param preRender Callback to prerender an instance
    */
    VkMesh(VulkanData& info, 
           const MeshData& mesh,
           PreRenderMesh preRender);

    /**
    * @return the mesh element
    */
    const Mesh& GetMesh() const;

    /**
    * @return the water information for the mesh
    */
    const Water& GetWater() const;

    /**
    * @return the water information for the mesh
    */
    const Terrain& GetTerrain() const;
};

/**
* Vulkan quad data
*/
struct VkQuadData
{
public:

    /**
    * Constructor
    */
    VkQuadData();

    std::vector<float> vertices;        ///< Vertex information
    std::vector<unsigned int> indices;  ///< Index information
};

/**
* Vulkan instanced quad
*/
class VkQuadMesh : public VkQuadData, public VkMeshData
{
public:

    /**
    * Constructor
    * @param info Data for the Vulkan Engine
    * @param mesh The mesh to use as a template
    * @param preRender Callback to prerender an instance
    */
    VkQuadMesh(VulkanData& info, 
               const MeshData& mesh,
               PreRenderMesh preRender);
};

/**
* Vulkan single quad
*/
class VkQuad : public VkQuadData, public VkMeshBuffer
{
public:

    /**
    * Constructor
    * @param info Data for the Vulkan Engine
    * @param name The name of the quad
    */
    VkQuad(VulkanData& info, 
           const std::string& name);
};