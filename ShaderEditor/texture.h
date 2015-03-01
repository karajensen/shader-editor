////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - texture.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>

/**
* Texture rendered on a mesh
*/
class Texture
{
public:

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
    * Constructor
    * @param name The filename of the texture
    * @param path The full path to the texture
    */
    Texture(const std::string& name, const std::string& path);

    /**
    * @return the filename of the texture
    */
    const std::string& Name() const;

    /**
    * @return the filename of the texture
    */
    const std::string& Path() const;

    /**
    * Gets a text description of the texture type
    * @param type The type to query for text
    * @return the text description of the type
    */
    static std::string GetTypeDescription(unsigned int type);

private:

    std::string m_name; ///< Name of the texture
    std::string m_path; ///< Path to the texture
};