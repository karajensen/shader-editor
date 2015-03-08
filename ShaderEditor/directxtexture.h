////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directxtexture.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "directxcommon.h"

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
    * Sends the texture to the currently active shader
    * @param context Direct3D device context
    * @param slot The slot to put the texture into
    */
    void SendTexture(ID3D11DeviceContext* context, int slot);

    /**
    * Reloads the texture from pixels
    */
    void ReloadPixels();

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

    const Texture& m_texture;  ///< Contains the texture data
    ID3D11ShaderResourceView* m_view = nullptr; ///< The texture to send to shaders
};