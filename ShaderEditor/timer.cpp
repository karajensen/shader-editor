////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - timer.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "timer.h"

namespace
{
    const float DT_MAXIMUM = 0.03f;   ///< Maximum allowed deltatime
    const float DT_MINIMUM = 0.01f;   ///< Minimum allowed deltatime
}

void Timer::StartTimer()
{
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&m_timer);

    m_frequency = static_cast<float>(frequency.QuadPart);
    m_previousTime = static_cast<float>(m_timer.QuadPart);
}

void Timer::UpdateTimer()
{
    QueryPerformanceCounter(&m_timer);
    float currentTime = static_cast<float>(m_timer.QuadPart);

    float deltatime = (currentTime - m_previousTime) / m_frequency;
    m_deltaTimeCounter += deltatime;
    if (m_deltaTimeCounter >= 1.0) //one second has passed
    {
        m_deltaTimeCounter = 0.0;
        m_fps = m_fpsCounter;
        m_fpsCounter = 0;
    }

    m_totalTime += m_deltaTime;
    m_totalTime = _finite(m_totalTime) ? m_totalTime : 0.0f;

    m_deltaTime = max(deltatime, DT_MINIMUM);
    m_deltaTime = min(m_deltaTime, DT_MAXIMUM);
    
    ++m_fpsCounter; 
    m_previousTime = currentTime;
}

float Timer::GetTotalTime() const
{
    return m_totalTime;
}

float Timer::GetDeltaTime() const 
{ 
    return m_deltaTime;
}

int Timer::GetFPS() const
{
    return static_cast<int>(m_fps);
}

int Timer::GetCappedFPS() const
{
    return min(static_cast<int>(m_fps), 60);
}