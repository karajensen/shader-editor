////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - application.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <memory>

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
    * @return whether or not initialisation succeeded
    */
    bool Initialise();

private:

    std::unique_ptr<OpenglEngine> m_opengl; ///< OpenGL rendering engine
    RenderEngine* m_engine; ///< currently selected rendering engine

};