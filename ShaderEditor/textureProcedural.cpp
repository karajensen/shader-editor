////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - textureProcedural.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "textureProcedural.h"
#include "common.h"
#include "colour.h"

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
        Float3 colour(Random::Generate(0.0f, 1.0f),
                      Random::Generate(0.0f, 1.0f),
                      Random::Generate(0.0f, 1.0f));

        colour.Normalize();

        Colour::SetRed(m_pixels[i], colour.x);
        Colour::SetGreen(m_pixels[i], colour.y);
        Colour::SetBlue(m_pixels[i], colour.z);
    }
}

const unsigned int* ProceduralTexture::Pixels() const
{
    return &m_pixels[0];
}

int ProceduralTexture::Size() const
{
    return m_size;
}

bool ProceduralTexture::HasPixels() const
{
    return true;
}