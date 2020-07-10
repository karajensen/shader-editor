////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directx_target.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "directx_common.h"

/**
* Render target that uses a shared depth buffer
*/
class DxRenderTarget : boost::noncopyable
{
public:

    /**
    * Constructor for the back buffer target
    * @param name Name of the render target
    */
    DxRenderTarget(const std::string& name);

    /**
    * Constructor for a render target with multiple textures
    * @param name Name of the render target
    * @param textures The number of textures bound to this target
    * @param multisampled Whether this target uses multisampling
    * @param readWrite Whether this target both read/writes to itself
    */
    DxRenderTarget(const std::string& name, 
                   int textures, 
                   bool multisampled,
                   bool readWrite = false);

    /**
    * Destructor
    */
    ~DxRenderTarget();

    /**
    * Releases the render target
    */
    void Release();

    /**
    * Initialises the back buffer
    * @param device The DirectX device interface
    * @param swapchain Collection of buffers for displaying frames
    * @return whether initialises succeeded or not
    */
    bool Initialise(ID3D11Device* device, IDXGISwapChain* swapchain);

    /**
    * Initialises a render target
    * @param device The DirectX device interface
    * @param state The sampler state to render this target
    * @return whether initialises succeeded or not
    */
    bool Initialise(ID3D11Device* device, ID3D11SamplerState* state);

    /**
    * Sets the render target as activated and clears it
    * @param context Direct3D device context
    */
    void SetActive(ID3D11DeviceContext* context);

    /**
    * Gets the render target texture
    * @param ID The texture index attached to the target
    */
    ID3D11ShaderResourceView* const* Get(int ID = 0) const;

    /**
    * Gets the copied render target texture
    * @param ID The texture index attached to the target
    */
    ID3D11ShaderResourceView* const* GetCopied(int ID = 0) const;

    /**
    * @return the sampler state of this render target
    */
    ID3D11SamplerState* const* State() const;

    /**
    * Copies the textures to the copied cache
    * @param context Direct3D device context
    */
    void CopyTextures(ID3D11DeviceContext* context);

    /**
    * Sets the render target texture as high quality
    * @note needs to be called before initialise
    */
    void SetHighQuality(int index);

private:

    /**
    * Clears the render target
    * @param context Direct3D device context
    */
    void ClearTarget(ID3D11DeviceContext* context);

    /**
    * Initialises the containers that hold the buffers
    */
    void InitialiseContainers();

    /**
    * Initialises a render target
    * @param device The DirectX device interface
    * @param ID the index of the target to initialise
    * @return whether initialise succeeded or not
    */
    bool InitialiseRenderTarget(ID3D11Device* device, int ID);

    /**
    * Initialises the depth buffer
    * @param device The DirectX device interface
    * @return whether initialise succeeded or not
    */
    bool InitialiseDepthBuffer(ID3D11Device* device);

    /**
    * Initialises the back buffer
    * @param device The DirectX device interface
    * @param swapchain Collection of buffers for displaying frames
    * @return whether initialise succeeded or not
    */
    bool InitialiseBackBuffer(ID3D11Device* device, IDXGISwapChain* swapchain);

private:

    const bool m_readWrite = false;                       ///< Whether this target both read/writes to itself
    const int m_count = 0;                                ///< The number of textures bound to this target
    const bool m_isBackBuffer = false;                    ///< Whether this render target is the back buffer
    const bool m_multisampled = false;                    ///< Whether this render target uses multisampling
    const std::string m_name;                             ///< Name of the render target
    std::vector<ID3D11Texture2D*> m_textures;             ///< Textures to render to
    std::vector<ID3D11Texture2D*> m_copiedTextures;       ///< Copied textures to render to
    std::vector<ID3D11RenderTargetView*> m_targets;       ////< Render target buffers
    std::vector<ID3D11ShaderResourceView*> m_views;       ///< Shader views for the textures
    std::vector<ID3D11ShaderResourceView*> m_copiedViews; ///< Shader views for the textures
    std::vector<bool> m_highQuality;                      ///< Whether to use high quality textures
    ID3D11DepthStencilView* m_depthBuffer = nullptr;      ///< Depth buffer for the render target
    static D3D11_TEXTURE2D_DESC sm_textureDesc;           ///< Base description of the render target textures
    ID3D11SamplerState* m_state = nullptr;                ///< The sampler state for rendering the target
};
