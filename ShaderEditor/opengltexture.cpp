////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - opengltexture.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "opengltexture.h"
#include "opengl/soil/SOIL.h"
#include "boost/algorithm/string.hpp"

GlTexture::GlTexture(const std::string& filepath) :
    m_filepath(filepath)
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
    if (boost::icontains(m_filepath, ".dds"))
    {

    }
    else
    {
        m_initialised = true;
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
    }
    return true;
}

GLuint GlTexture::GetID() const
{
    assert(m_initialised);
    return m_id;
}