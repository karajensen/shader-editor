////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - mesh.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <vector>
#include "float3.h"
#include "colour.h"
#include "matrix.h"
#include "meshdata.h"

/**
* Mesh loaded from file to be rendered in the scene
*/
class Mesh : public MeshData
{
public:

    /**
    * Holds information for a single instance of a mesh
    */
    struct Instance
    {
        Colour colour;            ///< Colouring of the mesh
        Float3 position;          ///< Position offset
        Float3 rotation;          ///< Degress rotated around each axis
        float scale = 1.0f;       ///< Scaling of the mesh
        bool shouldRender = true; ///< Whether to render this instance
    };

    /**
    * Constructor
    * @param node The data to intialize the mesh with
    */
    Mesh(const boost::property_tree::ptree& node);

    /**
    * Writes the data to a property tree
    * @param node The node to write to
    */
    virtual void Write(boost::property_tree::ptree& node) const override;

    /**
    * Writes to the data in the cache
    * @param cache The cache of data from the GUI
    */
    void Write(Cache& cache);

    /**
    * Reads the data from the cache
    * @param cache The cache of data from the GUI
    */
    void Read(Cache& cache);

    /**
    * @return The instances of this mesh
    */
    std::vector<Instance>& Instances();

    /**
    * @return The instances of this mesh
    */
    const std::vector<Instance>& Instances() const;

    /**
    * @return Brightness of the specular highlights
    */
    const float& Specularity() const;

    /**
    * @return The saturation of the bump
    */
    const float& Bump() const;

    /**
    * @return Ambient light multiplier
    */
    const float& Ambience() const;

    /**
    * @return Caustics multiplier
    */
    const float& Caustics() const;

    /**
    * Initialises the mesh data buffer containers from file
    * @param path The full path to the mesh file
    * @param requiresNormals Whether this mesh requires normals
    * @param requiresTangents Whether this mesh requires tangents/bitangents
    * @return Whether creation was successful
    */
    bool InitialiseFromFile(const std::string& path, 
                            bool requiresNormals, 
                            bool requiresTangents);

private:

    float m_bump = 1.0f;               ///< Saturation of bump
    float m_caustics = 1.0f;           ///< How much of the caustics are visible
    float m_specularity = 1.0f;        ///< Brightness of the specular highlights
    float m_ambience = 1.0f;           ///< Ambient light multiplier
    int m_initialInstances = 0;        ///< The number of instances on load
    std::vector<Instance> m_instances; ///< Instances of this mesh
};