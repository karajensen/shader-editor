////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directxtarget.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "directxtarget.h"

DxRenderTarget::DxRenderTarget(const std::string& name, bool isBackBuffer) :
    m_isBackBuffer(isBackBuffer),
    m_name(name),
    m_texture(nullptr),
    m_renderTarget(nullptr),
    m_textureView(nullptr)
{
}

DxRenderTarget::~DxRenderTarget()
{
    Release();
}

void DxRenderTarget::Release()
{
    if(m_texture)
    {
        m_texture->Release();
        m_texture = nullptr;
    }

    if(m_renderTarget)
    {
        m_renderTarget->Release();
        m_renderTarget = nullptr;
    }

    if(m_textureView)
    {
        m_textureView->Release();
        m_textureView = nullptr;
    }
}

bool DxRenderTarget::Initialise(ID3D11Device* device, IDXGISwapChain* swapchain)
{
    if(m_isBackBuffer)
    {
        swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&m_texture);
        if(FAILED(device->CreateRenderTargetView(m_texture, nullptr, &m_renderTarget)))
        {
            Logger::LogError("DirectX: Failed to create render target for " + m_name);
            return false;
        }
    }
    else
    {
        D3D11_TEXTURE2D_DESC textureDesc;
	    ZeroMemory(&textureDesc, sizeof(textureDesc));
	    textureDesc.Width = WINDOW_WIDTH;
	    textureDesc.Height = WINDOW_HEIGHT;
	    textureDesc.MipLevels = 1;
	    textureDesc.ArraySize = 1;
	    textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	    textureDesc.SampleDesc.Count = 1;
	    textureDesc.Usage = D3D11_USAGE_DEFAULT;
	    textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	    textureDesc.CPUAccessFlags = 0;
	    textureDesc.MiscFlags = 0;

	    if(FAILED(device->CreateTexture2D(&textureDesc, 0, &m_texture)))
        {
            Logger::LogError("DirectX: Failed to create texture for " + m_name);
            return false;
        }

	    D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	    renderTargetViewDesc.Format = textureDesc.Format;
	    renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	    renderTargetViewDesc.Texture2D.MipSlice = 0;

	    if(FAILED(device->CreateRenderTargetView(m_texture,
            &renderTargetViewDesc, &m_renderTarget)))
        {
            Logger::LogError("DirectX: Failed to create render target for " + m_name);
            return false;
        }

	    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	    shaderResourceViewDesc.Format = textureDesc.Format;
	    shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	    shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	    shaderResourceViewDesc.Texture2D.MipLevels = 1;

	    if(FAILED(device->CreateShaderResourceView(m_texture,
            &shaderResourceViewDesc, &m_textureView)))
        {
            Logger::LogError("DirectX: Failed to create texture view for " + m_name);
            return false;
        }
    }

	return true;
}

void DxRenderTarget::SetActive(ID3D11DeviceContext* context, ID3D11DepthStencilView* zbuffer)
{
	context->OMSetRenderTargets(1, &m_renderTarget, zbuffer);
    context->ClearRenderTargetView(m_renderTarget, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
    context->ClearDepthStencilView(zbuffer, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

ID3D11ShaderResourceView* DxRenderTarget::GetTexture()
{
	return m_textureView;
}