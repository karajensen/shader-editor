////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directxtexture.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "directxtexture.h"
#include "boost/filesystem.hpp"

DxTexture::DxTexture(const Texture& texture) :
    m_texture(texture)
{
}

DxTexture::~DxTexture()
{
    Release();
}

void DxTexture::Release()
{
    SafeRelease(&m_view);
}

void DxTexture::Initialise(ID3D11Device* device)
{
    if (m_texture.IsRenderable())
    {
        if (m_texture.IsCubeMap())
        {
            InitialiseCubeMap(device);
        }
        else if (m_texture.HasPixels())
        {
            InitialiseFromPixels(device);
        }
        else
        {
            InitialiseFromFile(device);
        }

        SetDebugName(m_view, m_texture.Name() + "_view");
    }
}

void DxTexture::InitialiseFromFile(ID3D11Device* device)
{
    if(FAILED(D3DX11CreateShaderResourceViewFromFile(device,
        m_texture.Path().c_str(), 0, 0, &m_view, 0)))
    {
        Logger::LogError("DirectX: Failed to create texture " + m_texture.Path());
    }
}

void DxTexture::InitialiseCubeMap(ID3D11Device* device)
{
    const std::string filePath = m_texture.Path() + ".dds";
    if (!boost::filesystem::exists(filePath))
    {
        Logger::LogError("DirectX: " + filePath + " doesn't exist");
    }

	D3DX11_IMAGE_LOAD_INFO loadInfo;
	loadInfo.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	ID3D11Texture2D* texture = nullptr;
    SetDebugName(texture, filePath + "_texture");

    if (FAILED(D3DX11CreateTextureFromFile(device, filePath.c_str(),
        &loadInfo, 0, (ID3D11Resource**)&texture, 0)))
    {
        Logger::LogError("DirectX: Failed to create texture " + filePath);
    }

	D3D11_TEXTURE2D_DESC textureDesc;
	texture->GetDesc(&textureDesc);

	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = textureDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	viewDesc.TextureCube.MipLevels = textureDesc.MipLevels;
	viewDesc.TextureCube.MostDetailedMip = 0;

    if (FAILED(device->CreateShaderResourceView(texture, &viewDesc, &m_view)))
    {
        Logger::LogError("DirectX: Failed to resource view " + filePath);
    }

    texture->Release();
}

bool DxTexture::ReloadPixels(ID3D11Device* device)
{
    if (m_texture.IsRenderable())
    {
        Release();

        ID3D11Texture2D* texture = nullptr;
        const int channels = 4;
        const int size = m_texture.Size();

        D3D11_SUBRESOURCE_DATA data;
        data.pSysMem = (void*)(&m_texture.Pixels()[0]);
        data.SysMemPitch = size*channels;

        D3D11_TEXTURE2D_DESC desc;
        desc.Width = size;
        desc.Height = size;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;

        if (FAILED(device->CreateTexture2D(&desc, &data, &texture)))
        {
            Logger::LogError("DirectX: Failed to create texture from pixels");
            return false;
        }

        D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
        viewDesc.Format = desc.Format;
        viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        viewDesc.TextureCube.MipLevels = 1;
        viewDesc.TextureCube.MostDetailedMip = 0;

        if (FAILED(device->CreateShaderResourceView(texture, &viewDesc, &m_view)))
        {
            Logger::LogError("DirectX: Failed to resource view " + m_texture.Name());
            return false;
        }

        texture->Release();
    }
    return true;
}

void DxTexture::InitialiseFromPixels(ID3D11Device* device)
{
    ReloadPixels(device);
}

ID3D11ShaderResourceView** DxTexture::Get()
{
    return &m_view;
}

Texture::Filter DxTexture::Filtering() const
{
    return m_texture.Filtering();
}

const std::string& DxTexture::Name() const
{
    return m_texture.Name();
}