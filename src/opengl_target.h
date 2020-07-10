////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - opengl_target.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "opengl_common.h"

/**
* Render target with optional attached depth buffer
*/
class GlRenderTarget : boost::noncopyable
{
public:

    /**
    * Constructor for the back buffer
    * @param name Name of the render target
    */
    GlRenderTarget(const std::string& name);

    /**
    * Constructor for a render target
    * @param name Name of the render target
    * @param textures The number of textures attached to this target
    * @param multisampled Whether this target has multisampling
    * @param readWrite Whether this target both read/writes to itself
    */
    GlRenderTarget(const std::string& name, 
                   int textures, 
                   bool multisampled, 
                   bool readWrite = false);

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

    /**
    * Sets the render target texture as high quality
    * @note needs to be called before initialise
    */
    void SetHighQuality(int index);
    
    /**
    * @return if this target is multisampled
    */
    bool IsMultisampled() const;

    /**
    * Switches the render target textures
    */
    void SwitchTextures();

private:

    /**
    * @param index The index of the texture
    * @return the OpenGL Attachement ID for the texture
    */
    unsigned int GetTextureAttachment(int index) const;

    /**
    * Creates the texture
    * @param id The ID to fill in for the new texture
    * @param type The type of texture to create
    * @para highQuality Whether to make this texture high quality
    * @return whether create was successful
    */
    bool CreateTexture(GLuint& id, unsigned int type, bool highQuality);

private:

    bool m_mainAttached = false;        ///< Whether the main textures are attached
    bool m_readWrite = false;           ///< Whether this target both read/writes to itself
    bool m_initialised = false;         ///< Whether the buffer is initialised or not
    const int m_count = 0;              ///< Number of textures attached to this buffer
    const bool m_isBackBuffer = false;  ///< Whether this render target is the back buffer
    const bool m_multisampled = false;  ///< Whether this target has multisampling
    const std::string m_name;           ///< Name of the render target
    std::vector<GLuint> m_texturesMain; ///< Unique IDs of the main attached textures
    std::vector<GLuint> m_texturesAlt;  ///< Unique IDs of the alternative attached textures
    std::vector<GLenum> m_attachments;  ///< Container of attachment slots taken up
    std::vector<bool> m_highQuality;    ///< Whether to use high quality textures
    GLuint m_renderBuffer = 0;          ///< Unique ID of the buffer holding the depth information
    GLuint m_frameBuffer = 0;           ///< Unique ID of the frame buffer
};
