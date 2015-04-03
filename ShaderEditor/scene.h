////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - scene.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "boost/property_tree/ptree.hpp"
#include "sceneInterface.h"

class AnimatedTexture;
class FragmentLinker;
class Diagnostic;
class SceneBuilder;
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
    */
    void Tick(float deltatime);

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
    * @param index The index of the light to get
    * @return the light 
    */
    Light& GetLight(int index);

    /**
    * @param index The index of the mesh to get
    * @return the mesh 
    */
    Mesh& GetMesh(int index);

    /**
    * @param index The index of the water to get
    * @return the water 
    */
    Water& GetWater(int index);

    /**
    * @param index The index of the terrain to get
    * @return the terrain 
    */
    Terrain& GetTerrain(int index);

    /**
    * @param index The index of the texture to get
    * @return the texture 
    */
    ProceduralTexture& GetProceduralTexture(int index);

    /**
    * @param index The index of the emitter to get
    * @return the emitter 
    */
    Emitter& GetEmitter(int index);

    /**
    * @param index The index of the shader to get
    * @return the shader 
    */
    Shader& GetShader(int index);

    /**
    * @return the post processing for the final image
    */
    PostProcessing& GetPost();

    /**
    * @return the caustics
    */
    AnimatedTexture& GetCaustics();

    /**
    * @return the diagnostics for the scene
    */
    Diagnostic& GetDiagnostics();

    /**
    * Sets which post map will currently be rendered
    * @param index The index for the map to render
    */
    void SetPostMap(int index);

    /**
    * Gets the name of the texture
    * @param index The index of the texture
    * @return the name of the texture
    */
    std::string GetTexture(int index);

    /**
    * Adds a shader to the scene
    * @param element The element to add to the scene
    * @return the element added
    */
    Shader& Add(std::unique_ptr<Shader> element);

    /**
    * Adds a mesh to the scene
    * @param element The element to add to the scene
    * @return the element added
    */
    Mesh& Add(std::unique_ptr<Mesh> element);

    /**
    * Adds terrain to the scene
    * @param element The element to add to the scene
    * @return the element added
    */
    Terrain& Add(std::unique_ptr<Terrain> element);

    /**
    * Adds a water to the scene
    * @param element The element to add to the scene
    * @return the element added
    */
    Water& Add(std::unique_ptr<Water> element);

    /**
    * Adds an emitter to the scene
    * @param element The element to add to the scene
    * @return the element added
    */
    Emitter& Add(std::unique_ptr<Emitter> element);

    /**
    * Adds a light to the scene
    * @param element The element to add to the scene
    */
    void Add(std::unique_ptr<Light> element);

    /**
    * Adds a texture to the scene
    * @param element The element to add to the scene
    * @return the index where the element was added
    */
    unsigned int Add(std::unique_ptr<Texture> element);

    /**
    * Adds a texture to the scene
    * @param element The element to add to the scene
    */
    void Add(std::unique_ptr<ProceduralTexture> element);

    /**
    * Adds post processing to the scene
    * @param post The post processing to add to the scene
    */
    void Add(std::unique_ptr<PostProcessing> post);

    /**
    * Adds diagnostics to the scene
    * @param diagnostics The diagnostics to add to the scene
    */
    void Add(std::unique_ptr<Diagnostic> diagnostics);

    /**
    * Adds caustics to the scene
    * @param caustics The animated texture to add to the scene
    */
    void Add(std::unique_ptr<AnimatedTexture> caustics);

    /**
    * Initialises the scene
    * @return whether initialisation was successful
    */
    bool Initialise();

    /**
    * Outputs the scene to an xml file
    */
    void SaveSceneToFile();

    /**
    * Outputs the procedural texture to file
    * @param ID The ID of the texture to save
    */
    void SaveTextureToFile(int ID);

    /**
    * Reloads the scene
    */
    void Reload();

    /**
    * Reloads the procedural texture
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
};                     