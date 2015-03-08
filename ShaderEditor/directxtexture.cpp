////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directxtexture.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "directxtexture.h"

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

void DxTexture::InitialiseFromPixels(ID3D11Device* device)
{


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

void DxTexture::SendTexture(ID3D11DeviceContext* context, int slot)
{
    context->PSSetShaderResources(slot, 1, &m_view);
}

void DxTexture::ReloadPixels()
{

}