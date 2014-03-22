////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - application.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <memory>
#include <Windows.h>
#include "common.h"

struct CTwBar;
class OpenglEngine;
class DirectxEngine;
class RenderEngine;
class Timer;
class Scene;
class Camera;

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

    /**
    * Toggles through the avaliable render engines
    */
    void ToggleRenderEngine();

private: 

    /**
    * Updates and renders the application
    */
    void TickApplication();

    /**
    * Initialises the tweak bar
    * @param opengl Whether to use opengl or directx
    */
    void InitialiseTweakBar(bool opengl);

    /**
    * Removes the tweak bar
    */
    void RemoveTweakBar();

    /**
    * Toggles whether to hide/show the tweak bar
    */
    void ToggleTweakBar();

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
    void UpdateMouseCoordinates(const MSG& msg);

    RenderEngine* m_engine;  ///< currently selected rendering engine
    bool m_showTweakBar;     ///< Whether the tweak bar is currently visible
    CTwBar* m_tweakbar;      ///< Used for runtime diagnostics

    bool m_mousePressed;     ///< Whether the mouse is held down or not
    Float2 m_mouseDirection; ///< Direction of movement for the mouse
    Float2 m_mousePosition;  ///< 2D coordinates of the mouse

    std::unique_ptr<Camera> m_camera;         ///< Scene camera for generating view matrix
    std::unique_ptr<Scene> m_scene;           ///< Holds meshes, lighting and shader data
    std::unique_ptr<Timer> m_timer;           ///< For measure change in frame time
    std::unique_ptr<OpenglEngine> m_opengl;   ///< OpenGL rendering engine
    std::unique_ptr<DirectxEngine> m_directx; ///< DirectX rendering engine
};