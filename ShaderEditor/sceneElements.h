////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - sceneElements.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "common.h"

/**
* Allows access to the elements of the scene
*/
class SceneElements
{
public:

    virtual ~SceneElements() = default;

    /**
    * @return the meshes in the scene
    */
    virtual const std::vector<Mesh>& Meshes() const = 0;

    /**
    * @return the water in the scene
    */
    virtual const std::vector<Water>& Waters() const = 0;

    /**
    * @return the shaders in the scene
    */
    virtual const std::vector<Shader>& Shaders() const = 0;

    /**
    * @return the lights in the scene
    */
    virtual const std::vector<Light>& Lights() const = 0;

    /**
    * @return the textures in the scene
    */
    virtual const std::vector<Texture>& Textures() const = 0;

    /**
    * @return the post processing for the final image
    */
    virtual const PostProcessing& Post() const = 0;

    /**
    * @return the emitters in the scene
    */
    virtual const std::vector<Emitter>& Emitters() const = 0;
};                     