////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - textureProcedural.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "textureProcedural.h"

ProceduralTexture::ProceduralTexture(const std::string& name, 
                                     const std::string& path,
                                     int size,
                                     Type type) :
    Texture(name, path),
    m_type(type)
{
    m_size = size;
    m_pixels.resize(size * size);

    switch (type)
    {
    case RANDOM:
        MakeRandomNormals();
        break;
    }
}

void ProceduralTexture::MakeRandomNormals()
{
    for (unsigned int i = 0; i < m_pixels.size(); ++i)
    {
        // Temporary for testing
        m_pixels[i] = std::rand() % 255;
    }
}