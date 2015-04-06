////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - terrain.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "terrain.h"
#include "cache.h"
#include "boost/algorithm/string.hpp"

Terrain::Terrain(const boost::property_tree::ptree& node, 
                 const std::vector<unsigned int>& pixels,
                 const std::string& heightmap) :
    Grid(node),
    m_pixels(pixels),
    m_heightmap(heightmap)
{
    m_causticsAmount = GetAttributeOptional<float>(node, "Caustics", "amount", 1.0f);
    m_causticsScale = GetAttributeOptional<float>(node, "Caustics", "scale", 1.0f);
    m_specularity = GetValueOptional<float>(node, 0.0f, "Specularity");
    m_ambience = GetValueOptional<float>(node, 1.0f, "Ambience");
    m_bump = GetValueOptional<float>(node, 0.0f, "Bump");
    m_minHeight = GetAttribute<float>(node, "Height", "min");
    m_maxHeight = GetAttribute<float>(node, "Height", "max");
    m_height = GetAttribute<float>(node, "Height", "start");
    m_uvScale.x = GetAttributeOptional<float>(node, "UVScale", "u", 1.0f);
    m_uvScale.y = GetAttributeOptional<float>(node, "UVScale", "v", 1.0f);
}

void Terrain::Write(boost::property_tree::ptree& node) const
{
    Grid::Write(node);

    node.add("HeightMap", m_heightmap);
    node.add("Height.<xmlattr>.min", m_minHeight);
    node.add("Height.<xmlattr>.max", m_maxHeight);
    node.add("Height.<xmlattr>.start", m_height);
    AddValueOptional(node, "Bump", m_bump, 0.0f);
    AddValueOptional(node, "Ambience", m_ambience, 1.0f);
    AddValueOptional(node, "Specularity", m_specularity, 0.0f);
    AddValueOptional(node, "Caustics.<xmlattr>.amount", m_causticsAmount, 1.0f);
    AddValueOptional(node, "Caustics.<xmlattr>.scale", m_causticsScale, 1.0f);
    AddValueOptional(node, "UVScale.<xmlattr>.u", m_uvScale.x, 1.0f);
    AddValueOptional(node, "UVScale.<xmlattr>.v", m_uvScale.y, 1.0f);
}

void Terrain::Write(Cache& cache)
{
    cache.Terrain[TERRAIN_CAUSTICS_AMOUNT].SetUpdated(m_causticsAmount);
    cache.Terrain[TERRAIN_CAUSTICS_SCALE].SetUpdated(m_causticsScale);
    cache.Terrain[TERRAIN_BUMP].SetUpdated(m_bump);
    cache.Terrain[TERRAIN_AMBIENCE].SetUpdated(m_ambience);
    cache.Terrain[TERRAIN_SPECULARITY].SetUpdated(m_specularity);
    cache.Terrain[TERRAIN_MIN_HEIGHT].SetUpdated(m_minHeight);
    cache.Terrain[TERRAIN_MAX_HEIGHT].SetUpdated(m_maxHeight);
    cache.Terrain[TERRAIN_SCALE_U].SetUpdated(m_uvScale.x);
    cache.Terrain[TERRAIN_SCALE_V].SetUpdated(m_uvScale.y);
    cache.TerrainShader.SetUpdated(ShaderName());
}

void Terrain::Read(Cache& cache)
{
    m_causticsAmount = cache.Terrain[TERRAIN_CAUSTICS_AMOUNT].Get();
    m_causticsScale = cache.Terrain[TERRAIN_CAUSTICS_SCALE].Get();
    m_bump = cache.Terrain[TERRAIN_BUMP].Get();
    m_specularity = cache.Terrain[TERRAIN_SPECULARITY].Get();
    m_ambience = cache.Terrain[TERRAIN_AMBIENCE].Get();
    m_minHeight = cache.Terrain[TERRAIN_MIN_HEIGHT].Get();
    m_maxHeight = cache.Terrain[TERRAIN_MAX_HEIGHT].Get();
    m_uvScale.x = cache.Terrain[TERRAIN_SCALE_U].Get();
    m_uvScale.y = cache.Terrain[TERRAIN_SCALE_V].Get();
}

bool Terrain::Initialise(bool hasNormals, 
                         bool hasTangents)
{
    if (CreateGrid(m_uvScale, hasNormals, hasTangents))
    {
        GenerateTerrain();
        RecalculateNormals();
        Logger::LogInfo("Terrain: " + Name() + " generated");
        return true;
    }
    return false;
}

void Terrain::Reload()
{
    ResetGrid(m_uvScale);
    GenerateTerrain();
    RecalculateNormals();
}

void Terrain::GenerateTerrain()
{
    assert(Rows() == Columns());

    const int gridSize = Rows();
    const int mapSize = static_cast<int>(sqrt(static_cast<double>(m_pixels.size())));
    const double stepIncrease = static_cast<double>(mapSize / gridSize);
    double step = 0.0;

    for (int r = 0; r < gridSize; ++r)
    {
        for (int c = 0; c < gridSize; ++c)
        {
            const int index = static_cast<int>(std::round(step));
            const float colour = Clamp((m_pixels[index] & 0xFF) / 255.0f, 0.0f, 1.0f);
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

const float& Terrain::CausticsScale() const
{
    return m_causticsScale;
}

const float& Terrain::CausticsAmount() const
{
    return m_causticsAmount;
}

void Terrain::SetInstance(int index, const Float2& position)
{
    m_instances[index].position.x = position.x;
    m_instances[index].position.y = m_height;
    m_instances[index].position.z = position.y;
}

void Terrain::AddInstance(const Float2& position)
{
    m_instances.emplace_back();
    SetInstance(static_cast<int>(m_instances.size()-1), position);
}