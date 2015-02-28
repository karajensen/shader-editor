////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - meshdata.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <vector>
#include "texture.h"
#include "ptree_utilities.h"

struct Cache;

/**
* Base Mesh Information
*/
class MeshData
{
public:

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
    * Writes to the data in the cache
    * @param cache The cache of data from the GUI
    */
    virtual void Write(Cache& cache);

    /**
    * Reads the data from the cache
    * @param cache The cache of data from the GUI
    */
    virtual void Read(Cache& cache);

    /**
    * Initialises the mesh data buffer containers from file
    * @param requiresNormals Whether this mesh requires normals
    * @param requiresTangents Whether this mesh requires tangents/bitangents
    * @return Whether creation was successful
    */
    bool Initialise(bool requiresNormals, bool requiresTangents);

    /**
    * @return The saturation of the bump
    */
    const float& Bump() const;

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
    * @return The ID of the normal shader to render with
    */
    int NormalID() const;

    /**
    * @return The vertices constructing this mesh
    */
    const std::vector<float>& Vertices() const;

    /**
    * @return The indicies constructing this mesh
    */
    const std::vector<unsigned long>& Indices() const;

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
    * @param shaderName The name of the shader to render with
    * @param shaderID The ID of the shader to render with
    * @param normalID The ID of the normal shader to render with
    */
    void SetShaderID(const std::string& shaderName, int shaderID, int normalID);

    /**
    * Sets the ID of the texture to use
    * @param type The type of texture to set
    * @param ID The ID of the texture to use
    */
    void SetTexture(Texture::Type type, int ID);

private:

    float m_bump = 1.0f;                     ///< Saturation of bump
    std::string m_name;                      ///< Name of the mesh
    int m_shaderIndex = -1;                  ///< Unique Index of the mesh shader to use
    int m_normalIndex = -1;                  ///< Unique Index of the normal shader to use
    std::string m_shaderName;                ///< The name of the shader to render with
    std::vector<float> m_vertices;           ///< The vertices constructing this mesh
    std::vector<unsigned long> m_indices;    ///< The indices constructing this mesh
    std::vector<int> m_textureIDs;           ///< IDs for each texture used
    std::vector<std::string> m_textureNames; ///< Names for each texture used
    int m_vertexComponentCount = 1;          ///< Number of components that make up a vertex
};