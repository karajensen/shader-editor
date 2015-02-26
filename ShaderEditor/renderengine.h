////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - renderengine.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "common.h"
#include "renderdata.h"

class IScene;

/**
* Base graphics API interface
*/
class RenderEngine : boost::noncopyable
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
    * @param scene The elements making up the scene
    * @param timer The time passed since scene start
    */
    virtual void Render(const IScene& scene,
                        float timer) = 0;

    /**
    * Generates the shader for the engine
    * @param index An unique index for the shader
    * @return an error message if compilation failed
    */
    virtual std::string CompileShader(int index) = 0;

    /**
    * Initialises the scene for the engine
    * @param scene The elements making up the scene
    * @return whether initialisation was successful
    */
    virtual bool InitialiseScene(const IScene& scene) = 0;

    /**
    * ReInitialises the scene for the ending
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

    /**
    * Writes the shader text file
    * @param name The name of the shader to write over
    * @param text The new text for the shader
    */
    virtual void WriteToShader(const std::string& name, 
                               const std::string& text) = 0;
};