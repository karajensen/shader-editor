////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - openglcommon.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "opengltarget.h"

GlRenderTarget::GlRenderTarget(const std::string& name) :
    m_isBackBuffer(true),
    m_name(name),
    m_count(0)
{
}

GlRenderTarget::GlRenderTarget(const std::string& name, int textures) :
    m_isBackBuffer(false),
    m_name(name),
    m_count(textures)
{
    m_attachments.resize(m_count);
    m_textures.resize(m_count);
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
        for (GLuint texture : m_textures)
        {
            glDeleteTextures(1, &texture);
        }
        glDeleteRenderbuffers(1, &m_renderBuffer);
    }
    m_initialised = false;
}

GLuint GlRenderTarget::GetTexture(int index) const
{ 
    return m_textures[index]; 
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

        for (unsigned int i = 0; i < m_textures.size(); ++i)
        {
            m_attachments[i] = GetTextureAttachment(i);
            glGenTextures(1, &m_textures[i]);
            glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_textures[i]);

            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 
                MULTISAMPLING_COUNT, GL_RGBA32F, WINDOW_WIDTH, WINDOW_HEIGHT, GL_TRUE);  

            glFramebufferTexture2D(GL_FRAMEBUFFER, m_attachments[i], 
                GL_TEXTURE_2D_MULTISAMPLE, m_textures[i], 0);

            if(HasCallFailed())
            {
                const auto ID = boost::lexical_cast<std::string>(i);
                Logger::LogError(m_name + " Failed to create texture " + ID);
                return false;
            }
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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
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