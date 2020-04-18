////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - opengl_texture.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "opengl_common.h"

/**
* Holds an individual opengl texture
*/
class GlTexture : boost::noncopyable
{
public:

    /**
    * Constructor
    * @param texture Contains the texture data
    */
    GlTexture(const Texture& texture);

    /**
    * Destructor
    */
    ~GlTexture();

    /**
    * Releases the texture
    */
    void Release();

    /**
    * Initialises the texture
    * @return whether initialisation succeeded or not
    */
    bool Initialise();

    /**
    * @return the unique ID for the texture
    */
    GLuint GetID() const;

    /**
    * @return the filename of the texture
    */
    const std::string& Name() const;

    /**
    * @return whether this texture is a cube map
    */
    bool IsCubeMap() const;

    /**
    * Reloads the texture from pixels
    * @return whether reloading was successful
    */
    bool ReloadPixels();

private:

    /**
    * Creates the mipmaps for the texture
    * @return whether creation was successful
    */
    bool CreateMipMaps();

    /**
    * Sets the filtering for the texture
    */
    void SetFiltering();

    /**
    * Initialises a cube map
    */
    void InitialiseCubeMap();

    /**
    * Initialises the texture from file
    */
    void InitialiseFromFile();

    /**
    * Initialises the texture from pixels
    */
    void InitialiseFromPixels();

    /**
    * Loads a texture from file
    * @param type The type of texture to load
    * @param path The path to the texture
    */
    void LoadTexture(GLenum type, const std::string& path);

    const Texture& m_texture;       ///< Contains the texture data
    bool m_initialised = false;     ///< Whether this texture is initialised
    GLuint m_id = 0;                ///< Unique id for the texture
};