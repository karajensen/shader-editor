////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - timer.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "timer.h"

namespace
{
    const double DT_MAXIMUM = 0.03;   ///< Maximum allowed deltatime
    const double DT_MINIMUM = 0.01;   ///< Minimum allowed deltatime
}

Timer::Timer() :
    m_frequency(0.0),
    m_previousTime(0.0),
    m_deltaTime(0.0),
    m_deltaTimeCounter(0.0),
    m_fps(0),
    m_fpsCounter(0)
{
}

void Timer::StartTimer()
{
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&m_timer);

    m_frequency = static_cast<double>(frequency.QuadPart);
    m_previousTime = static_cast<double>(m_timer.QuadPart);
}

void Timer::UpdateTimer()
{
    QueryPerformanceCounter(&m_timer);
    double currentTime = static_cast<double>(m_timer.QuadPart);

    double deltatime = (currentTime - m_previousTime) / m_frequency;
    m_deltaTimeCounter += deltatime;
    if (m_deltaTimeCounter >= 1.0) //one second has passed
    {
        m_deltaTimeCounter = 0.0;
        m_fps = m_fpsCounter;
        m_fpsCounter = 0;
    }

    m_deltaTime = max(deltatime, DT_MINIMUM);
    m_deltaTime = min(m_deltaTime, DT_MAXIMUM);
    
    ++m_fpsCounter; //increment frame counter
    m_previousTime = currentTime;
}

float Timer::GetDeltaTime() const 
{ 
    return static_cast<float>(m_deltaTime); 
}