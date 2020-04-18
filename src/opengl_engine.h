////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - opengl_engine.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "render_engine.h"
#include "glm/glm.hpp"
#include <Windows.h>

class MeshData;
class MeshAttributes;
class Particle;
class Mesh;
class Quad;
class Water;
class Light;
class Terrain;
class PostProcessing;
class Emitter;
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
    OpenglEngine(HWND hwnd);

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

    /**
    * Updates and switches to main shader the mesh requires
    * @param mesh The mesh currently rendering
    * @param scene The scene to render
    * @param alphaBlend Whether to use alpha blending
    * @param timer The timer to send or -1 if not to send
    * @return whether the mesh can now be rendered
    */
    bool UpdateShader(const MeshData& mesh, 
                      const IScene& scene,
                      bool alphaBlend, 
                      float timer = 0.0f);

    /**
    * Updates and switches to main shader the mesh requires
    * @param mesh The mesh currently rendering
    * @param scene The data for the scene
    * @return whether the mesh can now be rendered
    */
    bool UpdateShader(const Mesh& mesh, const IScene& scene);

    /**
    * Updates and switches to main shader the terrain requires
    * @param terrain The terrain currently rendering
    * @param scene The data for the scene
    * @return whether the terrain can now be rendered
    */
    bool UpdateShader(const Terrain& terrain, 
                      const IScene& scene);

    /**
    * Updates and switches to main shader the water requires
    * @param water The water currently rendering
    * @param scene Data for the scene to render
    * @param timer The time passed since scene start
    * @return whether the mesh can now be rendered
    */
    bool UpdateShader(const Water& water, 
                      const IScene& scene, 
                      float timer);

    /**
    * Updates and switches to the shader for an emitter
    * @param emitter The emitter to render
    * @param scene Data for the scene to render
    * @return whether the emitter can now be rendered
    */
    bool UpdateShader(const Emitter& emitter,
                      const IScene& scene);

    /**
    * Updates and switches to the shader for a quad
    * @param quad The quad to render
    * @return whether the quad can now be rendered
    */
    bool UpdateShader(const MeshData& quad);

    /**
    * Updates the shader for a particle per instance
    * @param world The world matrix for the particle
    * @param particle The data for the particle
    */
    void UpdateShader(const glm::mat4& world, const Particle& particle);

    /**
    * Updates the shader for a mesh per instance
    * @param world The world matrix for the mesh
    * @param texture The colour texture to render
    */
    void UpdateShader(const glm::mat4& world, int texture);

    /**
    * Sets the shader at the given index as selected
    */
    void SetSelectedShader(int index);

    /**
    * Sends any attributes for a mesh
    * @param attributes The attributes of the mesh currently rendering
    */
    void SendAttributes(const MeshAttributes& attributes);

    /**
    * Sends light information to the selected shader
    */
    void SendLights(const std::vector<std::unique_ptr<Light>>& lights);

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
    bool SendTexture(const std::string& sampler, int ID);

    /**
    * Renders the scene
    * @param scene All the elements in the scene
    * @param timer The time passed since scene start
    */
    void RenderSceneMap(const IScene& scene, float timer);

    /**
    * Renders the scene with post processing
    * @param postProcessing values for the final image
    */
    void RenderPostProcessing(const PostProcessing& post);

    /**
    * Renders the scene as blurred
    * @param postProcessing values for the final image
    */
    void RenderBlur(const PostProcessing& post);

    /**
    * Renders the scene for pre-paring for post processing
    * @param postProcessing values for the final image
    */
    void RenderPreEffects(const PostProcessing& post);

    /**
    * Renders all emitters
    * @param scene The scene to render
    */
    void RenderEmitters(const IScene& scene);

    /**
    * Renders all shadows
    */
    void RenderShadows();

    /**
    * Renders the scene meshes
    * @param scene The scene to render
    */
    void RenderMeshes(const IScene& scene);

    /**
    * Renders the scene terrain
    * @param scene The scene to render
    */
    void RenderTerrain(const IScene& scene);

    /**
    * Renders the scene water
    * @param timer The time passed since scene start
    */
    void RenderWater(const IScene& scene, float timer);

    /**
    * Sets whether alpha blending is enabled or not
    * @param enable Whether to enable alpha blending
    * @param multiply Whether to multiply the blend colours
    */
    void EnableAlphaBlending(bool enable, bool multiply);

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
    std::unique_ptr<OpenglData> m_data;  ///< member data of opengl
};