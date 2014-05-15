////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - opengltexture.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "opengltexture.h"
#include "opengl/soil/SOIL.h"

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
}

void GlTexture::Initialise(GLuint id)
{
    m_id = id;
    glBindTexture(GL_TEXTURE_2D, m_id);

    int width, height;
    unsigned char* image = SOIL_load_image(m_filepath.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void GlTexture::SendTexture(int slot)
{
    switch(slot)
    {
    case 0:
        glActiveTexture(GL_TEXTURE0);
        break;
    case 1:
        glActiveTexture(GL_TEXTURE1);
        break;
    case 2:
        glActiveTexture(GL_TEXTURE2);
        break;
    case 3:
        glActiveTexture(GL_TEXTURE3);
        break;
    case 4:
        glActiveTexture(GL_TEXTURE4);
        break;
    case 5:
        glActiveTexture(GL_TEXTURE5);
        break;
    case 6:
        glActiveTexture(GL_TEXTURE6);
        break;
    case 7:
        glActiveTexture(GL_TEXTURE7);
        break;
    case 8:
        glActiveTexture(GL_TEXTURE8);
        break;
    default:
        Logger::LogError("Unknown texture slot");
    }

    glBindTexture(GL_TEXTURE_2D, m_id);
}