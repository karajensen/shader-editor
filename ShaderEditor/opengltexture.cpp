////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - opengltexture.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "opengltexture.h"
#include "opengl/soil/SOIL.h"
#include "boost/algorithm/string.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/filesystem.hpp"

GlTexture::GlTexture(const std::string& filepath) :
    m_filepath(filepath),
    m_isCubeMap(boost::filesystem::path(filepath).extension().string().empty())
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
    m_initialised = true;

    auto type = m_isCubeMap ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D;
    glBindTexture(type, m_id);

    if (m_isCubeMap)
    {
        LoadTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_X, m_filepath + "_c00.png");
        LoadTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, m_filepath + "_c01.png");
        LoadTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, m_filepath + "_c02.png");
        LoadTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, m_filepath + "_c03.png");
        LoadTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, m_filepath + "_c04.png");
        LoadTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, m_filepath + "_c05.png");
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, m_id);
        LoadTexture(GL_TEXTURE_2D, m_filepath);
    }

    glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(type, GL_TEXTURE_MAX_ANISOTROPY_EXT, MAX_ANISOTROPY);

    if(HasCallFailed())
    {
        Logger::LogError("OpenGL: Failed " + m_filepath + " texture");
        return false;
    }

    return true;
}

void GlTexture::LoadTexture(GLenum type, const std::string& path)
{
    if (!boost::filesystem::exists(path))
    {
        Logger::LogError("OpenGL: " + path + " doesn't exist");
    }

    int width, height;
    unsigned char* image = SOIL_load_image(path.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(type, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);

    if(HasCallFailed())
    {
        Logger::LogError("OpenGL: Failed " + path + " texture");
    }
}

GLuint GlTexture::GetID() const
{
    assert(m_initialised);
    return m_id;
}

bool GlTexture::IsCubeMap() const
{
    return m_isCubeMap;
}