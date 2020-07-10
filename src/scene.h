////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - scene.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "scene_interface.h"
#include "float3.h"

#include <boost/property_tree/ptree.hpp>

class Camera;
class SceneBuilder;
class SceneModifier;
class ScenePlacer;
struct SceneData;

/**
* Manager and owner of all objects and diagnostics
*/
class Scene : public IScene
{
public:

    Scene();
    ~Scene();

    /**
    * Ticks the scene
    * @param deltatime The time passed between ticks
    * @param camera For updating the viewable area
    */
    void Tick(float deltatime, const Camera& camera);

    /**
    * Post ticks the scene
    */
    void PostTick();

    /**
    * @return the meshes in the scene
    */
    virtual const std::vector<std::unique_ptr<Mesh>>& Meshes() const override;

    /**
    * @return the water in the scene
    */
    virtual const std::vector<std::unique_ptr<Water>>& Waters() const override;

    /**
    * @return the shaders in the scene
    */
    virtual const std::vector<std::unique_ptr<Shader>>& Shaders() const override;

    /**
    * @return the lights in the scene
    */
    virtual const std::vector<std::unique_ptr<Light>>& Lights() const override;

    /**
    * @return the terrain in the scene
    */
    virtual const std::vector<std::unique_ptr<Terrain>>& Terrains() const override;

    /**
    * @return the textures in the scene
    */
    virtual const std::vector<std::unique_ptr<Texture>>& Textures() const override;

    /**
    * @return the emitters in the scene
    */
    virtual const std::vector<std::unique_ptr<Emitter>>& Emitters() const override;

    /**
    * @return the post processing for the final image
    */
    virtual const PostProcessing& Post() const override;

    /**
    * @return the shadow instances for meshes
    */
    virtual const MeshData& Shadows() const override;

    /**
    * Sets which post map will currently be rendered
    * @param index The index for the map to render
    */
    void SetPostMap(int index);

    /**
    * @return the data for the scene
    */
    SceneData& GetData();

    /**
    * Initialises the scene
    * @param camera The position of the camera
    * @return whether initialisation was successful
    */
    bool Initialise(const Float3& camera);

    /**
    * Reloads the scene
    */
    void Reload();

    /**
    * Reloads just the placement of the scene
    */
    void ReloadPlacement();

    /**
    * Reloads the texture
    * @param ID The ID of the texture to reload
    */
    void ReloadTexture(int ID);

    /**
    * Reloads the terrain
    * @param ID the ID of the terrain to reload
    */
    void ReloadTerrain(int ID);

private:

    std::unique_ptr<SceneData> m_data;         ///< Elements of the scene
    std::unique_ptr<SceneBuilder> m_builder;   ///< Creates meshes, lighting and shader data
    std::unique_ptr<ScenePlacer> m_placer;    ///< Updates the scene depending on the camera
};          
