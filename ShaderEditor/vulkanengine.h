////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vulkanengine.h
// Vulkan code Reference: https://github.com/SaschaWillems/Vulkan
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "renderengine.h"
#include <Windows.h>

class MeshAttributes;
class MeshData;
class Particle;
class Mesh;
class Water;
class Light;
class Terrain;
class Quad;
class PostProcessing;
class Emitter;
struct VulkanData;

/**
* Vulkan Graphics engine
*/
class VulkanEngine : public RenderEngine
{
public:

    /**
    * Constructor
    * @param hwnd Handle to the window
    * @param hinstance Handle to the current instance of the application
    */
    VulkanEngine(HWND hwnd, HINSTANCE hinstance);

    /**
    * Destructor
    */
    ~VulkanEngine();

    /**
    * Explicity releases resources for the engine
    */
    virtual void Release() override;

    /**
    * Sets up the graphics engine for rendering
    * @return whether initialization succeeded
    */
    virtual bool Initialize() override;

    /**
    * Renders the 3D scene
    * @param scene The elements making up the scene
    * @param timer The time passed since scene start
    */
    virtual void Render(const IScene& scene,
                        float timer) override;

    /**
    * Initialises the scene for the engine
    * @param scene The elements making up the scene
    * @return whether initialisation was successful
    */
    virtual bool InitialiseScene(const IScene& scene) override;

    /**
    * ReInitialises the scene
    * @return whether initialisation was successful
    */
    virtual bool ReInitialiseScene() override;

    /**
    * Reloads the texture at the given index
    */
    virtual void ReloadTexture(int index) override;

    /**
    * Reloads the terrain at the given index
    */
    virtual void ReloadTerrain(int index) override;

    /**
    * Generates the shader for the engine
    * @param index An unique index for the shader
    * @return an error message if compilation failed
    */
    virtual std::string CompileShader(int index) override;

    /**
    * @return the name of the render engine
    */
    virtual std::string GetName() const override;

    /**
    * Updates the engine's cached view matrix
    * @param world The world matrix of the camera
    */
    virtual void UpdateView(const Matrix& world) override;

    /**
    * Gets the text for a specific shader
    * @param index The shader index
    * @return the text for the shader
    */
    virtual std::string GetShaderText(int index) const override;

    /**
    * Gets the assembly for a specific shader
    * @param index The shader index
    * @return the assembly for the shader
    */
    virtual std::string GetShaderAssembly(int index) override;

    /**
    * Fades the screen in or out to black by the given amount
    * @param in Whether to fade in or out
    * @param amount The amount to fade by
    * @return whether the fade has reached the capped target of [0,1]
    */
    virtual bool FadeView(bool in, float amount) override;

    /**
    * Explicitly sets the current amount of fade
    * @param value The amount of fade between [0,1]
    */
    virtual void SetFade(float value) override;

    /**
    * Toggles whether meshes are rendered in wireframe
    */
    virtual void ToggleWireframe() override;

    /**
    * Writes the shader text file
    * @param shader The shader to write to
    * @param text The new text for the shader
    */
    virtual void WriteToShader(const Shader& shader,
                               const std::string& text) override;

private:

    std::unique_ptr<VulkanData> m_data;  ///< member data
};                     