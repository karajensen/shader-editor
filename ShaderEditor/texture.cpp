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
    m_filter(filter),
    m_type(IsCubeMap(path) ? CUBE : FROM_FILE)
{
}

void Texture::Write(Cache& cache)
{
}

void Texture::Read(Cache& cache)
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

Texture::Filter Texture::Filtering() const
{
    return m_filter;
}

bool Texture::IsCubeMap(const std::string& path)
{
    return boost::filesystem::path(path).extension().string().empty();
}

bool Texture::IsCubeMap() const
{
    return m_type == CUBE;
}

const std::vector<unsigned int>& Texture::Pixels() const
{
    throw std::exception("Texture::Pixels not implemented");
}

bool Texture::HasPixels() const
{
    return false;
}

int Texture::Size() const
{
    throw std::exception("Texture::Size not implemented");
}

void Texture::Reload()
{
    throw std::exception("Texture::Reload not implemented");
}

void Texture::Save()
{
    throw std::exception("Texture::Save not implemented");
}

bool Texture::IsRenderable() const
{
    return true;
}

void Texture::SetType(Type type)
{
    m_type = type;
}