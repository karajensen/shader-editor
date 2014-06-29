////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - cache.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "common.h"
#include <thread>

/**
* Manages information sending between the gui and application
*/
class Cache
{
public:

    /**
    * Constructor
    */ 
    Cache();

    /**
    * @return whether the application is running
    * @note accessed solely by the gui thread
    */
    bool ApplicationRunning() const;

    /**
    * @param run Whether the application is running
    * @note accessed solely by the application thread
    */
    void SetApplicationRunning(bool run);
    
private:

    mutable std::mutex m_mutex;   ///< Mutex for accessing cached data
    bool m_applicatonRunning;     ///< Whether the application is running
};