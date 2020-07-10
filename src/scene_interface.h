////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - scene_interface.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <memory>

class MeshData;
class Mesh;
class Water;
class Light;
class Shader;
class Terrain;
class Texture;
class PostProcessing;
class Emitter;

/**
* Allows access to the elements of the scene
*/
class IScene
{
public:

    virtual ~IScene() = default;

    /**
    * @return the meshes in the scene
    */
    virtual const std::vector<std::unique_ptr<Mesh>>& Meshes() const = 0;

    /**
    * @return the water in the scene
    */
    virtual const std::vector<std::unique_ptr<Water>>& Waters() const = 0;

    /**
    * @return the shaders in the scene
    */
    virtual const std::vector<std::unique_ptr<Shader>>& Shaders() const = 0;

    /**
    * @return the lights in the scene
    */
    virtual const std::vector<std::unique_ptr<Light>>& Lights() const = 0;

    /**
    * @return the terrain in the scene
    */
    virtual const std::vector<std::unique_ptr<Terrain>>& Terrains() const = 0;

    /**
    * @return the textures in the scene
    */
    virtual const std::vector<std::unique_ptr<Texture>>& Textures() const = 0;

    /**
    * @return the post processing for the final image
    */
    virtual const PostProcessing& Post() const = 0;

    /**
    * @return the emitters in the scene
    */
    virtual const std::vector<std::unique_ptr<Emitter>>& Emitters() const = 0;

    /**
    * @return the shadow instances for meshes
    */
    virtual const MeshData& Shadows() const = 0;
};           
