////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - opengltexture.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "openglcommon.h"

/**
* Holds an individual opengl texture
*/
class GlTexture
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
    * Sends the texture to the currently active shader
    * @param slot The slot to put the texture into
    */
    void SendTexture(int slot);

private:

    bool m_initialised;           ///< Whether this texture is initialised
    GLuint m_id;                  ///< Unique id for the texture
    const std::string m_filepath; ///< The path for the texture
};