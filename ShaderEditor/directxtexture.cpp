////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directxtexture.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "directxtexture.h"
#include "boost/algorithm/string.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/filesystem.hpp"

DxTexture::DxTexture(const std::string& filepath) :
    m_filepath(filepath),
    m_isCubeMap(boost::filesystem::path(filepath).extension().string().empty())
{
}

DxTexture::~DxTexture()
{
    Release();
}

void DxTexture::Release()
{
    SafeRelease(&m_texture);
}

void DxTexture::Initialise(ID3D11Device* device)
{
    if (m_isCubeMap)
    {
        const std::string filePath = m_filepath + ".dds";
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

        if (FAILED(device->CreateShaderResourceView(texture, &viewDesc, &m_texture)))
        {
            Logger::LogError("DirectX: Failed to resource view " + filePath);
        }

        texture->Release();
    }
    else
    {
        if(FAILED(D3DX11CreateShaderResourceViewFromFile(device,
            m_filepath.c_str(), 0, 0, &m_texture, 0)))
        {
            Logger::LogError("DirectX: Failed to create texture " + m_filepath);
        }
    }

    SetDebugName(m_texture, m_filepath + "_view");
}

void DxTexture::SendTexture(ID3D11DeviceContext* context, int slot)
{
    context->PSSetShaderResources(slot, 1, &m_texture);
}