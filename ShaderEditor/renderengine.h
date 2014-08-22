////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - renderengine.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Windows.h>
#include "elements.h"

/**
* Base graphics API interface
*/
class RenderEngine
{
public:

    /**
    * Destructor
    */
    virtual ~RenderEngine() {}

    /**
    * Sets up the graphics engine for rendering
    * @return whether initialization succeeded
    */
    virtual bool Initialize() = 0;

    /**
    * Explicity releases resources for the engine
    */
    virtual void Release() = 0;

    /**
    * Renders the 3D scene
    * @param lights All lighting in the scene
    * @param postProcessing values for the final image
    */
    virtual void Render(const std::vector<Light>& lights,
                        const PostProcessing& post) = 0;

    /**
    * Generates the shader for the engine
    * @param index An unique index for the shader
    * @return an error message if compilation failed
    */
    virtual std::string CompileShader(int index) = 0;

    /**
    * Initialises the scene for openGL
    * @param meshes All mesh data for the scene
    * @param alpha All translucent mesh data for the scene
    * @param shaders All shader data for the scene
    * @param textuers All texture data for the scene
    * @return whether initialisation was successful
    */
    virtual bool InitialiseScene(
        const std::vector<Mesh>& meshes, 
        const std::vector<Mesh>& alpha, 
        const std::vector<Shader>& shaders,
        const std::vector<Texture>& textures) = 0;

    /**
    * ReInitialises the scene for openGL
    * @return whether initialisation was successful
    */
    virtual bool ReInitialiseScene() = 0;

    /**
    * @return the name of the render engine
    */
    virtual std::string GetName() const = 0;

    /**
    * Gets the text for a specific shader
    * @param index The shader index
    * @return the text for the shader
    */
    virtual std::string GetShaderText(int index) const = 0;

    /**
    * Gets the assembly for a specific shader
    * @param index The shader index
    * @return the assembly for the shader
    */
    virtual std::string GetShaderAssembly(int index) = 0;

    /**
    * Updates the engine's cached view matrix
    * @param world The world matrix of the camera
    */
    virtual void UpdateView(const Matrix& world) = 0;

    /**
    * Fades the screen in or out to black by the given amount
    * @param in Whether to fade in or out
    * @param amount The amount to fade by
    * @return whether the fade has reached the capped target of [0,1]
    */
    virtual bool FadeView(bool in, float amount) = 0;

    /**
    * Explicitly sets the current amount of fade
    * @param value The amount of fade between [0,1]
    */
    virtual void SetFade(float value) = 0;
};