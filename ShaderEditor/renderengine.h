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
    * Sets up the graphics engine for rendering
    * @return whether initialization succeeded
    */
    virtual bool Initialize() = 0;

    /**
    * Begins rendering the scene
    */
    virtual void BeginRender() = 0;

    /**
    * Renders the 3D scene
    * @param lights All lighting in the scene
    */
    virtual void Render(const std::vector<Light>& lights) = 0;

    /**
    * Ends rendering the scene
    */
    virtual void EndRender() = 0;

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
    * @return whether initialisation was successful
    */
    virtual bool InitialiseScene(
        const std::vector<Mesh>& meshes, 
        const std::vector<Mesh>& alpha, 
        const std::vector<Shader>& shaders) = 0;

    /**
    * ReInitialises the scene for openGL
    * @return whether initialisation was successful
    */
    virtual bool ReInitialiseScene() = 0;

    /**
    * @return the name of the render engine
    */
    virtual std::string GetName() const = 0;
};