////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directxengine.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "renderengine.h"
#include <memory>

struct DirectxData;
struct ID3D11Device;

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
    DirectxEngine(HWND hwnd);

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
    * @param lights All lighting in the scene
    */
    virtual void Render(const std::vector<Light>& lights) override;

    /**
    * Initialises the scene for directX
    * @param meshes All mesh data for the scene
    * @param alpha All translucent mesh data for the scene
    * @param shaders All shader data for the scene
    * @param textures All texture data for the scene
    * @return whether initialisation was successful
    */
    virtual bool InitialiseScene(
        const std::vector<Mesh>& meshes, 
        const std::vector<Mesh>& alpha, 
        const std::vector<Shader>& shaders,
        const std::vector<Texture>& textures) override;

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

private:

    /**
    * Sets whether directx should cull backfaces or not
    * @param shouldCull whether to cull or not
    */
    void SetBackfaceCull(bool shouldCull);

    /**
    * Updates and switches to shader the mesh requires
    * @param mesh The mesh currently rendering
    * @param lights All lighting in the scene
    */
    void UpdateShader(const Mesh& mesh, const std::vector<Light>& lights);

    /**
    * Sends the textures to the selected shader
    * @param textureIDs the unique ids of the textures to send
    */
    void SetTextures(const std::vector<int>& textureIDs);

    /**
    * Initialises the DirectX debugging layer
    * @note only for _DEBUG
    */
    void InitialiseDebugging();

    HWND m_hwnd;                         ///< handle to the window
    std::unique_ptr<DirectxData> m_data; ///< member data of directX
};                     