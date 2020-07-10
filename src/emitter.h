////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - emitter.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <string>
#include "particle.h"
#include "colour.h"

struct Cache;
struct BoundingArea;

/**
* Data for a particle emitter
*/
struct EmitterData
{
    float radius = 0.0f;          ///< The radius of the emitter
    float width = 0.0f;           ///< The width of the emitter
    float length = 0.0f;          ///< The length of the emitter
    float minSpeed = 1.0f;        ///< The minimum speed of the particles
    float maxSpeed = 1.0f;        ///< The maximum speed of the particles
    float minSize = 1.0f;         ///< The minimum size of the particles
    float maxSize = 1.0f;         ///< The maximum size of the particles
    float minAmplitude = 1.0f;    ///< The minimum height of the particle waves
    float maxAmplitude = 1.0f;    ///< The maximum height of the particle waves
    float minFrequency = 1.0f;    ///< The minimum intensity of the waves
    float maxFrequency = 1.0f;    ///< The maximum intensity of the waves
    float lifeTime = 0.0f;        ///< Seconds the particle can live before dying
    float lifeFade = 1.0f;        ///< Seconds before lifetime ends to fade the particle
    float minWaitTime = 0.0f;     ///< Seconds to wait before respawning
    float maxWaitTime = 0.0f;     ///< Seconds to wait before respawning
    Float3 direction;             ///< The direction the particles will spawn 
    Colour tint;                  ///< Colour to tint the particle texture
    int instances = 0;            ///< Number of instances of this emitter
    int particles = 0;            ///< Number of particles per instance
};

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
    * @param name The name of the emitter
    * @param shaderID The ID of the shader to render with
    */
    Emitter(const std::string& name, int shaderID);

    /**
    * Initialises the emitter
    * @param data The data to initialise the emitter with
    * @return whether initialisation succeeded
    */
    bool Initialise(const EmitterData& data);

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

    /**
    * Sets whether the emitter is enabled or not
    */
    void SetEnabled(bool enabled);

private:

    /**
    * Determines whether the emitter should be rendered
    * @param instancePosition The position of the emitter instance
    * @param cameraBounds Bounding area in front of the camera
    */
    bool ShouldRender(const Float3& instancePosition, 
                      const BoundingArea& bounds);

private:

    EmitterData m_data;                  ///< Data for this emitter
    std::vector<int> m_textures;         ///< Indexes for the particle textures to use
    std::vector<Instance> m_instances;   ///< All instances of this emitter
    int m_shaderIndex = -1;              ///< Unique Index of the mesh shader to render with
    int m_totalParticles = 0;            ///< Total amount of particles over all instances
    int m_visibleInstances = 0;          ///< Number of instances currently rendered
    std::string m_name;                  ///< Name of this emitter
    bool m_paused = false;               ///< Whether emission is paused
    bool m_enabled = false;              ///< Whether emission is enabled
};
