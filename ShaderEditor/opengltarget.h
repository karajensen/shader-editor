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
    * Constructor for the back buffer
    * @param name Name of the render target
    * @param isBackBuffer Whether this render target is the back buffer
    */
    GlRenderTarget(const std::string& name);

    /**
    * Constructor for a render target
    * @param name Name of the render target
    * @param textures The number of textures attached to this target
    */
    GlRenderTarget(const std::string& name, int textures);

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
    GLuint GetTexture(int index = 0) const;

private:

    /**
    * @param index The index of the texture
    * @return the OpenGL Attachement ID for the texture
    */
    unsigned int GetTextureAttachment(int index) const;

    bool m_initialised = false;        ///< Whether the buffer is initialised or not
    const int m_count = 0;             ///< Number of textures attached to this buffer
    const bool m_isBackBuffer = false; ///< Whether this render target is the back buffer
    const std::string m_name;          ///< Name of the render target
    std::vector<GLuint> m_textures;    ///< Unique IDs of the attached textures
    std::vector<GLenum> m_attachments; ///< Container of attachment slots taken up
    GLuint m_renderBuffer = 0;         ///< Unique ID of the buffer holding the depth information
    GLuint m_frameBuffer = 0;          ///< Unique ID of the frame buffer
};