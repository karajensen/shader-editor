////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - terrain.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "grid.h"

/**
* Procedurally generated mesh
*/
class Terrain : public Grid
{
public:

    /**
    * Constructor
    * @param node The data to intialize the mesh with
    */
    Terrain(const boost::property_tree::ptree& node);

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
    * @param pixels The pixels of the heightmap
    * @param hasNormals Whether the terrain requires normals
    * @param hasTangents Whether the terrain requires tangents
    * @return whether call was successful
    */
    bool Initialise(const std::vector<unsigned int>& pixels,
                    bool hasNormals, 
                    bool hasTangents);

    /**
    * @return the name of the height map to use
    */
    const std::string& HeightMap() const;

    /**
    * @return Brightness of the specular highlights
    */
    const float& Specularity() const;

    /**
    * @return Ambient light multiplier
    */
    const float& Ambience() const;

    /**
    * @return The saturation of the bump
    */
    const float& Bump() const;

    /**
    * @return Caustics multiplier
    */
    const float& Caustics() const;

private:

    /**
    * Generates terrain using the given type
    * @param pixels The pixels of the height map
    */
    void GenerateTerrain(const std::vector<unsigned int>& pixels);

    std::string m_heightmap;     ///< The name of the heightmap to use
    float m_maxHeight = 1.0f;    ///< The maximum height of the terrain
    float m_minHeight = 0.0f;    ///< The minimum height of the terrain
    float m_bump = 1.0f;         ///< Saturation of bump
    float m_caustics = 1.0f;     ///< How much of the caustics are visible
    float m_specularity = 1.0f;  ///< Brightness of the specular highlights
    float m_ambience = 1.0f;     ///< Ambient light multiplier
};