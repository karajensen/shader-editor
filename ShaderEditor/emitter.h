////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - emitter.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <string>
#include "particle.h"
#include "colour.h"
#include "ptree_utilities.h"

struct Cache;
struct BoundingArea;

/**
* Data for a particle emitter
* @note currently only support for an emitter aligned with the ground plane
*/
class Emitter
{
public:

    /**
    * Holds information for an instance of this emitter
    * @note instances have the same data but particles do not match
    */
    struct Instance
    {
        std::vector<Particle> particles;   ///< Particles this emitter can spawn
        bool render = true;                ///< Whether this emitter should be rendered
        Float3 position;                   ///< Position of this emitter
    };

    /**
    * Constructor
    * @param node The data to intialize the emitter with
    * @param shaderID The ID of the shader to render with
    */
    Emitter(const boost::property_tree::ptree& node, int shaderID);

    /**
    * Writes the emitter data to a property tree
    * @param node The node to write to
    */
    void Write(boost::property_tree::ptree& node) const;

    /**
    * Writes to the data in the cache from the emitter
    * @param cache The cache of data from the GUI
    */
    void Write(Cache& cache);

    /**
    * Reads the data from the cache into the emitter
    * @param cache The cache of data from the GUI
    */
    void Read(Cache& cache);

    /**
    * Ticks the emitter
    * @param deltatime The time passed between ticks
    * @param cameraBounds Bounding area in front of the camera
    */
    void Tick(float deltatime, 
              const BoundingArea& cameraBounds);

    /**
    * Toggles whether the emitter is paused
    */
    void TogglePaused();

    /**
    * @return The name of the emitter
    */
    const std::string& Name() const;

    /**
    * @return The texture IDs used in the emitter
    */
    const std::vector<int>& Textures() const;

    /**
    * @return The textures names used in the emitter
    */
    const std::vector<std::string>& TextureNames() const;

    /**
    * @return The shader used for rendering particles
    */
    int ShaderID() const;

    /**
    * @return The colour tint of the particles
    */
    const Colour& Tint() const;

    /**
    * Adds a texture to be used by the particles
    * @param ID The unique ID of the texture to use
    */
    void AddTexture(int ID);

    /**
    * @return the instance of the emitter
    */
    const Instance& GetInstance(int index) const;

    /**
    * Sets the position of the instance
    */
    void SetInstance(int index, const Float3& position);

    /**
    * @return the amount of instances of this emitter
    */
    unsigned int InstanceCount() const;

    /**
    * @return a description of what instances are rendered
    */
    std::string GetRenderedInstances() const;

    /**
    * @return the instances of this emitter
    */
    const std::vector<Instance>& Instances() const;

private:

    /**
    * Determines whether the emitter should be rendered
    * @param instancePosition The position of the emitter instance
    * @param cameraBounds Bounding area in front of the camera
    */
    bool ShouldRender(const Float3& instancePosition, 
                      const BoundingArea& bounds);

    float m_minWaitTime = 0.0f;              ///< Seconds to wait before respawning
    float m_maxWaitTime = 0.0f;              ///< Seconds to wait before respawning
    float m_width = 0.0f;                    ///< The width of the emitter
    float m_length = 0.0f;                   ///< The length of the emitter
    float m_minSpeed = 1.0f;                 ///< The minimum speed of the particles
    float m_maxSpeed = 1.0f;                 ///< The maximum speed of the particles
    float m_minSize = 1.0f;                  ///< The minimum size of the particles
    float m_maxSize = 1.0f;                  ///< The maximum size of the particles
    float m_minAmplitude = 1.0f;             ///< The minimum height of the particle waves
    float m_maxAmplitude = 1.0f;             ///< The maximum height of the particle waves
    float m_minWaveSpeed = 1.0f;             ///< The minimum speed of the particle wave
    float m_maxWaveSpeed = 1.0f;             ///< The maximum speed of the particle wave
    float m_minFrequency = 1.0f;             ///< The minimum intensity of the waves
    float m_maxFrequency = 1.0f;             ///< The maximum intensity of the waves
    float m_lifeTime = 0.0f;                 ///< Seconds the particle can live before dying
    float m_lifeFade = 1.0f;                 ///< Seconds before lifetime ends to fade the particle
    Float3 m_direction;                      ///< The direction the particles will spawn 
    Colour m_tint;                           ///< Colour to tint the particle texture
    std::vector<int> m_textures;             ///< Indexes for the particle textures to use
    std::vector<std::string> m_textureNames; ///< Names for the particle textures to use
    std::vector<Instance> m_instances;       ///< All instances of this emitter
    int m_shaderIndex = -1;                  ///< Unique Index of the mesh shader to render with
    std::string m_name;                      ///< Name of this emitter
    bool m_paused = false;                   ///< Whether emission is paused
    int m_visibleInstances = 0;              ///< Number of instances currently rendered
};