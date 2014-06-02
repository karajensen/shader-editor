////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - openglcommon.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "opengltarget.h"

bool GlRenderTarget::sm_depthEnabled = false;

GlRenderTarget::GlRenderTarget(const std::string& name, bool isBackBuffer) :
    m_isBackBuffer(isBackBuffer),
    m_name(name),
    m_colorBuffer(0),
    m_depthBuffer(0),
    m_frameBuffer(0),
    m_initialised(false)
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
        glDeleteTextures(1, &m_colorBuffer);
        glDeleteRenderbuffers(1, &m_depthBuffer);
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

        glGenTextures(1, &m_colorBuffer);
        glBindTexture(GL_TEXTURE_2D, m_colorBuffer);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WINDOW_WIDTH, 
            WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, 
            GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorBuffer, 0);

        if(HasCallFailed())
        {
            Logger::LogError(m_name + " Render target colour buffer failed");
            return false;
        }

        glGenRenderbuffers(1, &m_depthBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);

        glRenderbufferStorage(GL_RENDERBUFFER, 
            GL_DEPTH24_STENCIL8, WINDOW_WIDTH, WINDOW_HEIGHT);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, 
            GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);

        if(HasCallFailed())
        {
            Logger::LogError(m_name + " Render target depth buffer failed");
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
        if(sm_depthEnabled)
        {
            glDisable(GL_DEPTH_TEST);
            sm_depthEnabled = false;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    else
    {
        if(!sm_depthEnabled)
        {
            glEnable(GL_DEPTH_TEST);
            sm_depthEnabled = true;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}

void GlRenderTarget::SendTexture(int slot)
{
    glActiveTexture(GetTexture(slot));
    glBindTexture(GL_TEXTURE_2D, m_colorBuffer);
}

void GlRenderTarget::ClearTexture(int slot)
{
    glActiveTexture(GetTexture(slot));
    glBindTexture(GL_TEXTURE_2D, 0);
}