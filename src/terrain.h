////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - terrain.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "grid.h"
#include "mesh_attributes.h"

/**
* Procedurally generated mesh
*/
class Terrain : public Grid, public MeshAttributes
{
public:

     /**
    * Constructor
    * @param name The name of the terrain
    * @param shaderID The id of the shader to use
    * @param shaderName The name of the shader to use
    * @param pixels The pixels of the height map
    */
    Terrain(const std::string& name,
            const std::string& shaderName,
            int shaderID,
            const std::vector<unsigned int>& pixels);

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
    * @param uvTextureStretch The texture stretch multipliers
    * @param minHeight The minimum height offset of the terrain
    * @param maxHeight The maximum height offset of the terrain
    * @param height The starting height of the terrain
    * @param spacing The spacing between vertices
    * @param size How many rows/columns for the grid
    * @param hasNormals Whether the terrain requires normals
    * @param hasNormals Whether the terrain requires tangents
    * @param requiresTiling whether to force tiling between edges
    * @return whether call was successful
    */
    bool Initialise(float uvTextureStretch,
                    float minHeight,
                    float maxHeight,
                    float height,
                    float spacing, 
                    int size,
                    bool hasNormals,
                    bool hasTangents,
                    bool requiresTiling);

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
                     const Float3& scale);
    /**
    * Adds instances at the world center with default values
    */
    virtual void AddInstances(int amount) override;

    /**
    * @param instance The ID of the instance to use
    * @return the minimum bounds of the grid
    */
    const Float2& GetMinBounds(int instance) const;

    /**
    * @param instance The ID of the instance to use
    * @return the maximum bounds of the grid
    */
    const Float2& GetMaxBounds(int instance) const;

    /**
    * @param instance The ID of the instance to use
    * @return the approximate absoluate position on the terrain
    */
    Float3 GetAbsolutePosition(int instance, float x, float z);

private:

    /**
    * Generates terrain using the given type
    */
    void GenerateTerrain();

    /**
    * Calculates the min/max bounds for the instance
    */
    void CalculateBounds(int instance);

private:

    float m_height = 0.0f; ///< The initial height of the terrain
    float m_maxHeight = 1.0f; ///< The maximum height offset of the terrain
    float m_minHeight = 0.0f; ///< The minimum height offset of the terrain
    bool m_requiresTiling = false; ///< Whether to force tiling between edges
    const std::vector<unsigned int>& m_pixels; ///< The pixel of the height map
    std::vector<Float2> m_minBounds; ///< Minimum bounds for each instance
    std::vector<Float2> m_maxBounds; ///< Maximum bounds for each instance
    Float2 m_uvScale; ///< Texture stretch multiplier
};