////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - emitter.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <string>
#include "float3.h"
#include "colour.h"

/**
* Data for a single particle
*/
struct Particle
{
    Particle();

    Float3 startPosition;       ///< The position when first given life
    Float3 position;            ///< The position of the particle
    float lifeTime = 0.0f;      ///< How long has this particle been alive
    float waitTime = 0.0f;      ///< How long has passed while waiting
    float maxWaitTime = 0.0f;   ///< Time to wait before respawning
    float speed = 1.0f;         ///< The speed of the particle
    float waveSpeed = 1.0f;     ///< The wave speed of the particle
    int texture = -1;           ///< The texture to render the particle with
    bool alive = false;         ///< Whether this particle should be rendered
    float size = 1.0f;          ///< Size of the particle
    float alpha = 1.0f;         ///< How transparent this particle is
    float amplitude = 1.0f;     ///< How much this particle with wave
    float frequency = 1.0f;     ///< How intese are the waves
};

/**
* Data for a particle emitter
* @note currently only support for an emitter aligned with the ground plane
*/
struct Emitter
{
    /**
    * Resizes the emitter for a new amount of particles
    * @param size The new size to make the particles
    */
    void Resize(int size);

    /**
    * Ticks the emitter
    * @param deltatime the time passed between ticks
    */
    void Tick(float deltatime);

    /**
    * Toggles whether the emitter is paused
    */
    void TogglePaused();

    float width = 0.0f;              ///< The width of the emitter
    float length = 0.0f;             ///< The length of the emitter
    float minSpeed = 1.0f;           ///< The minimum speed of the particles
    float maxSpeed = 1.0f;           ///< The maximum speed of the particles
    float minSize = 1.0f;            ///< Minimum size of the particles
    float maxSize = 1.0f;            ///< Maximum size of the particles
    float minAmplitude = 1.0f;       ///< How much this particle with wave
    float maxAmplitude = 1.0f;       ///< How much this particle with wave
    float minWaveSpeed = 1.0f;       ///< Speed of the particle wave
    float maxWaveSpeed = 1.0f;       ///< Speed of the particle wave
    float minFrequency = 1.0f;       ///< How intese are the waves
    float maxFrequency = 1.0f;       ///< How intese are the waves
    float lifeTime = 0.0f;           ///< Seconds the particle can live before dying
    float lifeFade = 1.0f;           ///< Seconds before lifetime ends to fade the particle
    Float3 position;                 ///< The position of the emitter
    Float3 direction;                ///< The direction the particles will spawn 
    Colour tint;                     ///< Colour to tint the particle texture
    std::vector<int> textures;       ///< Indexes for the particle textures to use
    std::vector<Particle> particles; ///< Particles this emitter can spawn
    int shaderIndex = -1;            ///< Unique Index of the mesh shader to use
    int normalIndex = -1;            ///< Unique Index of the normal shader to use
    std::string name;                ///< Name of this emitter
    bool paused = false;             ///< Whether emission is paused
};