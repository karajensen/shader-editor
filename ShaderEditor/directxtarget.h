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
    * Constructor
    * @param name Name of the render target
    * @param isBackBuffer Whether this render target is the back buffer
    */
    DxRenderTarget(const std::string& name, bool isBackBuffer = false);

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
    * @param slot The slot to put the texture into
    * @note requires clearing before render target can use again
    */
    void SendTexture(ID3D11DeviceContext* context, int slot);

    /**
    * Clears the texture sent to the currently active shader
    * @param context Direct3D device context
    * @param slot The slot the texture exists in
    * @note Required as texture needs to be unbound from the 
    *       shader before render target is activated
    */
    void ClearTexture(ID3D11DeviceContext* context, int slot);

private:

    /**
    * Initialises the shared depth buffer
    * @param device The DirectX device interface
    * @return whether initialises succeeded or not
    */
    bool InitialiseDepthBuffer(ID3D11Device* device);

    /**
    * Initiliases a render target pass
    * @param device The DirectX device interface
    * @param firstPass Whether this is the first pass initialised
    * @return whether initialises succeeded or not
    */
    bool InitialisePass(ID3D11Device* device, bool firstPass);

    bool m_isBackBuffer = false;                       ///< Whether this render target is the back buffer
    const std::string m_name;                          ///< Name of the render target
    ID3D11Texture2D* m_texture = nullptr;              ///< Texture to render to
    ID3D11RenderTargetView* m_renderTarget = nullptr;  ///< Render target buffer for the first pass
    ID3D11ShaderResourceView* m_view = nullptr;        ///< Shader view of the first pass texture
    
    static D3D11_TEXTURE2D_DESC sm_textureDesc;        ///< Base description of the render target textures
    static ID3D11DepthStencilView* sm_depthBuffer;     ///< Depth buffer shared between render targets
};