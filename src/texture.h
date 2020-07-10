////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - texture.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <vector>

struct Cache;

/**
* Texture rendered on a mesh
*/
class Texture
{
public:

    /**
    * Type of filtering for this texture
    */
    enum Filter
    {
        Nearest,
        Linear,
        Anisotropic
    };

    /**
    * The type of texture this is
    */
    enum Type
    {
        FromFile,
        Cube,
        Procedural
    };

    /**
    * Constructor for a texture
    * @param name The filename of the texture
    * @param path The full path to the texture
    * @param type The type of image to display
    * @param filter The type of filtering for this texture
    */
    Texture(const std::string& name, 
            const std::string& path,
            Type type,
            Filter filter);

    /**
    * Destructor
    */
    virtual ~Texture() = default;

    /**
    * Writes to the data in the cache
    * @param cache The cache of data from the GUI
    */
    virtual void Write(Cache& cache);

    /**
    * Reads the data from the cache
    * @param cache The cache of data from the GUI
    */
    virtual void Read(Cache& cache);

    /**
    * @return whether this texture is a cube map
    */
    bool IsCubeMap() const;

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
    * @throw not implemented exception
    */
    virtual const std::vector<unsigned int>& Pixels() const;

    /**
    * Saves the texture to file
    * @throw not implemented exception
    */
    virtual void Save();

    /**
    * Reloads the texture
    * @throw not implemented exception
    */
    virtual void Reload();

    /**
    * @return the size of the texture if set
    * @throw not implemented exception
    */
    virtual int Size() const;

    /**
    * @return whether this texture has explicitly set pixels
    */
    virtual bool HasPixels() const;

    /**
    * @return whether this texture is to be rendered
    */
    virtual bool IsRenderable() const;

private:

    Type m_type;       ///< The type of texture this is
    Filter m_filter;    ///< The type of filtering to use
    std::string m_name; ///< Name of the texture
    std::string m_path; ///< Path to the texture
};
