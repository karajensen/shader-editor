////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - cache.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "cache.h"

Cache::Cache() :
    m_applicatonRunning(true)
{

}

bool Cache::ApplicationRunning() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_applicatonRunning;
}

void Cache::SetApplicationRunning(bool run)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_applicatonRunning = run;
}