////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - terrain.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "grid.h"
#include "meshAttributes.h"

/**
* Procedurally generated mesh
*/
class Terrain : public Grid, public MeshAttributes
{
public:

    /**
    * Constructor
    * @param node The data to intialize the mesh with
    * @param pixels The height map to modify the terrain with
    * @param heightmap The name of the height map texture
    */
    Terrain(const boost::property_tree::ptree& node, 
            const std::vector<unsigned int>& pixels,
            const std::string& heightmap);

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
    * Initialises the terrain
    * @param hasNormals Whether the terrain requires normals
    * @param hasTangents Whether the terrain requires tangents
    * @return whether call was successful
    */
    bool Initialise(bool hasNormals, bool hasTangents);

    /**
    * Reloads the terrain
    */
    void Reload();

    /**
    * Adds a new instance of the terrain
    * @param position The position of the instance
    */
    void AddInstance(const Float2& position);

    /**
    * Sets the instance of the terrain
    * @param index The index of the instance to set
    * @param position The position of the instance
    */
    void SetInstance(int index, const Float2& position);

private:

    /**
    * Generates terrain using the given type
    */
    void GenerateTerrain();

    std::string m_heightmap;   ///< The name of the heightmap to use
    float m_height = 0.0f;     ///< The starting height of the terrain
    float m_maxHeight = 1.0f;  ///< The maximum height offset of the terrain
    float m_minHeight = 0.0f;  ///< The minimum height offset of the terrain
    Float2 m_uvScale;          ///< Texture stretch multiplier
    const std::vector<unsigned int>& m_pixels; ///< Pixels of the height map
};