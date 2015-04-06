////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - water.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "grid.h"
#include "colour.h"

/**
* Data for a water-shaded mesh
*/
class Water : public Grid
{
public:

    /**
    * Data for a water wave
    */
    struct Wave
    {
        float frequency = 0.0f;   ///< Frequency of the wave
        float amplitude = 0.0f;   ///< Amplitude of the wave
        float phase = 0.0f;       ///< Phase of the wave
        float directionX = 0.0f;  ///< Direction the wave moves
        float directionZ = 0.0f;  ///< Direction the wave moves
    };

    /**
    * Constructor
    * @param node The data to intialize the mesh with
    */
    Water(const boost::property_tree::ptree& node);

    /**
    * Writes the data to a property tree
    * @param node The node to write to
    * @param waveNodes The container to add wave data to
    * @param saveNode Callback to create a new node in the tree
    */
    void Write(boost::property_tree::ptree& node,
               std::function<boost::property_tree::ptree&()> createNode) const;

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
    * Writes to the data in the cache
    * @param cache The cache of data from the GUI
    */
    void WriteWave(Cache& cache, int index);

    /**
    * Reads the data from the cache
    * @param cache The cache of data from the GUI
    */
    void ReadWave(Cache& cache, int index);

    /**
    * @return The Wave information for the water
    */
    const std::vector<Wave>& Waves() const;

    /**
    * @return The Velocity of the bump movement
    */
    const Float2& BumpVelocity() const;

    /**
    * @return The overall speed of the wave
    */
    const float& Speed() const;

    /**
    * @return The scale of the water texture
    */
    const Float2& UVScale() const;

    /**
    * @return The fresnal scale, bias and power 
    */
    const Float3& Fresnal() const;

    /**
    * @return Colour intensity of the shallow water
    */
    const Colour& Shallow() const;

    /**
    * @return Colour intensity of the deep water
    */
    const Colour& Deep() const;

    /**
    * @return Colour of the reflections
    */
    const Colour& ReflectionTint() const;

    /**
    * @return the intensity of the reflections
    */
    const float& ReflectionIntensity() const;

    /**
    * @return the maximum supported maximum waves per water
    */
    static int GetMaxWaves();

    /**
    * @return The saturation of the bump
    */
    const float& Bump() const;

    /**
    * Adds a new instance for the water
    * @param position The world position of the center
    * @param flippedX Whether to flip the water in the x axis for tiling
    * @param flippedZ Whether to flip the water in the z axis for tiling
    */
    void AddInstance(const Float2& position, bool flippedX, bool flippedZ);

    /**
    * Sets the instance of the water
    * @param index The index of the instance to set
    * @param position The world position of the center
    * @param flippedX Whether to flip the water in the x axis for tiling
    * @param flippedZ Whether to flip the water in the z axis for tiling
    */
    void SetInstance(int index, const Float2& position, bool flippedX, bool flippedZ);

private:

    float m_height = 0.0f;        ///< The starting height of the water
    float m_bump = 1.0f;          ///< Saturation of bump
    Float2 m_bumpVelocity;        ///< Velocity of the bump movement
    float m_speed = 0.0f;         ///< Overall speed of the wave
    Float2 m_uvScale;             ///< Scale of the water texture
    Float3 m_fresnal;             ///< The fresnal scale, bias and power
    Colour m_shallowColour;       ///< Colour intensity of the shallow water
    Colour m_deepColour;          ///< Colour intensity of the deep water
    Colour m_reflectionTint;      ///< Colour of the reflections
    float m_reflection = 0.0f;    ///< Intensity of the reflections
    std::vector<Wave> m_waves;    ///< Wave infomation
};