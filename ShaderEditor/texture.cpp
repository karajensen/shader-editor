////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - texture.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "texture.h"
#include "common.h"
#include "boost/algorithm/string.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/filesystem.hpp"

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
    case ENVIRONMENT:
        return "Environment";
    case CAUSTICS:
        return "Caustics";
    default:
        return "None";
    };
}

bool Texture::IsAnimatedType(unsigned int type)
{
    return type == CAUSTICS;
}

bool Texture::IsCubeMap() const
{
    return boost::filesystem::path(m_path).extension().string().empty();
}

const unsigned int* Texture::Pixels() const
{
    Logger::LogError("Texture::Pixels Not Implemented");
    return nullptr;
}

bool Texture::HasPixels() const
{
    return false;
}

int Texture::Size() const
{
    Logger::LogError("Texture::Size Not Implemented");
    return 0;
}