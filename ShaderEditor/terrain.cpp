////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - terrain.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "terrain.h"
#include "cache.h"
#include "boost/algorithm/string.hpp"

Terrain::Terrain(const boost::property_tree::ptree& node) :
    Grid(node)
{
    m_caustics = GetValueOptional<float>(node, 1.0f, "Caustics");
    m_specularity = GetValueOptional<float>(node, 0.0f, "Specularity");
    m_ambience = GetValueOptional<float>(node, 1.0f, "Ambience");
    m_bump = GetValueOptional<float>(node, 0.0f, "Bump");
    m_minHeight = GetAttribute<float>(node, "Height", "min");
    m_maxHeight = GetAttribute<float>(node, "Height", "max");
    m_heightmap = boost::to_lower_copy(GetValue<std::string>(node, "HeightMap"));
}

void Terrain::Write(boost::property_tree::ptree& node) const
{
    Grid::Write(node);

    node.add("HeightMap", m_heightmap);
    node.add("Height.<xmlattr>.min", m_minHeight);
    node.add("Height.<xmlattr>.max", m_maxHeight);
    AddValueOptional(node, "Bump", m_bump, 0.0f);
    AddValueOptional(node, "Caustics", m_caustics, 1.0f);
    AddValueOptional(node, "Ambience", m_ambience, 1.0f);
    AddValueOptional(node, "Specularity", m_specularity, 0.0f);
}

void Terrain::Write(Cache& cache)
{
    cache.Terrain[TERRAIN_CAUSTICS].SetUpdated(m_caustics);
    cache.Terrain[TERRAIN_BUMP].SetUpdated(m_bump);
    cache.Terrain[TERRAIN_AMBIENCE].SetUpdated(m_ambience);
    cache.Terrain[TERRAIN_SPECULARITY].SetUpdated(m_specularity);
    cache.Terrain[TERRAIN_MIN_HEIGHT].SetUpdated(m_minHeight);
    cache.Terrain[TERRAIN_MAX_HEIGHT].SetUpdated(m_maxHeight);
    cache.TerrainShader.SetUpdated(ShaderName());
}

void Terrain::Read(Cache& cache)
{
    m_caustics = cache.Terrain[TERRAIN_CAUSTICS].Get();
    m_bump = cache.Terrain[TERRAIN_BUMP].Get();
    m_specularity = cache.Terrain[TERRAIN_SPECULARITY].Get();
    m_ambience = cache.Terrain[TERRAIN_AMBIENCE].Get();
    m_minHeight = cache.Terrain[TERRAIN_MIN_HEIGHT].Get();
    m_maxHeight = cache.Terrain[TERRAIN_MAX_HEIGHT].Get();
}

const std::string& Terrain::HeightMap() const
{
    return m_heightmap;
}

bool Terrain::Initialise(const std::vector<unsigned int>& pixels, 
                         bool hasNormals, 
                         bool hasTangents)
{
    if (CreateGrid(hasNormals, hasTangents))
    {
        GenerateTerrain(pixels);
        RecalculateNormals();
        Logger::LogInfo("Terrain: " + Name() + " generated");
        return true;
    }
    return false;
}

void Terrain::GenerateTerrain(const std::vector<unsigned int>& pixels)
{
    assert(Rows() == Columns());

    const int gridSize = Rows();
    const int mapSize = static_cast<int>(sqrt(static_cast<double>(pixels.size())));
    const double stepIncrease = static_cast<double>(mapSize / gridSize);
    double step = 0.0;

    for (int r = 0; r < gridSize; ++r)
    {
        for (int c = 0; c < gridSize; ++c)
        {
            const int index = static_cast<int>(std::round(step));
            const float colour = Clamp((pixels[index] & 0xFF) / 255.0f, 0.0f, 1.0f);
            const float height = ConvertRange(colour, 0.0f, 1.0f, m_minHeight, m_maxHeight);
            SetHeight(r, c, height);
            step += stepIncrease;
        }
    }
}

const float& Terrain::Specularity() const
{
    return m_specularity;
}

const float& Terrain::Ambience() const
{
    return m_ambience;
}

const float& Terrain::Bump() const
{
    return m_bump;
}

const float& Terrain::Caustics() const
{
    return m_caustics;
}