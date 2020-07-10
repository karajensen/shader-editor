////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - openglt_arget.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "opengl_target.h"

#include <boost/lexical_cast.hpp>

#include <algorithm>

GlRenderTarget::GlRenderTarget(const std::string& name)
    : m_isBackBuffer(true)
    , m_name(name)
{
}

GlRenderTarget::GlRenderTarget(const std::string& name, 
                               int textures, 
                               bool multisampled, 
                               bool readWrite)

    : m_multisampled(multisampled)
    , m_name(name)
    , m_count(textures)
    , m_readWrite(readWrite)
{
    m_attachments.resize(m_count);
    m_texturesMain.resize(m_count);

    m_highQuality.resize(m_count);
    m_highQuality.assign(m_count, false);

    if (m_readWrite)
    {
        m_texturesAlt.resize(m_count);
    }
}

GlRenderTarget::~GlRenderTarget()
{
    Release();
}

void GlRenderTarget::Release()
{
    if(m_initialised && !m_isBackBuffer)
    {
        glDeleteFramebuffers(1, &m_frameBuffer);
        for (GLuint texture : m_texturesMain)
        {
            glDeleteTextures(1, &texture);
        }
        for (GLuint texture : m_texturesAlt)
        {
            glDeleteTextures(1, &texture);
        }
        glDeleteRenderbuffers(1, &m_renderBuffer);
    }
    m_initialised = false;
}

GLuint GlRenderTarget::GetTexture(int index) const
{ 
    return m_readWrite && !m_mainAttached ? 
        m_texturesAlt[index] : m_texturesMain[index];
}

bool GlRenderTarget::Initialise()
{
    if(!m_isBackBuffer)
    {
        glGenFramebuffers(1, &m_frameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

        if(HasCallFailed())
        {
            Logger::LogError(m_name + " Render target creation failed");
            return false;
        }

        const unsigned int textureType = m_multisampled ? 
            GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

        for (unsigned int i = 0; i < m_texturesAlt.size(); ++i)
        {
            if (!CreateTexture(m_texturesAlt[i], textureType, m_highQuality[i]))
            {
                return false;
            }
        }

        for (unsigned int i = 0; i < m_texturesMain.size(); ++i)
        {
            if (!CreateTexture(m_texturesMain[i], textureType, m_highQuality[i]))
            {
                return false;
            }

            m_attachments[i] = GetTextureAttachment(i);
            glFramebufferTexture2D(GL_FRAMEBUFFER, 
                m_attachments[i], textureType, m_texturesMain[i], 0);

            if(HasCallFailed())
            {
                const auto ID = boost::lexical_cast<std::string>(i);
                Logger::LogError(m_name + " Failed to create texture buffer" + ID);
                return false;
            }
        }

        // Create the render buffer to hold depth information
        glGenRenderbuffers(1, &m_renderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer);

        if (m_multisampled)
        {
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, MULTISAMPLING_COUNT, 
                GL_DEPTH_COMPONENT24, WINDOW_WIDTH, WINDOW_HEIGHT);
        }
        else
        {
            glRenderbufferStorage(GL_RENDERBUFFER,
                GL_DEPTH_COMPONENT24, WINDOW_WIDTH, WINDOW_HEIGHT);
        }

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, 
            GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_renderBuffer);

        if(HasCallFailed())
        {
            Logger::LogError(m_name + " Failed to create depth texture");
            return false;
        }

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            Logger::LogError(m_name + " sampling does not match across attachments");
            return false;
        }
    }

    m_mainAttached = true;
    m_initialised = true;
    return true;
}

void GlRenderTarget::SetHighQuality(int index)
{
    m_highQuality[index] = true;
}

bool GlRenderTarget::CreateTexture(GLuint& id, unsigned int type, bool highQuality)
{
    glGenTextures(1, &id);
    glBindTexture(type, id);

    if (m_multisampled)
    {
        glTexImage2DMultisample(type, MULTISAMPLING_COUNT, 
            highQuality ? GL_RGBA32F : GL_RGBA, WINDOW_WIDTH, WINDOW_HEIGHT, GL_TRUE);  
    }
    else
    {
        glTexImage2D(type, 0, GL_RGBA, WINDOW_WIDTH,
            WINDOW_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    if(HasCallFailed())
    {
        Logger::LogError(m_name + " Failed to create texture");
        return false;
    }

    return true;
}

bool GlRenderTarget::IsMultisampled() const
{
    return m_multisampled;
}

void GlRenderTarget::SwitchTextures()
{
    assert(!m_multisampled);
    assert(m_readWrite);

    m_mainAttached = !m_mainAttached;

    for (unsigned int i = 0; i < m_texturesMain.size(); ++i)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, m_attachments[i], GL_TEXTURE_2D, 
            m_mainAttached ? m_texturesMain[i] : m_texturesAlt[i], 0);

        if(HasCallFailed())
        {
            Logger::LogError(m_name + " Failed to switch textures");
        }
    }
}

void GlRenderTarget::SetActive()
{
    assert(m_initialised);

    m_multisampled ? glEnable(GL_MULTISAMPLE) : glDisable(GL_MULTISAMPLE); 

    if(m_isBackBuffer)
    {
        glDisable(GL_DEPTH_TEST);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    else
    {
        glEnable(GL_DEPTH_TEST);
        glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawBuffers(m_count, &m_attachments[0]);
    }

    if(HasCallFailed())
    {
        Logger::LogError("Could not set " + m_name + " as active");
    }
}

unsigned int GlRenderTarget::GetTextureAttachment(int index) const
{
    switch (index)
    {
    case 0:
        return GL_COLOR_ATTACHMENT0_EXT;
    case 1:
        return GL_COLOR_ATTACHMENT1_EXT;
    case 2:
        return GL_COLOR_ATTACHMENT2_EXT;
    case 3:
        return GL_COLOR_ATTACHMENT3_EXT;
    case 4:
        return GL_COLOR_ATTACHMENT4_EXT;
    default:
        Logger::LogError("Unsupported index for render target texture");
        return 0;
    }
}
