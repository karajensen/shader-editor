////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - texture.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>

/**
* Texture rendered on a mesh
*/
struct Texture
{
    /**
    * Type of texture
    * Required in order of usage in shader body for OpenGL
    */
    enum Type
    {
        DIFFUSE,
        NORMAL,
        SPECULAR,
        GLOW,
        ENVIRONMENT,
        MAX_TYPES
    };

    /**
    * Gets a text description of the texture type
    * @param type The type to query for text
    * @return the text description of the type
    */
    static std::string GetTypeDescription(unsigned int type)
    {
        switch (type)
        {
        case DIFFUSE:
            return "Diffuse";
        case NORMAL:
            return "Normal";
        case SPECULAR:
            return "Specular";
        case GLOW:
            return "Glow";
        case ENVIRONMENT:
            return "Environment";
        default:
            return "None";
        };
    }

    std::string name; ///< Name of the texture
    std::string path; ///< Path to the texture
};