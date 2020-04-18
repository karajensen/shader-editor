////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - mesh_data.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <vector>
#include "float3.h"
#include "matrix.h"
#include "render_data.h"
#include "boost/noncopyable.hpp"

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
        Matrix world;                    ///< World matrix 
        Float3 position = Float3(0,0,0); ///< Position offset
        Float3 rotation = Float3(0,0,0); ///< Degress rotated around each axis
        Float3 scale = Float3(1,1,1);    ///< Scaling of the mesh
        int colour = -1;                 ///< Colour texture for rendering
        bool enabled = true;             ///< Whether to render this instance
        bool render = true;              ///< Whether this mesh is visible
        bool requiresUpdate = false;     ///< Whether this mesh requires an update
    };

    /**
    * Constructor
    * @param name The name of the data
    * @param shaderName The name of the shader to use
    * @param shader The ID of the shader to use
    */
    MeshData(const std::string& name, 
             const std::string& shaderName,
             int shaderID);

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
    * Post ticks the mesh
    */
    void PostTick();

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
    * Sets Whether back facing polygons are culled
    */
    void BackfaceCull(bool value);

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
    * @return Number of components that make up a vertex
    */
    int VertexComponentCount() const;

    /**
    * Sets whether this mesh is a sky box
    */
    void SetSkyBox();

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
    * Adds instances at the world center with default values
    */
    virtual void AddInstances(int amount);

    /**
    * Sets an instance for this mesh
    * @param index The ID of this instance
    * @param position The position offset 
    * @param rotation How much to rotate 
    * @param scale The size of the mesh
    */
    void SetInstance(int index,
                     const Float3& position,
                     const Float3& rotation,
                     float scale);
protected:

    /**
    * @return whether this mesh renders with caustics
    */
    bool UsesCaustics() const;

    /**
    * @return the world matrix for the given matrix
    * @note will update the world if pending an update
    */
    const Matrix& GetWorldInstance(int instance);


    std::vector<float> m_vertices;           ///< The vertices constructing this mesh
    std::vector<unsigned int> m_indices;     ///< The indices constructing this mesh
    std::vector<Instance> m_instances;       ///< Current instances of this mesh
    int m_vertexComponentCount = 0;          ///< Number of components that make up a vertex

private:

    /**
    * Updates the world transforms for the given instance
    */
    void UpdateTransforms(Instance& instance);

    /**
    * Determines the radius surrounding this mesh
    * This is the based on the furthest vertex from the mesh center
    */
    void GenerateRadius();

    /**
    * Determines whether the instance should be rendered
    * @param instance The instance to check
    * @param cameraBounds Bounding area in front of the camera
    */
    bool ShouldRender(const Instance& instance,
                      const BoundingArea& bounds);

    /**
    * Gets a text description of the texture type
    * @param type The type to query for text
    * @return the text description of the type
    */
    static std::string GetTypeDescription(unsigned int type);

    std::string m_name;                      ///< Name of the mesh
    bool m_backfacecull = true;              ///< Whether back facing polygons are culled
    int m_shaderIndex = -1;                  ///< Unique Index of the mesh shader to use
    std::string m_shaderName;                ///< The name of the shader to render with
    std::vector<int> m_textureIDs;           ///< IDs for each texture used
    std::vector<int> m_colourIDs;            ///< Possible colour texture for instances
    int m_visibleInstances = 0;              ///< Number of instances visible this tick
    int m_initialInstances = 0;              ///< The number of instances on load
    bool m_skybox = false;                   ///< Whether this mesh is a skybox
    float m_radius = 0.0f;                   ///< The radius of the sphere surrounding the mesh
};