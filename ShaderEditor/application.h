////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - application.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <memory>
#include <Windows.h>
#include "common.h"

class OpenglEngine;
class DirectxEngine;
class RenderEngine;
class Timer;
class Scene;
class Camera;
class Cache;

/**
* Main application class
*/
class Application
{
public:

    /**
    * Constructor
    * @param cache Shared data between the gui and application
    */
    Application(std::shared_ptr<Cache> cache);

    /**
    * Destructor
    */
    ~Application();

    /**
    * Main loop for the application
    */
    void Run();

    /**
    * Initialise the world
    * @param hwnd The handle to the window
    * @param hinstance Handle to the current instance of the application
    * @return whether or not initialisation succeeded
    */
    bool Initialise(HWND hwnd, HINSTANCE hinstance);

private: 

    /**
    * Switches to the currently set render engine
    */
    void SwitchRenderEngine();

    /**
    * Updates and renders the application
    */
    void TickApplication();

    /**
    * Handles any custom input events
    * @param keydown The current key being pressed
    * @param msg The windows event message
    */
    void HandleInputEvents(WPARAM& keydown, const MSG& msg);

    /**
    * Handles any custom key press events
    * @param keydown The current key being pressed
    */
    void HandleKeyPress(const WPARAM& keydown);

    /**
    * Determines the direction and position of movement for the mouse
    * @param msg The windows event message
    */
    void HandleMouseMovement(const MSG& msg);

    /**
    * @return whether the vk key code is current pressed
    * @param key The VK_ key code to query
    */
    bool IsKeyDown(unsigned int key) const;

    /**
    * Initialises the OpenGL Render Engine
    * @return whether initialisation succeeded
    */
    bool InitialiseOpenGL();

    /**
    * Initialises the DirectX Render Engine
    * @return whether initialisation succeeded
    */
    bool InitialiseDirectX();

    RenderEngine* m_engine;  ///< currently selected rendering engine
    bool m_mousePressed;     ///< Whether the mouse is held down or not
    Float2 m_mouseDirection; ///< Direction of movement for the mouse
    Float2 m_mousePosition;  ///< 2D coordinates of the mouse

    std::shared_ptr<Cache> m_cache;           ///< Shared data between the gui and application
    std::unique_ptr<Camera> m_camera;         ///< Scene camera for generating view matrix
    std::unique_ptr<Scene> m_scene;           ///< Holds meshes, lighting and shader data
    std::unique_ptr<Timer> m_timer;           ///< For measure change in frame time
    std::unique_ptr<OpenglEngine> m_opengl;   ///< OpenGL rendering engine
    std::unique_ptr<DirectxEngine> m_directx; ///< DirectX rendering engine
};