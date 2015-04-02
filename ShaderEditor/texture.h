////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - texture.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <vector>

/**
* Texture rendered on a mesh
*/
class Texture
{
public:

    /**
    * Type of texture
    * Ordering must match usage in shader body
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
    * Type of filtering for this texture
    */
    enum Filter
    {
        NEAREST,
        LINEAR,
        ANISOTROPIC
    };

    /**
    * Constructor
    * @param name The filename of the texture
    * @param path The full path to the texture
    * @param filter The filtering to use
    */
    Texture(const std::string& name, const std::string& path, Filter filter);

    /**
    * Destructor
    */
    virtual ~Texture() = default;

    /**
    * @return whether this texture is a cube map
    */
    bool IsCubeMap() const;

    /**
    * @return whether this texture is a cube map
    */
    static bool IsCubeMap(const std::string& path);

    /**
    * @return the type of filtering used
    */
    Filter Filtering() const;

    /**
    * @return the filename of the texture
    */
    const std::string& Name() const;

    /**
    * @return the filename of the texture
    */
    const std::string& Path() const;

    /**
    * @return the pixels of the texture or nullptr if empty
    */
    virtual const std::vector<unsigned int>& Pixels() const;

    /**
    * @return the size of the texture if set
    */
    virtual int Size() const;

    /**
    * @return whether this texture has explicitly set pixels
    */
    virtual bool HasPixels() const;

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

private:

    Filter m_filter;    ///< The type of filtering to use
    std::string m_name; ///< Name of the texture
    std::string m_path; ///< Path to the texture
};