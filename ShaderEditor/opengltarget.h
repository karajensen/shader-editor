////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - openglcommon.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "openglcommon.h"

/**
* Render target with optional attached depth buffer
*/
class GlRenderTarget
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
    * Sends the texture to the currently active shader
    * @param slot The slot to put the texture into
    * @note requires clearing before render target can use again
    */
    void SendTexture(int slot);

    /**
    * Clears the texture sent to the currently active shader
    * @param slot The slot the texture exists in
    * @note Required as texture needs to be unbound from the 
    *       shader before render target is activated
    */
    void ClearTexture(int slot);

private:

    bool m_initialised;          ///< Whether the buffer is initialised or not
    bool m_isBackBuffer;         ///< Whether this render target is the back buffer
    const std::string m_name;    ///< Name of the render target
    GLuint m_colorTexture;       ///< Unique ID of the attached color buffer
    GLuint m_renderBuffer;       ///< Unique ID of the buffer holding the depth information
    GLuint m_frameBuffer;        ///< Unique ID of the frame buffer
};