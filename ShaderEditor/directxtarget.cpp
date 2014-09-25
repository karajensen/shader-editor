////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directxtarget.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "directxtarget.h"

ID3D11DepthStencilView* DxRenderTarget::sm_depthBuffer = nullptr;
D3D11_TEXTURE2D_DESC DxRenderTarget::sm_textureDesc;

DxRenderTarget::DxRenderTarget(const std::string& name, bool isBackBuffer) :
    m_isBackBuffer(isBackBuffer),
    m_name(name)
{
}

DxRenderTarget::~DxRenderTarget()
{
    Release();
}

void DxRenderTarget::Release()
{
    SafeRelease(&m_texture);
    SafeRelease(&m_view);
    SafeRelease(&m_renderTarget);
    SafeRelease(&sm_depthBuffer);
}

bool DxRenderTarget::InitialiseDepthBuffer(ID3D11Device* device)
{
    D3D11_TEXTURE2D_DESC textureDesc = sm_textureDesc;
    textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    textureDesc.Format = DXGI_FORMAT_D32_FLOAT;

    ID3D11Texture2D* depthTexture;
    if(FAILED(device->CreateTexture2D(&textureDesc, 0, &depthTexture)))
    {
        Logger::LogError("DirectX: Depth buffer texture creation failed");
        return false;
    }

    D3D11_DEPTH_STENCIL_VIEW_DESC depthBufferDesc;
    ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
    depthBufferDesc.Format = textureDesc.Format;
    depthBufferDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
    depthBufferDesc.Texture2D.MipSlice = 0;

    if(FAILED(device->CreateDepthStencilView(
        depthTexture, &depthBufferDesc, &sm_depthBuffer)))
    {
        Logger::LogError("DirectX: Depth buffer creation failed");
        return false;
    }

    SetDebugName(depthTexture, "DepthBufferTexture");
    SetDebugName(sm_depthBuffer, "DepthBuffer");
    depthTexture->Release();

    return true;
}

bool DxRenderTarget::Initialise(ID3D11Device* device, IDXGISwapChain* swapchain)
{
    if (m_isBackBuffer)
    {
        swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&m_texture);
        if (FAILED(device->CreateRenderTargetView(m_texture, nullptr, &m_renderTarget)))
        {
            Logger::LogError("DirectX: Failed to create back buffer");
            return false;
        }

        m_texture->GetDesc(&sm_textureDesc);

        if (!InitialiseDepthBuffer(device))
        {
            Logger::LogError("DirectX: Failed to create depth buffer");
            return false;
        }
    }
    else
    {
        D3D11_TEXTURE2D_DESC textureDesc = sm_textureDesc;
        textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

        if (FAILED(device->CreateTexture2D(&textureDesc, 0, &m_texture)))
        {
            Logger::LogError("DirectX: Failed to create texture for " + m_name);
            return false;
        }

        D3D11_RENDER_TARGET_VIEW_DESC renderTargetDesc;
        ZeroMemory(&renderTargetDesc, sizeof(renderTargetDesc));
        renderTargetDesc.Format = textureDesc.Format;
        renderTargetDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
        renderTargetDesc.Texture2D.MipSlice = 0;

        if (FAILED(device->CreateRenderTargetView(m_texture, &renderTargetDesc, &m_renderTarget)))
        {
            Logger::LogError("DirectX: Failed to create render target for " + m_name);
            return false;
        }

        D3D11_SHADER_RESOURCE_VIEW_DESC textureViewDesc;
        ZeroMemory(&textureViewDesc, sizeof(textureViewDesc));
        textureViewDesc.Format = textureDesc.Format;
        textureViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
        textureViewDesc.Texture2D.MipLevels = 1;
        textureViewDesc.Texture2D.MostDetailedMip = 0;

        if (FAILED(device->CreateShaderResourceView(m_texture, &textureViewDesc, &m_view)))
        {
            Logger::LogError("DirectX: Failed to create texture view for " + m_name);
            return false;
        }
    }

    SetDebugName(m_texture, m_name + "_Texture");
    SetDebugName(m_view, m_name + "_TextureView");
    SetDebugName(m_renderTarget, m_name + "_RenderTarget");
    return true;
}

void DxRenderTarget::SetActive(ID3D11DeviceContext* context)
{
    ID3D11DepthStencilView* zbuffer = !m_isBackBuffer ? sm_depthBuffer : nullptr;
	context->OMSetRenderTargets(1, &m_renderTarget, zbuffer);
    context->ClearRenderTargetView(m_renderTarget, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

    if(zbuffer)
    {
        context->ClearDepthStencilView(zbuffer, D3D11_CLEAR_DEPTH, 1.0f, 0);
    }
}

void DxRenderTarget::SendTexture(ID3D11DeviceContext* context, int slot)
{
    context->PSSetShaderResources(slot, 1, &m_view);
}

void DxRenderTarget::ClearTexture(ID3D11DeviceContext* context, int slot)
{
    ID3D11ShaderResourceView* nullView = nullptr;
    context->PSSetShaderResources(slot, 1, &nullView);
}