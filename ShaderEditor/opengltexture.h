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
    * @param id The unique OpenGL id for the texture
    */
    void Initialise(GLuint id);

    /**
    * Sends the texture to the currently active shader
    */
    void SendTexture();

private:

    int m_id; ///< Unique id for the texture
    const std::string m_filepath; ///< The path for the texture
};