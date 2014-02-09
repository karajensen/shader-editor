////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - application.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <memory>
#include <Windows.h>

class OpenglEngine;
class RenderEngine;

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

    bool m_runApplication; ///< Whether the application is running or not
    std::unique_ptr<OpenglEngine> m_opengl; ///< OpenGL rendering engine
    RenderEngine* m_engine; ///< currently selected rendering engine

};