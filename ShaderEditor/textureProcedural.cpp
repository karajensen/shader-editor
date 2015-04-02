////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - textureProcedural.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "textureProcedural.h"
#include "common.h"
#include "colour.h"
#include "cache.h"
#include "soil/SOIL.h"
#include "boost/filesystem.hpp"
#include "boost/algorithm/string.hpp"

ProceduralTexture::ProceduralTexture(const std::string& name, 
                                     const std::string& path,
                                     int size,
                                     Type type,
                                     Filter filter) :
    Texture(name, path, filter),
    m_type(type)
{
    m_size = size;
    m_pixels.resize(size * size);

    m_savePath = boost::filesystem::initial_path().string();
    m_savePath += std::string(path.begin() + 1, path.end()); // Remove .
    boost::algorithm::ireplace_all(m_savePath, R"(\)", R"(/)");
    boost::algorithm::ireplace_all(m_savePath, R"(//)", R"(/)");

    switch (type)
    {
    case RANDOM:
        MakeRandomNormals();
        break;
    case DIAMOND_SQUARE:
        MakeDiamondSquareFractal();
        break;
    }

    Logger::LogInfo("Texture: " + Name() + " generated");
}

void ProceduralTexture::MakeRandomNormals()
{
    for (unsigned int i = 0; i < m_pixels.size(); ++i)
    {
        Float3 colour(Random::Generate(0.0f, 1.0f),
                      Random::Generate(0.0f, 1.0f),
                      Random::Generate(0.0f, 1.0f));

        colour.Normalize();
        SetRed(i, colour.x);
        SetGreen(i, colour.y);
        SetBlue(i, colour.z);
    }
}

const std::vector<unsigned int>& ProceduralTexture::Pixels() const
{
    return m_pixels;
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
        data[j] = RedAsChar(i);
        data[j+1] = GreenAsChar(i);
        data[j+2] = BlueAsChar(i);
    }

    if (SOIL_save_image(m_savePath.c_str(), SOIL_SAVE_TYPE_BMP, 
        m_size, m_size, channels, &data[0]) == 0)
    {
        Logger::LogError("Failed to save " + Path());
    }
    else
    {
        Logger::LogInfo("Saved texture " + Path());
    }
}

void ProceduralTexture::Write(Cache& cache)
{
    cache.TexturePath.SetUpdated(m_savePath);
}

void ProceduralTexture::Read(Cache& cache)
{
}

void ProceduralTexture::SetRed(unsigned int index, int value)
{
    Set(index, value, GreenAsInt(index),
        BlueAsInt(index), AlphaAsInt(index));
}

void ProceduralTexture::SetGreen(unsigned int index, int value)
{
    Set(index, RedAsInt(index), value, 
        BlueAsInt(index), AlphaAsInt(index));
}

void ProceduralTexture::SetBlue(unsigned int index, int value)
{
    Set(index, RedAsInt(index), 
        GreenAsInt(index), value, AlphaAsInt(index));
}

void ProceduralTexture::SetAlpha(unsigned int index, int value)
{
    Set(index, RedAsInt(index), 
        GreenAsInt(index), BlueAsInt(index), value);
}

void ProceduralTexture::SetRed(unsigned int index, float value)
{
    SetRed(index, static_cast<int>(value * 255));
}

void ProceduralTexture::SetGreen(unsigned int index, float value)
{
    SetGreen(index, static_cast<int>(value * 255));
}

void ProceduralTexture::SetBlue(unsigned int index, float value)
{
    SetBlue(index, static_cast<int>(value * 255));
}

void ProceduralTexture::SetAlpha(unsigned int index, float value)
{
    SetAlpha(index, static_cast<int>(value * 255));
}

void ProceduralTexture::Set(unsigned int index, float r, float g, float b, float a)
{
    Set(index, 
        static_cast<int>(r * 255.0f), 
        static_cast<int>(g * 255.0f),
        static_cast<int>(b * 255.0f), 
        static_cast<int>(a * 255.0f));
}

void ProceduralTexture::Set(unsigned int index, int r, int g, int b, int a)
{
    using namespace std;
    r = min(255, max(0, r));
    g = min(255, max(0, g));
    b = min(255, max(0, b));
    a = min(255, max(0, a));
    m_pixels[index] = ((a & 0xFF) << 24) + 
                      ((b & 0xFF) << 16) + 
                      ((g & 0xFF) << 8) + 
                      (r & 0xFF);
}

void ProceduralTexture::Set(int row, int column, float value)
{
    Set(Index(row, column), value, value, value, value);
}

unsigned char ProceduralTexture::RedAsChar(unsigned int index)
{
    return m_pixels[index] & 0xFF;
}

unsigned char ProceduralTexture::GreenAsChar(unsigned int index)
{
    return (m_pixels[index] >> 8) & 0xFF;
}

unsigned char ProceduralTexture::BlueAsChar(unsigned int index)
{
    return (m_pixels[index] >> 16) & 0xFF;
}

unsigned char ProceduralTexture::AlphaAsChar(unsigned int index)
{
    return (m_pixels[index] >> 24) & 0xFF;
}

int ProceduralTexture::RedAsInt(unsigned int index)
{
    return RedAsChar(index);
}

int ProceduralTexture::GreenAsInt(unsigned int index)
{
    return GreenAsChar(index);
}

int ProceduralTexture::BlueAsInt(unsigned int index)
{
    return BlueAsChar(index);
}

int ProceduralTexture::AlphaAsInt(unsigned int index)
{
    return AlphaAsChar(index);
}

float ProceduralTexture::RedAsFlt(unsigned int index)
{
    return static_cast<float>(RedAsChar(index)) / 255.0f;
}

float ProceduralTexture::GreenAsFlt(unsigned int index)
{
    return static_cast<float>(GreenAsChar(index)) / 255.0f;
}

float ProceduralTexture::BlueAsFlt(unsigned int index)
{
    return static_cast<float>(BlueAsChar(index)) / 255.0f;
}

float ProceduralTexture::AlphaAsFlt(unsigned int index)
{
    return static_cast<float>(AlphaAsChar(index)) / 255.0f;
}

unsigned int ProceduralTexture::Get(int row, int column) const
{
    return m_pixels[Index(row, column)];
}

unsigned int ProceduralTexture::Index(int row, int column) const
{
    return row * m_size + column;
}

void ProceduralTexture::MakeDiamondSquareFractal()
{
    // Diamond Square Fractal References:
    // http://www.gameprogrammer.com/fractal.html 
    // http://www.playfuljs.com/realistic-terrain-in-130-lines/

    const int maxSize = m_size;
    const int maxIndex = maxSize - 1;
    int size = maxSize;
    int half = size / 2;
    float scale = 2.0f;

    // All four corners must have the same point to allow tiling
    const float initial = 0.0f;
    Set(0, 0, initial);
    Set(maxIndex, 0, initial);
    Set(maxIndex, maxIndex, initial);
    Set(0, maxIndex, initial);

    /**
    * Averages the value of the four points in a square
    * o  .  o
    * .  .  .
    * o  .  o
    */
    auto AverageSquare = [&](int row, int column) -> float
    {
        return (RedAsFlt(Index(row-half, column-half)) +
            RedAsFlt(Index(row+half-1, column+half-1)) +
            RedAsFlt(Index(row-half, column+half-1)) +
            RedAsFlt(Index(row+half-1, column-half))) * 0.25f;
    };

    /**
    * Averages the value of the four points in a diamond
    * If row/column is on edge of texture looks at opposite side to support tiling
    * .  o  .
    * o  .  o
    * .  o  .
    */
    auto AverageDiamond = [&](int row, int column) -> float
    {
        const auto top = Index(row, column-half);
        const auto bot = Index(row, column+half-1);
        const auto left = Index(row-half, column);
        const auto right = Index(row+half-1, column);

        if (row == 0)
        {
            return (RedAsFlt(top) + RedAsFlt(bot) + RedAsFlt(right)
                + RedAsFlt(Index(maxIndex-half, column))) * 0.25f;
        }
        else if (row == maxIndex)
        {
            return (RedAsFlt(top) + RedAsFlt(bot) + RedAsFlt(left) 
                + RedAsFlt(Index(half, column))) * 0.25f;
        }
        else if (column == 0)
        {
            return (RedAsFlt(right) + RedAsFlt(left) + RedAsFlt(bot) 
                + RedAsFlt(Index(row, maxIndex-half))) * 0.25f;
        }
        else if (column == maxIndex)
        {
            return (RedAsFlt(right) + RedAsFlt(left) + RedAsFlt(bot) 
                + RedAsFlt(Index(row, half))) * 0.25f;
        }

        return (RedAsFlt(right) + RedAsFlt(left) 
            + RedAsFlt(top) + RedAsFlt(bot)) * 0.25f;
    };

    // Terminate loop when power of 2 texture halves to reach 0.5
    while (half >= 1)
    {
        /**
        * Set the midpoint of the sections
        * .  .  .
        * .  o  .
        * .  .  .
        */
        for (int r = half; r < maxSize; r += size) 
        {
            for (int c = half; c < maxSize; c += size)
            {
                Set(r, c, AverageSquare(r, c) + scale * Random::Generate(-0.5f, 0.5f));
            }
        }

        /**
        * Set the four corners of the diamond surrounding the midpoint
        * .  o  .
        * o  .  o
        * .  o  .
        */
        for (int r = 0; r <= maxIndex; r += half)
        {
            for (int c = (r + half) % size; c <= maxIndex; c += size)
            {
                Set(r, c, AverageDiamond(r, c) + scale * Random::Generate(-0.5f, 0.5f));
        
                if (r == 0) // Ensure opposite side has matching value for wrapping
                {
                    m_pixels[Index(maxIndex, c)] = m_pixels[Index(r, c)];
                }
                if (c == 0) // Ensure opposite side has matching value for wrapping
                {
                    m_pixels[Index(r, maxIndex)] = m_pixels[Index(r, c)];
                }
            }
        }

        size = half;
        half = size / 2;
    }   
}