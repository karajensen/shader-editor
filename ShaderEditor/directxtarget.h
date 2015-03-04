////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directxtarget.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "directxcommon.h"

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
    */
    DxRenderTarget(const std::string& name, int textures, bool multisampled);

    /**
    * Destructor
    */
    ~DxRenderTarget();

    /**
    * Releases the render target
    */
    void Release();

    /**
    * Initialises the render target
    * @param device The DirectX device interface
    * @param swapchain Collection of buffers for displaying frames
    * @return whether initialises succeeded or not
    */
    bool Initialise(ID3D11Device* device, IDXGISwapChain* swapchain = nullptr);

    /**
    * Sets the render target as activated and clears it
    * @param context Direct3D device context
    */
    void SetActive(ID3D11DeviceContext* context);

    /**
    * Sends the texture to the currently active shader
    * @param context Direct3D device context
    * @param slot The shader slot the texture should send to
    * @param ID The texture index attached to the target to send
    * @note requires clearing before render target can use again
    */
    void SendTexture(ID3D11DeviceContext* context, int slot, int ID = 0);

    /**
    * Clears the texture sent to the currently active shader
    * @param context Direct3D device context
    * @param slot The shader slot the texture should clear from
    * @note Required as texture needs to be unbound from the 
    *       shader before render target is activated
    */
    void ClearTexture(ID3D11DeviceContext* context, int slot);

private:

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

    const int m_count = 0;                              ///< The number of textures bound to this target
    const bool m_isBackBuffer = false;                  ///< Whether this render target is the back buffer
    const bool m_multisampled = false;                  ///< Whether this render target uses multisampling
    const std::string m_name;                           ///< Name of the render target
    std::vector<ID3D11Texture2D*> m_textures;           ///< Textures to render to
    std::vector<ID3D11RenderTargetView*> m_targets;     ////< Render target buffers
    std::vector<ID3D11ShaderResourceView*> m_views;     ///< Shader views for the textures
    ID3D11DepthStencilView* m_depthBuffer = nullptr;    ///< Depth buffer for the render target
    static D3D11_TEXTURE2D_DESC sm_textureDesc;         ///< Base description of the render target textures
};