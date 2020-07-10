////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - app_gui.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "cache.h"

#include <functional>

struct SceneData;
class Scene;
class RenderEngine;
class Timer;
class Camera;

/**
* Allows manipulation oft the scene elements through the tweak bar
*/
class AppGui
{
public:

    /**
    * Constructor
    * @param scene The scene to modify
    * @param timer The timer for the scene
    * @param camera The camera for the scene
    * @param cache Shared data between the gui and application
    * @param selectedMap The initial post map selected
    * @param reloadEngine Callback to reload the render engine
    */
    AppGui(Scene& scene,
           Timer& timer,
           Camera& camera,
           std::shared_ptr<Cache> cache,
           int selectedMap,
           std::function<void(void)> reloadEngine);

    /**
    * Destructor
    */
    ~AppGui();

    /**
    * Ticks the modifier to recieve information from the gui cache
    * @param engine The selected render engine
    */
    void Tick(RenderEngine& engine);

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
    * @param engine The selected Render Engine
    */
    void UpdateScene(RenderEngine& engine);

    /**
    * Updates the scene camera from the shared cache
    */
    void UpdateCamera();

    /**
    * Updates the light shared cache between the gui and application
    */
    void UpdateLight();

    /**
    * Updates the mesh shared cache between the gui and application
    */
    void UpdateMesh();

    /**
    * Updates the terrain shared cache between the gui and application
    * @param engine The selected Render Engine
    */
    void UpdateTerrain(RenderEngine& engine);

    /**
    * Updates the texture shared cache between the gui and application
    * @param engine The selected Render Engine
    */
    void UpdateTexture(RenderEngine& engine);

    /**
    * Updates the water shared cache between the gui and application
    */
    void UpdateWater();

    /**
    * Updates the emitter shared cache between the gui and application
    */
    void UpdateEmitter();

    /**
    * Updates the post shared cache between the gui and application
    * @param engine The selected render engine
    */
    void UpdatePost(RenderEngine& engine);

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

    /**
    * @return the names of the terrain in the scene
    */
    std::vector<std::string> GetTerrainNames() const;

    /**
    * @return the names of the editable textures in the scene
    */
    std::vector<std::string> GetTextureNames() const;

    /**
    * @return the names of the lights in the scene
    */
    std::vector<std::string> GetLightNames() const;

    /**
    * @return the names of the meshes in the scene
    */
    std::vector<std::string> GetMeshNames() const;

    /**
    * @return the names of the waters in the scene
    */
    std::vector<std::string> GetWaterNames() const;

    /**
    * @return the names of the emitters in the scene
    */
    std::vector<std::string> GetEmitterNames() const;

    /**
    * @return the names of the shaders in the scene
    */
    std::vector<std::string> GetShaderNames() const;

    /**
    * @return the names of the post maps in the scene
    */
    std::vector<std::string> GetPostMapNames() const;

private:

    Scene& m_scene;                   ///< The scene object to manipulate
    SceneData& m_data;                ///< Data for the scene to manipulate
    Timer& m_timer;                   ///< The timer for the scene
    Camera& m_camera;                 ///< The camera for the scene
    int m_selectedLight = -1;         ///< Current light selected
    int m_selectedMesh = -1;          ///< Current mesh selected
    int m_selectedWater = -1;         ///< Current water selected
    int m_selectedWave = -1;          ///< Current wave selected
    int m_selectedMap = -1;           ///< Current post map selected
    int m_selectedShader = -1;        ///< Current shader selected
    int m_selectedEmitter = -1;       ///< Current emitter selected
    int m_selectedTexture = -1;       ///< Current texture selected
    int m_selectedTerrain = -1;       ///< Current terrain selected
    int m_engineAmount = 0;           ///< Number of engines that can be selected
    std::shared_ptr<Cache> m_cache;   ///< Shared data between the gui and application

    std::function<void(void)> m_reloadEngine = nullptr; ///< Callback to reload the engine
};
