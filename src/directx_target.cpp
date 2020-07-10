////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directx_target.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "directx_target.h"
#include "logger.h"

#include <boost/lexical_cast.hpp>

D3D11_TEXTURE2D_DESC DxRenderTarget::sm_textureDesc;

DxRenderTarget::DxRenderTarget(const std::string& name)
    : m_isBackBuffer(true)
    , m_name(name)
    , m_count(1)
{
    InitialiseContainers();
}

DxRenderTarget::DxRenderTarget(const std::string& name, 
                               int textures, 
                               bool multisampled, 
                               bool readWrite) :

    m_multisampled(multisampled),
    m_name(name),
    m_count(textures),
    m_readWrite(readWrite)
{
    InitialiseContainers();
}

void DxRenderTarget::InitialiseContainers()
{
    m_highQuality.resize(m_count);
    m_highQuality.assign(m_count, false);

    m_views.resize(m_count);
    m_views.assign(m_count, nullptr);
    
    m_textures.resize(m_count);
    m_textures.assign(m_count, nullptr);

    m_targets.resize(m_count);
    m_targets.assign(m_count, nullptr);

    if (m_readWrite)
    {
        m_copiedViews.resize(m_count);
        m_copiedViews.assign(m_count, nullptr);

        m_copiedTextures.resize(m_count);
        m_copiedTextures.assign(m_count, nullptr);
    }
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

    for (unsigned int i = 0; i < m_copiedTextures.size(); ++i)
    {
        SafeRelease(&(m_copiedTextures[i]));
    }

    for (unsigned int i = 0; i < m_targets.size(); ++i)
    {
        SafeRelease(&(m_targets[i]));
    }

    for (unsigned int i = 0; i < m_views.size(); ++i)
    {
        SafeRelease(&(m_views[i]));
    }

    for (unsigned int i = 0; i < m_copiedViews.size(); ++i)
    {
        SafeRelease(&(m_copiedViews[i]));
    }

    SafeRelease(&m_depthBuffer);
}

bool DxRenderTarget::InitialiseDepthBuffer(ID3D11Device* device)
{
    D3D11_TEXTURE2D_DESC textureDesc = sm_textureDesc;
    textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    textureDesc.SampleDesc.Count = m_multisampled ? MULTISAMPLING_COUNT : 1;

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

void DxRenderTarget::SetHighQuality(int index)
{
    m_highQuality[index] = true;
}

bool DxRenderTarget::InitialiseRenderTarget(ID3D11Device* device, int ID)
{
    const auto name = m_name + boost::lexical_cast<std::string>(ID);

    D3D11_TEXTURE2D_DESC textureDesc = sm_textureDesc;
    textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.SampleDesc.Count = m_multisampled ? MULTISAMPLING_COUNT : 1;
    textureDesc.Format = m_highQuality[ID] ? 
        DXGI_FORMAT_R32G32B32A32_FLOAT : DXGI_FORMAT_R8G8B8A8_UNORM;

    if (FAILED(device->CreateTexture2D(&textureDesc, 0, &(m_textures[ID]))))
    {
        Logger::LogError("DirectX: Failed to create texture for " + name);
        return false;
    }

    D3D11_RENDER_TARGET_VIEW_DESC renderTargetDesc;
    ZeroMemory(&renderTargetDesc, sizeof(renderTargetDesc));
    renderTargetDesc.Format = textureDesc.Format;
    renderTargetDesc.Texture2D.MipSlice = 0;
    renderTargetDesc.ViewDimension = m_multisampled ? 
        D3D11_RTV_DIMENSION_TEXTURE2DMS : D3D11_RTV_DIMENSION_TEXTURE2D;

    if (FAILED(device->CreateRenderTargetView(m_textures[ID], &renderTargetDesc, &(m_targets[ID]))))
    {
        Logger::LogError("DirectX: Failed to create render target for " + name);
        return false;
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC textureViewDesc;
    ZeroMemory(&textureViewDesc, sizeof(textureViewDesc));
    textureViewDesc.Format = textureDesc.Format;
    textureViewDesc.Texture2D.MipLevels = 1;
    textureViewDesc.Texture2D.MostDetailedMip = 0;
    textureViewDesc.ViewDimension = m_multisampled ? 
        D3D11_SRV_DIMENSION_TEXTURE2DMS : D3D11_SRV_DIMENSION_TEXTURE2D;

    if (FAILED(device->CreateShaderResourceView(m_textures[ID], &textureViewDesc, &(m_views[ID]))))
    {
        Logger::LogError("DirectX: Failed to create texture view for " + name);
        return false;
    }

    SetDebugName(m_textures[ID], name + "_Texture");
    SetDebugName(m_targets[ID], name + "_RenderTarget");
    SetDebugName(m_views[ID], name + "_TextureView");

    if (m_readWrite)
    {
        textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        if (FAILED(device->CreateTexture2D(&textureDesc, 0, &(m_copiedTextures[ID]))))
        {
            Logger::LogError("DirectX: Failed to create copy texture for " + name);
            return false;
        }

        if (FAILED(device->CreateShaderResourceView(m_copiedTextures[ID], &textureViewDesc, &(m_copiedViews[ID]))))
        {
            Logger::LogError("DirectX: Failed to create copy texture view for " + name);
            return false;
        }

        SetDebugName(m_copiedTextures[ID], name + "_CopyTexture");
        SetDebugName(m_copiedViews[ID], name + "_CopyTextureView");
    }

    return true;
}

bool DxRenderTarget::Initialise(ID3D11Device* device, IDXGISwapChain* swapchain)
{
    assert(m_isBackBuffer);

    return InitialiseBackBuffer(device, swapchain);
}

bool DxRenderTarget::Initialise(ID3D11Device* device, ID3D11SamplerState* state)
{
    assert(!m_isBackBuffer);

    m_state = state;

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
    }

    ClearTarget(context);
}

void DxRenderTarget::ClearTarget(ID3D11DeviceContext* context)
{
    if (!m_isBackBuffer)
    {
        context->ClearDepthStencilView(m_depthBuffer, D3D11_CLEAR_DEPTH, 1.0f, 0);
    }

    for (unsigned int i = 0; i < m_targets.size(); ++i)
    {
        context->ClearRenderTargetView(m_targets[i], 
            D3DXCOLOR(0.22f, 0.49f, 0.85f, 0.0f));
    }
}

ID3D11ShaderResourceView* const* DxRenderTarget::Get(int ID) const
{
    return &(m_views[ID]);
}

ID3D11ShaderResourceView* const* DxRenderTarget::GetCopied(int ID) const
{
    return &(m_copiedViews[ID]);
}

void DxRenderTarget::CopyTextures(ID3D11DeviceContext* context)
{
    assert(m_readWrite);

    for (unsigned int i = 0; i < m_textures.size(); ++i)
    {
        context->CopyResource(m_copiedTextures[i], m_textures[i]);
    }

    ClearTarget(context);
}

ID3D11SamplerState* const* DxRenderTarget::State() const
{
    return &m_state;
}
