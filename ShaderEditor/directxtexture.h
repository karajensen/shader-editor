////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - directxtexture.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "directxcommon.h"

/**
* Holds an individual directx texture
*/
class DxTexture
{
public:

    /**
    * Constructor
    * @param filepath The path for the texture
    */
    DxTexture(const std::string& filepath);

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
    * @param device The directx device
    */
    void Initialise(ID3D11Device* device);

private:

    const std::string m_filepath;        ///< The path for the texture
    ID3D11ShaderResourceView* m_texture; ///< The actual texture
};