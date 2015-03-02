////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directxtarget.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "directxtarget.h"

D3D11_TEXTURE2D_DESC DxRenderTarget::sm_textureDesc;

DxRenderTarget::DxRenderTarget(const std::string& name) :
    m_isBackBuffer(true),
    m_name(name),
    m_count(1)
{
    InitialiseContainers();
}

DxRenderTarget::DxRenderTarget(const std::string& name, int textures) :
    m_isBackBuffer(false),
    m_name(name),
    m_count(textures)
{
    InitialiseContainers();
}

void DxRenderTarget::InitialiseContainers()
{
    m_views.resize(m_count);
    m_textures.resize(m_count);
    m_targets.resize(m_count);

    m_views.assign(m_count, nullptr);
    m_textures.assign(m_count, nullptr);
    m_targets.assign(m_count, nullptr);
}

DxRenderTarget::~DxRenderTarget()
{
    Release();
}

void DxRenderTarget::Release()
{
    for (unsigned int i = 0; i < m_textures.size(); ++i)
    {
        SafeRelease(&(m_textures[i]));
    }

    for (unsigned int i = 0; i < m_targets.size(); ++i)
    {
        SafeRelease(&(m_targets[i]));
    }

    for (unsigned int i = 0; i < m_views.size(); ++i)
    {
        SafeRelease(&(m_views[i]));
    }

    SafeRelease(&m_depthBuffer);
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
        depthTexture, &depthBufferDesc, &m_depthBuffer)))
    {
        Logger::LogError("DirectX: Depth buffer creation failed");
        return false;
    }

    SetDebugName(depthTexture, m_name + "DepthBufferTexture");
    SetDebugName(m_depthBuffer, m_name + "DepthBuffer");
    depthTexture->Release();

    return true;
}

bool DxRenderTarget::InitialiseBackBuffer(ID3D11Device* device, IDXGISwapChain* swapchain)
{
    swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&(m_textures[0]));
    if (FAILED(device->CreateRenderTargetView(m_textures[0], nullptr, &(m_targets[0]))))
    {
        Logger::LogError("DirectX: Failed to create back buffer");
        return false;
    }

    m_textures[0]->GetDesc(&sm_textureDesc);

    SetDebugName(m_textures[0], m_name + "_Texture");
    SetDebugName(m_targets[0], m_name + "_RenderTarget");

    return true;
}

bool DxRenderTarget::InitialiseRenderTarget(ID3D11Device* device, int ID)
{
    const auto name = m_name + boost::lexical_cast<std::string>(ID);

    D3D11_TEXTURE2D_DESC textureDesc = sm_textureDesc;
    textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

    if (FAILED(device->CreateTexture2D(&textureDesc, 0, &(m_textures[ID]))) ||
        m_textures[ID] == nullptr)
    {
        Logger::LogError("DirectX: Failed to create texture for " + name);
        return false;
    }

    D3D11_RENDER_TARGET_VIEW_DESC renderTargetDesc;
    ZeroMemory(&renderTargetDesc, sizeof(renderTargetDesc));
    renderTargetDesc.Format = textureDesc.Format;
    renderTargetDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
    renderTargetDesc.Texture2D.MipSlice = 0;

    if (FAILED(device->CreateRenderTargetView(m_textures[ID], &renderTargetDesc, &(m_targets[ID]))) ||
        m_targets[ID] == nullptr)
    {
        Logger::LogError("DirectX: Failed to create render target for " + name);
        return false;
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC textureViewDesc;
    ZeroMemory(&textureViewDesc, sizeof(textureViewDesc));
    textureViewDesc.Format = textureDesc.Format;
    textureViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
    textureViewDesc.Texture2D.MipLevels = 1;
    textureViewDesc.Texture2D.MostDetailedMip = 0;

    if (FAILED(device->CreateShaderResourceView(m_textures[ID], &textureViewDesc, &(m_views[ID]))) ||
        m_views[ID] == nullptr)
    {
        Logger::LogError("DirectX: Failed to create texture view for " + name);
        return false;
    }

    SetDebugName(m_textures[ID], name + "_Texture");
    SetDebugName(m_views[ID], name + "_TextureView");
    SetDebugName(m_targets[ID], name + "_RenderTarget");

    return true;
}

bool DxRenderTarget::Initialise(ID3D11Device* device, IDXGISwapChain* swapchain)
{
    if (m_isBackBuffer)
    {
        return InitialiseBackBuffer(device, swapchain);
    }

    if (!InitialiseDepthBuffer(device))
    {
        Logger::LogError("DirectX: Failed to create depth buffer");
        return false;
    }

    for (int i = 0; i < m_count; ++i)
    {
        if (!InitialiseRenderTarget(device, i))
        {
            Logger::LogError("DirectX: Failed to create render target");
            return false;
        }
    }

    return true;
}

void DxRenderTarget::SetActive(ID3D11DeviceContext* context)
{
    if (m_isBackBuffer)
    {
        context->OMSetRenderTargets(m_count, &m_targets[0], nullptr);
    }
    else
    {
        context->OMSetRenderTargets(m_count, &m_targets[0], m_depthBuffer);
        context->ClearDepthStencilView(m_depthBuffer, D3D11_CLEAR_DEPTH, 1.0f, 0);
    }

    for (unsigned int i = 0; i < m_targets.size(); ++i)
    {
        context->ClearRenderTargetView(m_targets[i], D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
    }
}

void DxRenderTarget::SendTexture(ID3D11DeviceContext* context, int slot, int ID)
{
    context->PSSetShaderResources(slot, 1, &m_views[ID]);
}

void DxRenderTarget::ClearTexture(ID3D11DeviceContext* context, int slot)
{
    ID3D11ShaderResourceView* nullView = nullptr;
    context->PSSetShaderResources(slot, 1, &nullView);
}