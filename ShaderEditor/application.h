////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - application.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <memory>
#include <Windows.h>

class Diagnostics;
class OpenglEngine;
class RenderEngine;
class LightManager;
class Timer;

/**
* Main application class
*/
class Application
{
public:

    /**
    * Constructor
    */
    Application();

    /**
    * Destructor
    */
    ~Application();

    /**
    * @return whether something went wrong during the 
    * game or the game has exited successfully
    */
    bool Run();

    /**
    * Initialise the world
    * @param hwnd The handle to the window
    * @return whether or not initialisation succeeded
    */
    bool Initialise(HWND hwnd);

private:

    /**
    * Updates and renders the application
    */
    void TickApplication();

    /**
    * Sets which is the currently active render engine
    * @param opengl Whether to use opengl or directx
    */
    void SetRenderEngine(bool opengl);


    bool m_runApplication; ///< Whether the application is running or not
    RenderEngine* m_engine; ///< currently selected rendering engine

    std::unique_ptr<Timer> m_timer; ///< For measure change in frame time
    std::unique_ptr<OpenglEngine> m_opengl; ///< OpenGL rendering engine
    std::unique_ptr<LightManager> m_lighting; ///< Manages and owns all scene lights
    std::unique_ptr<Diagnostics> m_diagnostics; ///< Holds tweak bar and scene diagnostics
};