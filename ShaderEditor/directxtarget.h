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
    * @param zBuffer The depth buffer
    */
    void SetActive(ID3D11DeviceContext* context, ID3D11DepthStencilView* zbuffer);

    /**
    * @return the shader view of the texture
    */
    ID3D11ShaderResourceView* GetTexture();

private:

    bool m_isBackBuffer;                        ///< Whether this render target is the back buffer
    const std::string m_name;                   ///< Name of the render target
	ID3D11Texture2D* m_texture;                 ///< Texture ofthe render target
	ID3D11RenderTargetView* m_renderTarget;     ///< Render target buffer
	ID3D11ShaderResourceView* m_textureView;    ///< Shader view of the texture

};