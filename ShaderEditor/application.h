////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - application.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <memory>
#include <Windows.h>
#include "elements.h"
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
    * Updates the post shared cache between the gui and application
    */
    void UpdatePost();

    /**
    * Initialises a render engine
    * @param engine the Engine to intialise
    * @return whether initialisation succeeded
    */
    bool InitialiseEngine(RenderEngine* engine);

    /**
    * Initialises the cache shared between the application and gui
    * @param engineNames The names of all engines supported
    */
    void InitialiseCache(const std::vector<std::string>& engineNames);

    /**
    * Recompiles the currently selected shader
    * @param text The new text to recompile with
    * @return whether recompilation was successful
    */
    bool ReCompileShader(const std::string& text);

    /**
    * @return the currently selected render engine
    */
    RenderEngine* GetEngine() const;

    /**
    * State for fading in/out the selected engine
    */
    enum FadeState
    {
        FADE_IN,
        FADE_OUT,
        NO_FADE
    };

    /**
    * Available render engines
    */
    enum
    {
        OPENGL,
        DIRECTX,
        MAX_ENGINES
    };

    PostProcessing m_postProcessing;  ///< Post processing for the final image
    FadeState m_fadeState = FADE_IN;  ///< Current state of fading in/out the selected engine
    int m_selectedLight = NO_INDEX;   ///< Current light selected
    int m_selectedMesh = NO_INDEX;    ///< Current mesh selected
    int m_selectedEngine = NO_INDEX;  ///< Current render engine selected
    int m_selectedShader = NO_INDEX;  ///< Current shader selected
    int m_selectedPost = NO_INDEX;    ///< Current post texture selected
    Float2 m_mouseDirection;          ///< Direction of movement for the mouse
    Float2 m_mousePosition;           ///< 2D coordinates of the mouse
    bool m_mousePressed = false;      ///< Whether the mouse is held down or not

    std::shared_ptr<Cache> m_cache;   ///< Shared data between the gui and application
    std::unique_ptr<Camera> m_camera; ///< Scene camera for generating view matrix
    std::unique_ptr<Scene> m_scene;   ///< Holds meshes, lighting and shader data
    std::unique_ptr<Timer> m_timer;   ///< For measure change in frame time
    
    std::vector<std::unique_ptr<RenderEngine>> m_engines; ///< Available render engines
};