////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - openglcommon.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "opengltarget.h"

GlRenderTarget::GlRenderTarget(const std::string& name, bool isBackBuffer) :
    m_isBackBuffer(isBackBuffer),
    m_name(name)
{
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
        glDeleteTextures(1, &m_colorTexture);
        glDeleteRenderbuffers(1, &m_renderBuffer);
    }
    m_initialised = false;
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

        // Create the colour texture
        glGenTextures(1, &m_colorTexture);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_colorTexture);

        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 
            MULTISAMPLING_COUNT, GL_RGBA, WINDOW_WIDTH, WINDOW_HEIGHT, GL_TRUE);  

        glFramebufferTexture2D(GL_FRAMEBUFFER, 
            GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_colorTexture, 0);

        if(HasCallFailed())
        {
            Logger::LogError(m_name + " Failed to create color texture");
            return false;
        }

        // Create the render buffer to hold depth information
        glGenRenderbuffers(1, &m_renderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer);

        glRenderbufferStorageMultisample(GL_RENDERBUFFER, MULTISAMPLING_COUNT, 
            GL_DEPTH24_STENCIL8, WINDOW_WIDTH, WINDOW_HEIGHT);

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

    m_initialised = true;
    return true;
}

void GlRenderTarget::SetActive()
{
    assert(m_initialised);

    if(m_isBackBuffer)
    {
        glDisable(GL_MULTISAMPLE);  
        glDisable(GL_DEPTH_TEST);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    else
    {
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_DEPTH_TEST);
        glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    if(HasCallFailed())
    {
        Logger::LogError("Could not set " + m_name + " as active");
    }
}