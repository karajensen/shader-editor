////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - animation.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "animation.h"
    
void Animation::AddFrame(int ID)
{
    m_frames.push_back(ID);
}

int Animation::GetFrame() const
{
    return m_frames[m_selectedFrame];
}

void Animation::SetSpeed(float speed)
{
    m_speed = speed;
}

float Animation::GetSpeed() const
{
    return m_speed;
}

void Animation::Tick(float deltatime)
{
    m_timePassed += deltatime;
    if (m_timePassed >= m_speed)
    {
        const int maxFrame = static_cast<int>(m_frames.size())-1;
        m_selectedFrame = m_selectedFrame == maxFrame ? 0 : m_selectedFrame + 1;
        m_timePassed = 0.0f;
    }
}
