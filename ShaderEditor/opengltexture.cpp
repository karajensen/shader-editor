////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - opengltexture.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "opengltexture.h"
#include "opengl/soil/SOIL.h"

GlTexture::GlTexture(const Texture& texture) :
    m_texture(texture)
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

    auto type = m_texture.IsCubeMap() ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D;
    glBindTexture(type, m_id);

    if (m_texture.IsCubeMap())
    {
        InitialiseCubeMap();
    }
    else if (m_texture.HasPixels())
    {
        InitialiseFromPixels();
    }
    else
    {
        InitialiseFromFile();
    }

    glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(type, GL_TEXTURE_MAX_ANISOTROPY_EXT, static_cast<float>(MAX_ANISOTROPY));

    if(HasCallFailed())
    {
        Logger::LogError("OpenGL: Failed " + m_texture.Path() + " texture");
        return false;
    }

    return true;
}

void GlTexture::InitialiseCubeMap()
{
    LoadTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_X, m_texture.Path() + "_c00.png");
    LoadTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, m_texture.Path() + "_c01.png");
    LoadTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, m_texture.Path() + "_c02.png");
    LoadTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, m_texture.Path() + "_c03.png");
    LoadTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, m_texture.Path() + "_c04.png");
    LoadTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, m_texture.Path() + "_c05.png");
}

void GlTexture::InitialiseFromFile()
{
    LoadTexture(GL_TEXTURE_2D, m_texture.Path());
}

void GlTexture::InitialiseFromPixels()
{
    const int size = m_texture.Size();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    if(HasCallFailed())
    {
        Logger::LogError("OpenGL: Failed " + m_texture.Name() + " texture");
    }

    ReloadPixels();
}

void GlTexture::LoadTexture(GLenum type, const std::string& path)
{
    if (!boost::filesystem::exists(path))
    {
        Logger::LogError("OpenGL: " + path + " doesn't exist");
    }

    int width, height;
    unsigned char* image = SOIL_load_image(path.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
    glTexImage2D(type, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
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
    return m_texture.IsCubeMap();
}

void GlTexture::ReloadPixels()
{
    assert(!m_texture.HasPixels());

    glBindTexture(GL_TEXTURE_2D, m_id);

    //glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 
    //    m_texture.Size, m_texture.Size, GL_RGBA, GL_UNSIGNED_BYTE, 
    //    static_cast<unsigned int*>(&m_texture.Pixels[0]));

    if(HasCallFailed())
    {
        Logger::LogError("OpenGL: Failed " + 
            m_texture.Name() + " reloading");
    }
}