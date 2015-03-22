////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - texture.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "texture.h"
#include "common.h"
#include "boost/algorithm/string.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/filesystem.hpp"

Texture::Texture(const std::string& name, const std::string& path, Filter filter) :
    m_name(name),
    m_path(path),
    m_filter(filter)
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

Texture::Filter Texture::Filtering() const
{
    return m_filter;
}

bool Texture::IsAnimatedType(unsigned int type)
{
    return type == CAUSTICS;
}

bool Texture::IsCubeMap(const std::string& path)
{
    return boost::filesystem::path(path).extension().string().empty();
}

bool Texture::IsCubeMap() const
{
    return IsCubeMap(m_path);
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