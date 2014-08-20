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

private:

    bool m_initialised = false;   ///< Whether this texture is initialised
    GLuint m_id = 0;              ///< Unique id for the texture
    const std::string m_filepath; ///< The path for the texture
};