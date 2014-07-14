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
struct Cache;

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
    * Switches to a new render engine
    * @param index The index of the engine to switch to
    */
    void SwitchRenderEngine(int index);

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
    * Updates the shader shared cache between the gui and application
    */
    void UpdateShader();

    /**
    * Updates the scene shared cache between the gui and application
    */
    void UpdateScene();

    /**
    * Updates the light shared cache between the gui and application
    */
    void UpdateLight();

    /**
    * Updates the mesh shared cache between the gui and application
    */
    void UpdateMesh();

    /**
    * Initialises a render engine
    * @param engine the Engine to intialise
    * @return whether initialisation succeeded
    */
    bool InitialiseEngine(RenderEngine* engine);

    /**
    * @return the currently selected render engine
    */
    RenderEngine* GetEngine() const;

    /**
    * Available render engines
    */
    enum
    {
        OPENGL,
        DIRECTX,
        MAX_ENGINES
    };

    bool m_mousePressed;              ///< Whether the mouse is held down or not
    Float2 m_mouseDirection;          ///< Direction of movement for the mouse
    Float2 m_mousePosition;           ///< 2D coordinates of the mouse
    int m_selectedLight;              ///< Current light selected
    int m_selectedMesh;               ///< Current mesh selected
    int m_selectedEngine;             ///< Current render engine selected
    int m_selectedShader;             ///< Current shader selected
    std::shared_ptr<Cache> m_cache;   ///< Shared data between the gui and application
    std::unique_ptr<Camera> m_camera; ///< Scene camera for generating view matrix
    std::unique_ptr<Scene> m_scene;   ///< Holds meshes, lighting and shader data
    std::unique_ptr<Timer> m_timer;   ///< For measure change in frame time
    
    std::vector<std::unique_ptr<RenderEngine>> m_engines; ///< Available render engines
};