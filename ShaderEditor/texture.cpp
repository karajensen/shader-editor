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
    case ENVIRONMENT:
        return "Environment";
    case OVERLAY:
        return "Overlay";
    default:
        return "None";
    };
}

void AnimatedTexture::AddFrame(int ID)
{
    m_frames.push_back(ID);
}

int AnimatedTexture::GetFrame() const
{
    return m_frames[m_selectedFrame];
}

void AnimatedTexture::Tick(float deltatime)
{
    m_timePassed += deltatime;

    const float secondsUntilSwitch = 0.05f;
    if (m_timePassed >= secondsUntilSwitch)
    {
        const int maxFrame = static_cast<int>(m_frames.size())-1;
        m_selectedFrame = m_selectedFrame == maxFrame ? 0 : m_selectedFrame + 1;
        m_timePassed = 0.0f;
    }
}