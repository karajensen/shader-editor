////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - textureProcedural.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "textureProcedural.h"

ProceduralTexture::ProceduralTexture(const std::string& name, 
                                     const std::string& path,
                                     int size) :
    Texture(name, path)
{
    m_size = size;
    m_pixels.resize(size * size);
}

void ProceduralTexture::MakeRandomNormals()
{
    for (unsigned int i = 0; i < m_pixels.size(); ++i)
    {
        // Temporary for testing
        m_pixels[i].r = 255;
    }
}