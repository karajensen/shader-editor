////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - timer.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <Windows.h>
#include <string>

struct CTwBar;

/**
* FPS class for determining frame rate and delta time
*/
class Timer
{
public:

    /**
    * Constructor
    */
    Timer();

    /**
    * Starts the initial ticking of the timer
    */
    void StartTimer();

    /**
    * Stops/Starts the timer to determine delta-time and fps
    */
    void UpdateTimer();

    /**
    * @return The time passed since last frame in seconds
    */
    float GetDeltaTime() const;

    /**
    * Adds tweakable timer parameters to the tweak bar
    * @param tweakbar Used for runtime diagnostics
    * @param group The header to put any diagnostics under
    */
    void InitialiseTweakBar(CTwBar* tweakbar, const std::string& group);

private:

    double m_frequency;         ///< The frequency of the high-resolution performance counter
    LARGE_INTEGER m_timer;      ///< The current time queried
    double m_previousTime;      ///< The previous time queried
    double m_deltaTime;         ///< The time passed since last frame in seconds
    double m_deltaTimeCounter;  ///< Combined timestep between frames up to 1 second
    unsigned int m_fps;         ///< Amount of frames rendered in 1 second
    unsigned int m_fpsCounter;  ///< Amount of frames rendered since delta time counter began
};

