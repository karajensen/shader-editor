////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - openglcommon.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "openglcommon.h"

/**
* Render target with optional attached depth buffer
*/
class GlRenderTarget : boost::noncopyable
{
public:

    /**
    * Constructor
    * @param name Name of the render target
    * @param isBackBuffer Whether this render target is the back buffer
    */
    GlRenderTarget(const std::string& name, bool isBackBuffer = false);

    /**
    * Destructor
    */
    ~GlRenderTarget();

    /**
    * Releases the render target
    */
    void Release();

    /**
    * Initialises the render target
    * @return whether initialisation succeeded or not
    */
    bool Initialise();

    /**
    * Sets the render target as activated and clears it
    */
    void SetActive();

    /**
    * @return the ID of the target texture
    */
    GLuint GetTextureID() const { return m_colorTexture; }

private:

    bool m_initialised = false;  ///< Whether the buffer is initialised or not
    bool m_isBackBuffer = false; ///< Whether this render target is the back buffer
    const std::string m_name;    ///< Name of the render target
    GLuint m_colorTexture = 0;   ///< Unique ID of the attached color buffer
    GLuint m_renderBuffer = 0;   ///< Unique ID of the buffer holding the depth information
    GLuint m_frameBuffer = 0;    ///< Unique ID of the frame buffer
};