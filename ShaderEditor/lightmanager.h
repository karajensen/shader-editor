////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - lightmanager.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include "common.h"

struct CTwBar;

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
};

/**
* Manager and owner of all lights in scene
*/
class LightManager
{
public:

    /**
    * Constructor
    * @param tweakbar The tweakbar for diagnostics
    */
    LightManager(CTwBar* tweakbar);

private:

    std::vector<Light> m_lights; ///< All lights in the scene


};                     