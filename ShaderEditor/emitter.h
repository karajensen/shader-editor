////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - emitter.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <string>
#include "particle.h"
#include "colour.h"
#include "ptree_utilities.h"

/**
* Data for a particle emitter
* @note currently only support for an emitter aligned with the ground plane
*/
class Emitter
{
public:

    /**
    * Constructor
    * @param node The data to read in for the emitter
    * @param shaderID the ID of the shader to use
    */
    Emitter(const boost::property_tree::ptree& node, int shaderID);

    /**
    * Writes the emitter data to a property tree
    * @param node The node to write to
    */
    void Write(boost::property_tree::ptree& node) const;

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

    /**
    * @return the name of the emitter
    */
    const std::string& Name() const;

    /**
    * @return the particles in the emitter
    */
    const std::vector<Particle>& Particles() const;

    /**
    * @return the textures used in the emitter
    */
    const std::vector<int>& Textures() const;

    /**
    * @return the shader used for the emitter
    */
    int ShaderID() const;

    /**
    * @return the colour tint of the particles
    */
    const Colour& Tint() const;

    /**
    * Adds a texture to be used by the particles
    */
    void AddTexture(int ID);

private:

    friend class SceneModifier; //TO FIX

    float m_width = 0.0f;              ///< The width of the emitter
    float m_length = 0.0f;             ///< The length of the emitter
    float m_minSpeed = 1.0f;           ///< The minimum speed of the particles
    float m_maxSpeed = 1.0f;           ///< The maximum speed of the particles
    float m_minSize = 1.0f;            ///< Minimum size of the particles
    float m_maxSize = 1.0f;            ///< Maximum size of the particles
    float m_minAmplitude = 1.0f;       ///< How much this particle with wave
    float m_maxAmplitude = 1.0f;       ///< How much this particle with wave
    float m_minWaveSpeed = 1.0f;       ///< Speed of the particle wave
    float m_maxWaveSpeed = 1.0f;       ///< Speed of the particle wave
    float m_minFrequency = 1.0f;       ///< The intensity of the waves
    float m_maxFrequency = 1.0f;       ///< The intensity of the waves
    float m_lifeTime = 0.0f;           ///< Seconds the particle can live before dying
    float m_lifeFade = 1.0f;           ///< Seconds before lifetime ends to fade the particle
    Float3 m_position;                 ///< The position of the emitter
    Float3 m_direction;                ///< The direction the particles will spawn 
    Colour m_tint;                     ///< Colour to tint the particle texture
    std::vector<int> m_textures;       ///< Indexes for the particle textures to use
    std::vector<Particle> m_particles; ///< Particles this emitter can spawn
    int m_shaderIndex = -1;            ///< Unique Index of the mesh shader to use
    std::string m_name;                ///< Name of this emitter
    bool m_paused = false;             ///< Whether emission is paused
};