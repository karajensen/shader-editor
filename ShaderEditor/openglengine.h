////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - openglengine.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "renderengine.h"
#include "opengl/glm/glm.hpp"
#include <Windows.h>

struct OpenglData;

/**
* OpenGL Graphics engine
*/
class OpenglEngine : public RenderEngine
{
public:

    /**
    * Constructor
    * @param hwnd Handle to the window
    * @param hinstance Handle to the current instance of the application
    */
    OpenglEngine(HWND hwnd, HINSTANCE hinstance);

    /**
    * Destructor
    */
    ~OpenglEngine();

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
    * ReInitialises the scene for openGL
    * @return whether initialisation was successful
    */
    virtual bool ReInitialiseScene() override;

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
    * Writes the shader text file
    * @param name The name of the shader to write over
    * @param text The new text for the shader
    */
    virtual void WriteToShader(const std::string& name,
                               const std::string& text) override;

private:

    /**
    * Updates and switches to main shader the mesh requires
    * @param mesh The mesh currently rendering
    * @param lights All lighting in the scene
    * @return whether the mesh can now be rendered
    */
    bool UpdateShader(const Mesh& mesh, 
                      const std::vector<Light>& lights);

    /**
    * Updates and switches to the normal shader the mesh requires
    * @param mesh The mesh currently rendering
    * @param post Data for post processing
    * @return whether the mesh can now be rendered
    */
    bool UpdateShader(const Mesh& mesh, 
                      const PostProcessing& post);

    /**
    * Updates and switches to main shader the water requires
    * @param water The water currently rendering
    * @param lights All lighting in the scene
    * @param timer The time passed since scene start
    * @return whether the mesh can now be rendered
    */
    bool UpdateShader(const Water& water, 
                      const std::vector<Light>& lights, 
                      float timer);

    /**
    * Updates and switches to the normal shader the water requires
    * @param water The mesh currently rendering
    * @param post Data for post processing
    * @param timer The time passed since scene start
    * @return whether the mesh can now be rendered
    */
    bool UpdateShader(const Water& water, 
                      const PostProcessing& post, 
                      float timer);

    /**
    * Updates and switches to the shader for an emitter
    * @return whether the emitter can now be rendered
    */
    bool UpdateShader(const Emitter& emitter);

    /**
    * Updates the shader for a particle per instance
    * @param world The world matrix for the particle
    * @param particle The data for the particle
    */
    void UpdateShader(const glm::mat4& world, const Particle& particle);

    /**
    * Updates the shader for a mesh per instance
    * @param world The world matrix for the particle
    * @param particle The data for the particle
    */
    void UpdateShader(const glm::mat4& world, const Colour& colour);

    /**
    * Sets the shader at the given index as selected
    */
    void SetSelectedShader(int index);

    /**
    * Sends wave information to the selected shader
    */
    void SendWaves(const std::vector<Wave>& waves);

    /**
    * Sends light information to the selected shader
    */
    void SendLights(const std::vector<Light>& lights);

    /**
    * Sends all textures to the selected shader
    */
    void SendTextures(const std::vector<int>& textures);

    /**
    * Sends the given texture to the selected shader
    * @param slot Which slot in the shader should it go in
    * @param ID The texture ID
    * @return whether sending was successful
    */
    bool SendTexture(int slot, int ID);

    /**
    * Renders the scene
    * @param lights All the lights in the scene
    * @param timer The time passed since scene start
    */
    void RenderSceneMap(const std::vector<Light>& lights, float timer);

    /**
    * Renders the scene as a normal/depth map
    * @param postProcessing values for the final image
    * @param timer The time passed since scene start
    */
    void RenderNormalMap(const PostProcessing& post, float timer);

    /**
    * Renders the scene with post processing
    * @param postProcessing values for the final image
    */
    void RenderPostProcessing(const PostProcessing& post);

    /**
    * Renders the scene as blurred
    * @param postProcessing values for the final image
    */
    void RenderSceneBlur(const PostProcessing& post);

    /**
    * Renders all emitters
    */
    void RenderEmitters();

    /**
    * Sets whether alpha blending is enabled or not
    */
    void EnableAlphaBlending(bool enable);

    /**
    * Sets whether values are written to the depth buffer or not
    * @note if set to false the depth buffer of the currently 
    *       selected render target till not clear
    */
    void EnableDepthWrite(bool enable);

    /**
    * Sets whether opengl should cull backfaces or not
    * @param enable whether to cull or not
    */
    void EnableBackfaceCull(bool enable);

    /**
    * Enables the attributes of the currently selected shader
    */
    void EnableAttributes();

    HWND m_hwnd = nullptr;               ///< handle to the window
    HWND m_temporaryHwnd = nullptr;      ///< Handle to the temporary window used for glew
    std::unique_ptr<OpenglData> m_data;  ///< member data of opengl
};                     