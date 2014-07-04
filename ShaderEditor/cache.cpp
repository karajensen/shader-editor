////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - cache.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "cache.h"

Cache::Cache() :
    m_applicatonRunning(true),
    m_deltaTime(0.0f)
{
}

bool Cache::ApplicationRunning() const
{
    std::lock_guard<std::mutex> lock(m_simMutex);
    return m_applicatonRunning;
}

void Cache::SetApplicationRunning(bool run)
{
    std::lock_guard<std::mutex> lock(m_simMutex);
    m_applicatonRunning = run;
}

float Cache::GetDeltaTime() const
{
    std::lock_guard<std::mutex> lock(m_simMutex);
    return m_deltaTime;
}

void Cache::SetDeltaTime(float dt)
{
    std::lock_guard<std::mutex> lock(m_simMutex);
    m_deltaTime = dt;
}

void Cache::SetMouse(const Float2& position, const Float2& direction)
{
    std::lock_guard<std::mutex> lock(m_simMutex);
    m_mouseDirection = direction;
    m_mousePosition = position;
}

Float2 Cache::GetMousePosition() const
{
    std::lock_guard<std::mutex> lock(m_simMutex);
    return m_mousePosition;
}

Float2 Cache::GetMouseDirection() const
{
    std::lock_guard<std::mutex> lock(m_simMutex);
    return m_mouseDirection;
}