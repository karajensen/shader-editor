////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - meshdata.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <vector>
#include "float3.h"
#include "texture.h"
#include "ptree_utilities.h"

struct Cache;

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
        bool shouldRender = true;        ///< Whether to render this instance
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
    * Writes the data to a property tree
    * @param node The node to write to
    */
    virtual void Write(boost::property_tree::ptree& node) const;

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
    * Sets the ID of the texture to use
    * @param type The type of texture to set
    * @param ID The ID of the texture to use
    */
    void SetTexture(Texture::Type type, int ID);

    /**
    * @return The instances of this mesh
    */
    std::vector<Instance>& Instances();

    /**
    * @return The instances of this mesh
    */
    const std::vector<Instance>& Instances() const;

protected:

    std::vector<float> m_vertices;           ///< The vertices constructing this mesh
    std::vector<unsigned int> m_indices;     ///< The indices constructing this mesh
    int m_vertexComponentCount = 1;          ///< Number of components that make up a vertex
    std::string m_name;                      ///< Name of the mesh
    bool m_backfacecull = true;              ///< Whether back facing polygons are culled
    int m_shaderIndex = -1;                  ///< Unique Index of the mesh shader to use
    std::string m_shaderName;                ///< The name of the shader to render with
    std::vector<int> m_textureIDs;           ///< IDs for each texture used
    std::vector<std::string> m_textureNames; ///< Names for each texture used
    std::vector<Instance> m_instances;       ///< Current instances of this mesh
};