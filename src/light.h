////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - light.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include "float3.h"
#include "colour.h"

struct Cache;

/**
* Single light to provide illumination in the scene
*/
class Light
{
public:

    /**
    * Constructor
    * @param name The name of the light
    */
    Light(const std::string& name); 

    /**
    * Writes to the data in the cache from the light
    * @param cache The cache of data from the GUI
    */
    void Write(Cache& cache);

    /**
    * Reads the data from the cache into the light
    * @param cache The cache of data from the GUI
    */
    void Read(Cache& cache);

    /**
    * @return The name of the light
    */
    const std::string& Name() const;

    /**
    * @return Colour of the light
    */
    const Colour& Diffuse() const;

    /**
    * @return Specular highlights the light will cast
    */
    const Colour& Specular() const;

    /**
    * @return How much the light will fade in distance
    */
    const Float3& Attenuation() const;

    /**
    * @return World coordinates of the light
    */
    const Float3& Position() const;

    /**
    * @return The brightness of the specular highlights
    */
    const float& Specularity() const;

    /**
    * @return How much activity this light contributes
    */
    const float& Active() const;

    /**
    * Sets the Colour of the light
    */
    void Diffuse(const Colour& value);

    /**
    * Sets the Specular highlights the light will cast
    */
    void Specular(const Colour& value);

    /**
    * Sets How much the light will fade in distance
    */
    void Attenuation(const Float3& value);

    /**
    * Sets the World coordinates of the light
    */
    void Position(const Float3& value);

    /**
    * Sets the World coordinates of the light
    */
    void PositionX(float x);

    /**
    * Sets the World coordinates of the light
    */
    void PositionZ(float z);

    /**
    * Sets The brightness of the specular highlights
    */
    void Specularity(float value);

    /**
    * Sets How much activity this light contributes
    */
    void Active(float value);

private:

    std::string m_name;          ///< Name of the light
    Colour m_diffuse;            ///< Colour of the light
    Colour m_specular;           ///< Specular highlights the light will cast
    Float3 m_attenuation;        ///< How much the light will fade in distance
    Float3 m_position;           ///< World coordinates of the light
    float m_specularity = 1.0f;  ///< Brightness of the specular highlights
    float m_active = 1.0f;       ///< How much activity this light contributes
};
