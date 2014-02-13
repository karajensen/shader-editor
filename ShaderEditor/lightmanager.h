////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - lightmanager.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include "common.h"

/**
* Individual light object
*/
struct Light
{
    /**
    * Constructor
    */
    Light();

    bool castshadow;     ///< Whether the light casts shadows or not
    std::string name;    ///< Name of the light
    Colour diffuse;      ///< Colour of the light
    Colour specular;     ///< Specular highlights the light will cast
    Float3 attenuation;  ///< How much the light will fade in distance
    Float3 position;     ///< World coordinates of the light
    float specularity;   ///< Brightness of the specular highlights
};

/**
* Manager and owner of all lights in scene
*/
class LightManager
{
public:

    /**
    * Constructor
    */
    LightManager();

    /**
    * Constructor
    * @return Whether the lighting was successful or not
    */
    bool Initialise();

    /**
    * Selects the next light to be editable
    */
    void SelectNextLight();

private:

    std::vector<Light> m_lights; ///< All lights in the scene
    int m_selectedLight;         ///< Index of the selected light
};                     