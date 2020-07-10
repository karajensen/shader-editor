////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directx_texture.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "directx_common.h"

/**
* Holds an individual directx texture
*/
class DxTexture : boost::noncopyable
{
public:

    /**
    * Constructor
    * @param texture Contains the texture data
    */
    DxTexture(const Texture& texture);

    /**
    * Destructor
    */
    ~DxTexture();

    /**
    * Releases the texture
    */
    void Release();

    /**
    * Initialises the texture
    * @param device The DirectX device interface
    */
    void Initialise(ID3D11Device* device);

    /**
    * Reloads the texture from pixels
    * @param device The DirectX device interface
    * @return whether reloading was successful
    */
    bool ReloadPixels(ID3D11Device* device);

    /**
    * Gets the texture
    */
    ID3D11ShaderResourceView** Get();

    /**
    * @return the type of filtering for this texture
    */
    Texture::Filter Filtering() const;

    /**
    * @return the filename of the texture
    */
    const std::string& Name() const;

private:

    /**
    * Initialises a cube map
    * @param device The DirectX device interface
    */
    void InitialiseCubeMap(ID3D11Device* device);

    /**
    * Initialises the texture from file
    * @param device The DirectX device interface
    */
    void InitialiseFromFile(ID3D11Device* device);

    /**
    * Initialises the texture from pixels
    * @param device The DirectX device interface
    */
    void InitialiseFromPixels(ID3D11Device* device);

private:

    const Texture& m_texture;  ///< Contains the texture data
    ID3D11ShaderResourceView* m_view = nullptr; ///< The texture to send to shaders
};
