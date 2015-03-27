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
    m_type = boost::to_lower_copy(GetValue<std::string>(node, "Type"));
}

void Terrain::Write(boost::property_tree::ptree& node) const
{
    Grid::Write(node);

    node.add("Type", m_type);
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
    cache.TerrainShader.SetUpdated(ShaderName());
}

void Terrain::Read(Cache& cache)
{
    m_caustics = cache.Mesh[TERRAIN_CAUSTICS].Get();
    m_bump = cache.Mesh[TERRAIN_BUMP].Get();
    m_specularity = cache.Mesh[TERRAIN_SPECULARITY].Get();
    m_ambience = cache.Mesh[TERRAIN_AMBIENCE].Get();
}

bool Terrain::Initialise()
{
    CreateGrid(true);

    GenerateTerrain();

    RecalculateNormals();

    return true;
}

void Terrain::GenerateTerrain()
{
    if (boost::iequals(m_type, "diamond_square"))
    {
        GenerateDiamondSquareTerrain();
    }
    else
    {
        Logger::LogError("Unknown terrain type");
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

void Terrain::GenerateDiamondSquareTerrain()
{
}