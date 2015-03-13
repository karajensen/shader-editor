////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - terrain.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include "meshdata.h"

/**
* Procedurally generated mesh
*/
class Terrain : public MeshData
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

    /**
    * @return Whether back facing polygons are culled
    */
    bool BackfaceCull() const;

private:

    float m_bump = 1.0f;         ///< Saturation of bump
    float m_caustics = 1.0f;     ///< How much of the caustics are visible
    float m_specularity = 1.0f;  ///< Brightness of the specular highlights
    float m_ambience = 1.0f;     ///< Ambient light multiplier
};