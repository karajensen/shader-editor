////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directxengine.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "renderengine.h"

struct DirectxData;
struct ID3D11Device;
struct D3DXMATRIX;

/**
* DirectX Graphics engine
*/
class DirectxEngine : public RenderEngine, boost::noncopyable
{
public:

    /**
    * Constructor
    * @param hwnd Handle to the window
    */
    explicit DirectxEngine(HWND hwnd);

    /**
    * Destructor
    */
    ~DirectxEngine();

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
    virtual void Render(const SceneElements& scene,
                        float timer) override;

    /**
    * Initialises the scene for the engine
    * @param scene The elements making up the scene
    * @return whether initialisation was successful
    */
    virtual bool InitialiseScene(const SceneElements& scene) override;

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
    * @return the directX device
    */
    ID3D11Device* GetDevice() const;

    /**
    * Updates the engine's cached view matrix
    * @param world The world matrix of the camera
    * @param depthNear The near cutoff value for the depth
    * @param depthFar The far cutoff value for the depth
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
    * Sets whether directx should cull backfaces or not
    * @param shouldCull whether to cull or not
    */
    void SetBackfaceCull(bool shouldCull);

    /**
    * Updates and switches to the main shader the mesh requires
    * @param mesh The mesh currently rendering
    * @param lights All lighting in the scene
    */
    void UpdateShader(const Mesh& mesh, const std::vector<Light>& lights);

    /**
    * Updates and switches to the normal shader the mesh requires
    * @param mesh The mesh currently rendering
    * @param post Data for post processing
    */
    void UpdateShader(const Mesh& mesh, const PostProcessing& post);

    /**
    * Updates and switches to main shader the water requires
    * @param water The water currently rendering
    * @param lights All lighting in the scene
    * @param timer The time passed since scene start
    */
    void UpdateShader(const Water& water, 
                      const std::vector<Light>& lights,
                      float timer);

    /**
    * Updates and switches to the normal shader the water requires
    * @param water The mesh currently rendering
    * @param post Data for post processing
    */
    void UpdateShader(const Water& water, 
                      const PostProcessing& post);

    /**
    * Updates the shader for an emitter
    */
    void UpdateShader(const Emitter& emitter);

    /**
    * Updates the shader for a particle
    * @param world The world matrix for the particle
    * @param particle The data for the particle
    */
    void UpdateShader(D3DXMATRIX world, const Particle& particle);

    /**
    * Sets the shader at the given index as selected
    */
    void SetSelectedShader(int index);

    /**
    * Sends all textures to the selected shader
    */
    void SendTextures(const std::vector<int>& textures);

    /**
    * Initialises the DirectX debugging layer
    * @note only for _DEBUG
    */
    void InitialiseDebugging();

    /**
    * Renders the scene as a normal/depth map
    * @param postProcessing values for the final image
    */
    void RenderNormalMap(const PostProcessing& post);

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
    * Sets whether alpha blending is enabled or not
    */
    void EnableAlphaBlending(bool enable);

    HWND m_hwnd = nullptr;               ///< handle to the window
    std::unique_ptr<DirectxData> m_data; ///< member data of directX
};                     