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

#pragma region GUI_THREAD_ACCESS

    /**
    * @return whether the application is running
    * @note accessed solely by the gui thread
    */
    bool ApplicationRunning() const;

    /**
    * @return the time passed in seconds between ticks
    * @note accessed solely by the gui thread
    */
    float GetDeltaTime() const;

    /**
    * @return The screen position of the mouse
    * @note accessed solely by the gui thread
    */
    Float2 GetMousePosition() const;

    /**
    * @return The direction normalized of the mouse
    * @note accessed solely by the gui thread
    */
    Float2 GetMouseDirection() const;

#pragma endregion
#pragma region SIM_THREAD_ACCESS

    /**
    * @param run Whether the application is running
    * @note accessed solely by the application thread
    */
    void SetApplicationRunning(bool run);

    /**
    * @param dt The time passed in seconds between ticks
    * @note accessed solely by the application thread
    */
    void SetDeltaTime(float dt);

    /**
    * Sets the coordinates for the mouse
    * @param position The screen position of the mouse
    * @param direction The direction normalized of the mouse
    */
    void SetMouse(const Float2& position, const Float2& direction);

#pragma endregion
    
private:

    mutable std::mutex m_guiMutex;   ///< Mutex for accessing gui data
    mutable std::mutex m_simMutex;   ///< Mutex for accessing simulation data

    bool m_applicatonRunning;   ///< Whether the application is running
    float m_deltaTime;          ///< The time passed in seconds between ticks
    Float2 m_mousePosition;     ///< The screen position of the mouse
    Float2 m_mouseDirection;    ///< The direction normalized of the mouse
};