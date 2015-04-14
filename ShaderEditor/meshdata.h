////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - meshdata.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <vector>
#include "float3.h"
#include "renderdata.h"
#include "ptree_utilities.h"

struct Cache;
struct BoundingArea;

/**
* Base Mesh Information
*/
class MeshData : boost::noncopyable
{
public:

    /**
    * Holds information for a single instance of a mesh
    */
    struct Instance
    {
        Float3 position = Float3(0,0,0); ///< Position offset
        Float3 rotation = Float3(0,0,0); ///< Degress rotated around each axis
        Float3 scale = Float3(1,1,1);    ///< Scaling of the mesh
        bool enabled = true;             ///< Whether to render this instance
        bool render = true;              ///< Whether this mesh is visible
    };

    /**
    * Constructor
    * @param node The data to intialize the data with
    */
    MeshData(const boost::property_tree::ptree& node);

    /**
    * Destructor
    */
    virtual ~MeshData() = default;

    /**
    * Ticks the mesh
    * @param cameraPosition The world position of the camera
    * @param cameraBounds Bounding area in front of the camera
    * @param causticsTexture The ID of the current texture for caustics
    */
    void Tick(const Float3& cameraPosition, 
              const BoundingArea& cameraBounds,
              int causticsTexture);

    /**
    * Writes the data to a property tree
    * @param node The node to write to
    */
    virtual void Write(boost::property_tree::ptree& node) const;

    /**
    * Initialises the mesh data
    */
    void InitialiseMeshData();

    /**
    * @return The name of the mesh
    */
    const std::string& Name() const;

    /**
    * @return The name of the shader to render with
    */
    const std::string& ShaderName() const;

    /**
    * @return The ID of the shader to render with
    */
    int ShaderID() const;

    /**
    * @return Whether back facing polygons are culled
    */
    bool BackfaceCull() const;

    /**
    * @return The vertices constructing this mesh
    */
    const std::vector<float>& Vertices() const;

    /**
    * @return The indicies constructing this mesh
    */
    const std::vector<unsigned int>& Indices() const;

    /**
    * @return The ID for each texture type used
    */
    const std::vector<int>& TextureIDs() const;

    /**
    * @return The name for each texture used
    */
    const std::vector<std::string>& TextureNames() const;

    /**
    * @return Number of components that make up a vertex
    */
    int VertexComponentCount() const;

    /**
    * Sets the shaders used by the mesh to render
    * @param shaderID The ID of the shader to render with
    */
    void SetShaderID(int shaderID);

    /**
    * @return The instances of this mesh
    */
    std::vector<Instance>& Instances();

    /**
    * @return The instances of this mesh
    */
    const std::vector<Instance>& Instances() const;

    /**
    * Gets the instance at the index
    * @param index The index of the instance to get
    * @return the instance
    */
    const Instance& GetInstance(int index) const;

    /**
    * Sets the ID of the texture to use
    * @param slot The type of texture to set
    * @param ID The ID of the texture to use
    */
    void SetTexture(TextureSlot slot, int ID);

    /**
    * @return a description of what instances are rendered
    */
    std::string GetRenderedInstances() const;

protected:

    /**
    * Determines the radius surrounding this mesh
    * This is the based on the furthest vertex from the mesh center
    */
    void GenerateRadius();

    /**
    * Determines whether the instance should be rendered
    * @param instance The instance to check
    * @param position The position of the camera
    * @param cameraBounds Bounding area in front of the camera
    */
    bool ShouldRender(const Instance& instance,
                      const Float3& position, 
                      const BoundingArea& bounds);

    /**
    * @return whether this mesh renders with caustics
    */
    bool UsesCaustics() const;

    /**
    * Gets a text description of the texture type
    * @param type The type to query for text
    * @return the text description of the type
    */
    static std::string GetTypeDescription(unsigned int type);

    std::vector<float> m_vertices;           ///< The vertices constructing this mesh
    std::vector<unsigned int> m_indices;     ///< The indices constructing this mesh
    int m_vertexComponentCount = 0;          ///< Number of components that make up a vertex
    std::string m_name;                      ///< Name of the mesh
    bool m_backfacecull = true;              ///< Whether back facing polygons are culled
    int m_shaderIndex = -1;                  ///< Unique Index of the mesh shader to use
    std::string m_shaderName;                ///< The name of the shader to render with
    std::vector<int> m_textureIDs;           ///< IDs for each texture used
    std::vector<std::string> m_textureNames; ///< Names for each texture used
    std::vector<Instance> m_instances;       ///< Current instances of this mesh
    int m_visibleInstances = 0;              ///< Number of instances visible this tick
    bool m_skybox = false;                   ///< Whether this mesh is a skybox
    bool m_usesCaustics = false;             ///< Whether this mesh supports caustics
    float m_radius = 0.0f;                   ///< The radius of the sphere surrounding the mesh
};