////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - texture.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "texture.h"

Texture::Texture(const std::string& name, const std::string& path) :
    m_name(name),
    m_path(path)
{
}

const std::string& Texture::Name() const
{
    return m_name;
}

const std::string& Texture::Path() const
{
    return m_path;
}

std::string Texture::GetTypeDescription(unsigned int type)
{
    switch (type)
    {
    case DIFFUSE:
        return "Diffuse";
    case NORMAL:
        return "Normal";
    case SPECULAR:
        return "Specular";
    case GLOW:
        return "Glow";
    case ENVIRONMENT:
        return "Environment";
    default:
        return "None";
    };
}