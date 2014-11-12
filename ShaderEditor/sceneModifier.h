////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - SceneModifier.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "common.h"

class Scene;
class RenderEngine;
class Timer;
struct Cache;

/**
* Allows manipulation oft the scene elements through the tweak bar
*/
class SceneModifier
{
public:

    /**
    * Constructor
    * @param scene The scene to modifer
    * @param timer The timer for the scene
    * @param cache Shared data between the gui and application
    * @param selectedMap The initial post map selected
    */
    SceneModifier(Scene& scene, 
                  Timer& timer,
                  std::shared_ptr<Cache> cache,
                  int selectedMap);

    /**
    * Destructor
    */
    ~SceneModifier();

    /**
    * Ticks the modifier to recieve information from the gui cache
    * @param engine The selected render engine
    * @param mousePosition The screen position of the mouse
    * @Param mouseDirection The direction the mouse is moving
    */
    void Tick(RenderEngine& engine,
              const Float2& mousePosition, 
              const Float2& mouseDirection);

    /**
    * Sets whether the application should be running
    * @param run Whether the application is running
    */
    void SetApplicationRunning(bool run);

    /**
    * @return the engine selected by the gui cache
    */
    int GetSelectedEngine() const;

    /**
    * Sets the engine selected by the gui cache
    */
    void SetSelectedEngine(int engine);

    /**
    * Initialises the cache shared between the application and gui
    * @param engineNames The names of all engines supported
    * @param selectedEngine The engine currently selected
    */
    void Initialise(const std::vector<std::string>& engineNames,
                    int selectedEngine);

private:

    /**
    * Updates the scene shared cache between the gui and application
    * @param mousePosition The screen position of the mouse
    * @Param mouseDirection The direction the mouse is moving
    */
    void UpdateScene(const Float2& mousePosition, const Float2& mouseDirection);

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
    * Updates the shader shared cache between the gui and application
    * @param engine The selected render engine
    */
    void UpdateShader(RenderEngine& engine);

    /**
    * Recompiles the currently selected shader
    * @param text The new text to recompile with
    * @param engine The selected render engine
    * @return whether recompilation was successful
    */
    bool ReCompileShader(const std::string& text, RenderEngine& engine);

    Scene& m_scene;                   ///< The scene object to manipulate
    Timer& m_timer;                   ///< The timer for the scene
    int m_selectedLight = NO_INDEX;   ///< Current light selected
    int m_selectedMesh = NO_INDEX;    ///< Current mesh selected
    int m_selectedWater = NO_INDEX;   ///< Current water selected
    int m_selectedWave = NO_INDEX;    ///< Current wave selected
    int m_selectedMap = NO_INDEX;     ///< Current post map selected
    int m_selectedShader = NO_INDEX;  ///< Current shader selected
    int m_engineAmount = 0;           ///< Number of engines that can be selected
    std::shared_ptr<Cache> m_cache;   ///< Shared data between the gui and application
};