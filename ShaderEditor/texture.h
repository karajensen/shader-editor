////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - texture.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <vector>
#include "colour.h"

/**
* Texture rendered on a mesh
*/
class Texture
{
public:

    /**
    * Type of texture
    * Ordering must much usage in shader body
    */
    enum Type
    {
        DIFFUSE,
        NORMAL,
        SPECULAR,
        ENVIRONMENT,
        CAUSTICS,
        MAX_TYPES
    };

    /**
    * Constructor
    * @param name The filename of the texture
    * @param path The full path to the texture
    */
    Texture(const std::string& name, const std::string& path);

    /**
    * @return whether this texture has explicitly set pixels
    */
    bool HasPixels() const;

    /**
    * @return whether this texture is a cube map
    */
    bool IsCubeMap() const;

    /**
    * @return the filename of the texture
    */
    const std::string& Name() const;

    /**
    * @return the filename of the texture
    */
    const std::string& Path() const;

    /**
    * @return the pixels of the texture or empty if to be loaded from file
    */
    const std::vector<Pixel>& Pixels() const;

    /**
    * @return the size of the texture if set
    */
    int Size() const;

    /**
    * Gets a text description of the texture type
    * @param type The type to query for text
    * @return the text description of the type
    */
    static std::string GetTypeDescription(unsigned int type);

    /**
    * Determines whether this texture type is animated
    * @param type The type to query
    * @return whether it is animated
    */
    static bool IsAnimatedType(unsigned int type);

protected:

    std::vector<Pixel> m_pixels; ///< Optional pixels of the texture
    int m_size;                  ///< Optional dimensions of the texture
    std::string m_name;          ///< Name of the texture
    std::string m_path;          ///< Path to the texture
};