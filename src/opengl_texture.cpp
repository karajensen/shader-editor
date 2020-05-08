////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - opengl_texture.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "opengl_texture.h"

#include "soil/SOIL.h"

#include <boost/filesystem.hpp>

GlTexture::GlTexture(const Texture& texture)
    : m_texture(texture)
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
    if (m_texture.IsRenderable())
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

        return CreateMipMaps() && !HasCallFailed();
    }
    return true;
}

void GlTexture::InitialiseCubeMap()
{
    const std::string path(m_texture.Path() + "_c0");
    LoadTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_X, path + "0.png");
    LoadTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, path + "1.png");
    LoadTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, path + "2.png");
    LoadTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, path + "3.png");
    LoadTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, path + "4.png");
    LoadTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, path + "5.png");
}

void GlTexture::InitialiseFromFile()
{
    LoadTexture(GL_TEXTURE_2D, m_texture.Path());
}

void GlTexture::InitialiseFromPixels()
{
    const int size = m_texture.Size();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
        size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    if(HasCallFailed())
    {
        Logger::LogError("OpenGL: Failed to load " + m_texture.Name());
    }

    if (!ReloadPixels())
    {
        Logger::LogError("OpenGL: Failed " + 
            m_texture.Name() + " reloading");
    }

    SetFiltering();
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
        Logger::LogError("OpenGL: Failed to load " + path + " texture");
    }

    SetFiltering();
}

void GlTexture::SetFiltering()
{
    const auto type = IsCubeMap() ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D;

    const auto filter = m_texture.Filtering();
    const auto magFilter = filter == Texture::NEAREST ? GL_NEAREST : GL_LINEAR;
    const auto minFilter = filter == Texture::NEAREST ? GL_NEAREST : GL_LINEAR_MIPMAP_LINEAR;

    glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(type, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, magFilter);

    if (filter == Texture::ANISOTROPIC)
    {
        glTexParameterf(type, GL_TEXTURE_MAX_ANISOTROPY_EXT, 
            static_cast<float>(MAX_ANISOTROPY));
    }

    if(HasCallFailed())
    {
        Logger::LogError("OpenGL: Failed filtering for " + m_texture.Name());
    }
}

bool GlTexture::CreateMipMaps()
{
    if (m_texture.Filtering() != Texture::NEAREST)
    {
        glGenerateMipmap(IsCubeMap() ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D);

        if(HasCallFailed())
        {
            Logger::LogError("OpenGL: Mipmap creation failed for " + m_texture.Name());
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

bool GlTexture::IsCubeMap() const
{
    return m_texture.IsCubeMap();
}

bool GlTexture::ReloadPixels()
{
    if (m_texture.IsRenderable())
    {
        assert(m_texture.HasPixels());

        glBindTexture(GL_TEXTURE_2D, m_id);

        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_texture.Size(), 
            m_texture.Size(), GL_RGBA, GL_UNSIGNED_BYTE, &m_texture.Pixels()[0]);

        return !HasCallFailed();        
    }
    return true;
}

const std::string& GlTexture::Name() const
{
    return m_texture.Name();
}