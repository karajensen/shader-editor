////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - opengltexture.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "openglcommon.h"

/**
* Holds an individual opengl texture
*/
class GlTexture : boost::noncopyable
{
public:

    /**
    * Constructor
    * @param filepath The path for the texture
    */
    GlTexture(const std::string& filepath);

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
    * @return whether this texture is a cube map
    */
    bool IsCubeMap() const;

private:

    /**
    * Loads a texture from file
    * @param type The type of texture to load
    * @param path The path to the texture
    */
    void LoadTexture(GLenum type, const std::string& path);

    const bool m_isCubeMap = false; ///< Whether the texture is 3D
    bool m_initialised = false;     ///< Whether this texture is initialised
    GLuint m_id = 0;                ///< Unique id for the texture
    const std::string m_filepath;   ///< The path for the texture
};