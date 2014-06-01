////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directxtarget.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "directxcommon.h"

/**
* Holds a single render target 
*/
class DxRenderTarget
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
    * @param device The directx device
    * @param swapchain Collection of buffers for displaying frames
    * @return whether initialises succeeded or not
    */
    bool Initialise(ID3D11Device* device, IDXGISwapChain* swapchain = nullptr);

    /**
    * Sets the render target as activated and clears it
    * @param context The directx device context
    * @param zBuffer The depth buffer or null if one is not used
    */
    void SetActive(ID3D11DeviceContext* context, 
        ID3D11DepthStencilView* zbuffer = nullptr);

    /**
    * Sends the texture to the currently active shader
    * @param context The direct device context
    * @param slot The slot to put the texture into
    * @note requires clearing before render target can use again
    */
    void SendTexture(ID3D11DeviceContext* context, int slot);

    /**
    * Clears the texture sent to the currently active shader
    * @param context The direct device context
    * @param slot The slot the texture exists in
    * @note Required as texture needs to be unbound from the 
    *       shader before render target is activated
    */
    void ClearTexture(ID3D11DeviceContext* context, int slot);

private:

    bool m_isBackBuffer;                        ///< Whether this render target is the back buffer
    const std::string m_name;                   ///< Name of the render target
	ID3D11Texture2D* m_texture;                 ///< Texture ofthe render target
	ID3D11RenderTargetView* m_renderTarget;     ///< Render target buffer
	ID3D11ShaderResourceView* m_textureView;    ///< Shader view of the texture
};