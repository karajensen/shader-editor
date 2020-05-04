////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - terrain.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "terrain.h"
#include "cache.h"
#include "utils.h"
#include "logger.h"

Terrain::Terrain(const std::string& name, 
                 const std::string& shaderName,
                 int shaderID,
                 const std::vector<unsigned int>& pixels)
    : Grid(name, shaderName, shaderID)
    , m_pixels(pixels)
{
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
    cache.Terrain[TERRAIN_SCALE].SetUpdated(m_uvScale.x);
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
    m_uvScale.x = m_uvScale.y = cache.Terrain[TERRAIN_SCALE].Get();
}

bool Terrain::Initialise(float uvTextureStretch,
                         float minHeight,
                         float maxHeight,
                         float height,
                         float spacing,
                         int size,
                         bool hasNormals,
                         bool hasTangents,
                         bool requiresTiling)
{
    m_height = height;
    m_minHeight = minHeight;
    m_maxHeight = maxHeight;
    m_requiresTiling = requiresTiling;
    m_uvScale.x = uvTextureStretch;
    m_uvScale.y = uvTextureStretch;

    if (CreateGrid(m_uvScale, spacing, size, size, hasNormals, hasTangents))
    {
        GenerateTerrain();
        RecalculateNormals();
        InitialiseMeshData();
        Logger::LogInfo("Terrain: Generated " + Name());
        return true;
    }

    return false;
}

void Terrain::Reload()
{
    ResetGrid();
    GenerateTerrain();
    RecalculateNormals();
}

void Terrain::GenerateTerrain()
{
    if (Rows() != Columns())
    {
        Logger::LogError("Generate Terrain: Rows and columns should match");
    }

    const int gridSize = Rows();
    const int mapSize = static_cast<int>(sqrt(static_cast<double>(m_pixels.size())));
    const double stepIncrease = (mapSize / static_cast<double>(gridSize));
    double mapC = 0.0, mapR = 0.0;

    for (int r = 0; r < gridSize; ++r, mapR += stepIncrease)
    {
        for (int c = 0; c < gridSize; ++c, mapC += stepIncrease)
        {
            const int index = int(mapR) * mapSize + int(mapC);
            const float colour = Clamp((m_pixels[index] & 0xFF) / 255.0f, 0.0f, 1.0f);
            const float height = ConvertRange(colour, 0.0f, 1.0f, m_minHeight, m_maxHeight);
            SetHeight(r, c, height);
        }
        mapC = 0.0;
    }

    if (m_requiresTiling)
    {
        for (int c = 0; c < gridSize; ++c)
        {
            SetHeight(0, c, GetHeight(gridSize-1, c));
        }
        for (int r = 0; r < gridSize; ++r)
        {
            SetHeight(r, 0, GetHeight(r, gridSize-1));
        }
    }
}

void Terrain::AddInstances(int amount)
{
    MeshData::AddInstances(amount);
    for (int i = 0; i < amount; ++i)
    {
        m_maxBounds.emplace_back();
        m_minBounds.emplace_back();
    }
}

void Terrain::CalculateBounds(int instance)
{
    const auto& instanceMesh = GetInstance(instance);
    const float size = Size();
    const float halfWidth = size * instanceMesh.scale.x  * 0.5f;
    const float halfLength = size * instanceMesh.scale.z  * 0.5f;
    m_minBounds[instance].x = instanceMesh.position.x - halfWidth;
    m_minBounds[instance].y = instanceMesh.position.z - halfLength;
    m_maxBounds[instance].x = instanceMesh.position.x + halfWidth;
    m_maxBounds[instance].y = instanceMesh.position.z + halfLength;
}

void Terrain::SetInstance(int index, 
                          const Float3& position,
                          const Float3& rotation,
                          const Float3& scale)
{
    m_height = position.y;
    m_instances[index].position = position;
    m_instances[index].rotation = rotation;
    m_instances[index].scale = scale;
    m_instances[index].requiresUpdate = true;
    CalculateBounds(index);
}

void Terrain::SetInstance(int index, const Float2& position)
{
    m_instances[index].position.x = position.x;
    m_instances[index].position.y = m_height;
    m_instances[index].position.z = position.y;
    m_instances[index].requiresUpdate = true;
    CalculateBounds(index);
}

void Terrain::AddInstance(const Float2& position)
{
    Terrain::AddInstances(1);
    SetInstance(static_cast<int>(m_instances.size()-1), position);
}

Float3 Terrain::GetAbsolutePosition(int instance, float x, float z)
{
    const auto& world = GetWorldInstance(instance);
    const float maxIndex = Rows() - 1.0f;
    const Float2& minBounds = GetMinBounds(instance);
    const Float2& maxBounds = GetMaxBounds(instance);

    const int row = static_cast<int>(std::round(Clamp(ConvertRange(
        x, minBounds.x, maxBounds.x, 0.0f, maxIndex), 0.0f, maxIndex)));

    const int column = static_cast<int>(std::round(Clamp(ConvertRange(
        z, minBounds.y, maxBounds.y, 0.0f, maxIndex), 0.0f, maxIndex)));

    return world * GetPosition(row, column);
}

const Float2& Terrain::GetMinBounds(int instance) const
{
    return m_minBounds[instance];
}

const Float2& Terrain::GetMaxBounds(int instance) const
{
    return m_maxBounds[instance];
}