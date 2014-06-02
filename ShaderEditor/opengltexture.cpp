////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - opengltexture.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "opengltexture.h"
#include "opengl/soil/SOIL.h"

GlTexture::GlTexture(const std::string& filepath) :
    m_filepath(filepath),
    m_id(0),
    m_initialised(false)
{
}

GlTexture::~GlTexture()
{
    Release();
}

void GlTexture::Release()
{
    if(m_initialised)
    {
        glDeleteTextures(1, &m_id);
        m_initialised = false;
    }
}

bool GlTexture::Initialise()
{
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);

    int width, height;
    unsigned char* image = SOIL_load_image(m_filepath.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if(HasCallFailed())
    {
        Logger::LogError("OpenGL: Failed " + m_filepath + " texture");
        return false;
    }

    m_initialised = true;
    return true;
}

void GlTexture::SendTexture(int slot)
{
    assert(m_initialised);
    glActiveTexture(GetTexture(slot));
    glBindTexture(GL_TEXTURE_2D, m_id);
}