////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directxtexture.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "directxtexture.h"

DxTexture::DxTexture(const std::string& filepath) :
    m_filepath(filepath),
    m_texture(nullptr)
{
}

DxTexture::~DxTexture()
{
    Release();
}

void DxTexture::Release()
{
    if(m_texture)
    {
        m_texture->Release();
        m_texture = nullptr;
    }
}

void DxTexture::Initialise(ID3D11Device* device)
{
    if(FAILED(D3DX11CreateShaderResourceViewFromFile(device,
        m_filepath.c_str(), 0, 0, &m_texture, 0)))
    {
        Logger::LogError("Failed to create texture " + m_filepath);
    }
}

void DxTexture::SendTexture(ID3D11DeviceContext* context)
{
    if(m_texture)
    {
        context->PSSetShaderResources(0, 1, &m_texture);
    }
}