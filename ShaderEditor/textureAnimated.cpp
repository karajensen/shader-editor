////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - textureAnimated.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "textureAnimated.h"
#include "boost/lexical_cast.hpp"
#include "boost/filesystem.hpp"
#include "common.h"

AnimatedTexture::AnimatedTexture(const std::string& path, 
                                 const std::string& name,
                                 const std::string& extension)
{
    int frame = 0;
    bool loading = true;
    std::string loaded = "0";

    while (loading)
    {
        ++frame;
        const std::string number(boost::lexical_cast<std::string>(frame));
        const std::string fullname(name + (frame < 10 ? "0" : "") + number + extension);
        const std::string fullpath(path + fullname);

        if (boost::filesystem::exists(fullpath))
        {
            loaded = number;
            m_paths.push_back(fullpath);
        }
        else
        {
            loading = false;
            Logger::LogInfo("Animation: " + name + " Initialised " + loaded + " frames");
        }
    }
}

const std::vector<std::string>& AnimatedTexture::Paths() const
{
    return m_paths;
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
    if (m_timePassed >= m_speed)
    {
        const int maxFrame = static_cast<int>(m_frames.size())-1;
        m_selectedFrame = m_selectedFrame == maxFrame ? 0 : m_selectedFrame + 1;
        m_timePassed = 0.0f;
    }
}

void AnimatedTexture::SetSpeed(float speed)
{
    m_speed = speed;
}

float AnimatedTexture::GetSpeed() const
{
    return m_speed;
}