////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - application.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <memory>
#include <Windows.h>
#include "common.h"

class OpenglEngine;
class DirectxEngine;
class VulkanEngine;
class RenderEngine;
class Timer;
class Scene;
class AppGUI;
class Camera;
struct Cache;

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
    * Main loop for the application
    */
    void Run();

    /**
    * Initialise the world
    * @param hwnd The handle to the window
    * @param hinstance Handle to the current instance of the application
    * @param cache Shared data between the gui and application
    * @return whether or not initialisation succeeded
    */
    bool Initialise(HWND hwnd,
                    HINSTANCE hinstance, 
                    std::shared_ptr<Cache> cache);

private: 

    /**
    * Switches to a new render engine
    * @param index The index of the engine to switch to
    */
    void SwitchRenderEngine(int index);

    /**
    * Forces a direct switch to a render engine without fade
    * @param index The index of the engine to switch to
    */
    void ForceRenderEngine(int index);

    /**
    * Updates and renders the application
    */
    void TickApplication();

    /**
    * Fades in or out if required for the selected render engine
    * @note when completing a fade out the render engine is release and switched
    */
    void FadeRenderEngine();

    /**
    * Handles any custom input events
    * @param keydown The current key being held
    * @param msg The windows event message
    */
    void HandleInputEvents(WPARAM& keydown, const MSG& msg);

    /**
    * Handles any key presses
    * @param keypress The current key being pressed
    */
    void HandleKeyPress(const WPARAM& keypress);

    /**
    * Handles any keys that are currently down
    */
    void HandleKeyDown();

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
    * Initialises a render engine
    * @param engine the Engine to intialise
    * @return whether initialisation succeeded
    */
    bool InitialiseEngine(RenderEngine& engine);

    /**
    * @return the currently selected render engine
    */
    RenderEngine& GetEngine() const;

    /**
    * State for fading in/out the selected engine
    */
    enum FadeState
    {
        FADE_IN,
        FADE_OUT,
        NO_FADE
    };

    FadeState m_fadeState = FADE_IN;             ///< Current state of fading in/out the selected engine
    Float2 m_mouseDirection;                     ///< Direction of movement for the mouse
    Float2 m_mousePosition;                      ///< 2D coordinates of the mouse
    bool m_mousePressed = false;                 ///< Whether the mouse is held down or not
    int m_selectedEngine = NO_INDEX;             ///< Currently selected engine
    std::unique_ptr<Camera> m_camera;            ///< Scene camera for generating view matrix
    std::unique_ptr<Scene> m_scene;              ///< Holds meshes, lighting and shader data
    std::unique_ptr<Timer> m_timer;              ///< For measure change in frame time
    std::unique_ptr<AppGUI> m_modifier;   ///< Manipulates meshes, lighting and shader data
    
    std::vector<std::unique_ptr<RenderEngine>> m_engines; ///< Available render engines
};