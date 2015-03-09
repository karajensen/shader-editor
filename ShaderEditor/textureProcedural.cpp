////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - textureProcedural.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "textureProcedural.h"
#include "common.h"
#include "colour.h"
#include "soil/SOIL.h"

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

void ProceduralTexture::SaveTexture()
{
    const int channels = 3;
    std::vector<unsigned char> data(m_pixels.size() * channels);

    for (unsigned int i = 0, j = 0; i < m_pixels.size(); ++i, j+=channels)
    {
        const unsigned int colour = m_pixels[i];
        data[j] = Colour::RedAsChar(colour);
        data[j+1] = Colour::GreenAsChar(colour);
        data[j+2] = Colour::BlueAsChar(colour);
    }

    if (SOIL_save_image(Path().c_str(), SOIL_SAVE_TYPE_BMP, 
        m_size, m_size, channels, &data[0]) == 0)
    {
        Logger::LogError("Failed to save " + Path());
    }
    else
    {
        Logger::LogInfo("Saved texture " + Path());
    }
}