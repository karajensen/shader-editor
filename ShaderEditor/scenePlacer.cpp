////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - scenePlacer.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "scenePlacer.h"
#include "sceneData.h"
#include "common.h"

namespace
{
    const bool USE_DIAGNOSTICS = false;
}

ScenePlacer::ScenePlacer(SceneData& data) :
    m_data(data)
{
    const int patchAmount = 36;
    const int minPatchAmount = 9;
    double unused = 0.0;
    if (std::modf(std::sqrt(static_cast<double>(patchAmount)), &unused) != 0.0 ||
        patchAmount < minPatchAmount)
    {
        Logger::LogError("Area patch size unusable");
    }

    m_patchData.resize(patchAmount);
    m_patches.resize(patchAmount);
    m_previous.resize(patchAmount);

    m_patchPerRow = static_cast<int>(
        std::sqrt(static_cast<double>(patchAmount)));
}

ScenePlacer::~ScenePlacer() = default;

int ScenePlacer::Index(int row, int column) const
{
    return row * m_patchPerRow + column;
}

bool ScenePlacer::IsInsidePatch(const Float3& position, int row, int column) const
{
    const float halfSize = m_patchSize * 0.5f;
    const int instanceID = m_patches[Index(row, column)];
    const auto& instance = m_data.water[m_data.oceanIndex]->Instances()[instanceID];
        
    return position.x < instance.position.x + halfSize &&
        position.z < instance.position.z + halfSize &&
        position.x >= instance.position.x - halfSize &&
        position.z >= instance.position.z - halfSize;
}

Int2 ScenePlacer::GetPatchInside(const Float3& position) const
{
    for (int r = 0; r < m_patchPerRow; ++r)
    {
        for (int c = 0; c < m_patchPerRow; ++c)
        {
            if (IsInsidePatch(position, r, c))
            {
                return Int2(r, c);
            }
        }
    }
    return Int2(NO_INDEX, NO_INDEX);
}

void ScenePlacer::Update(const Float3& cameraPosition)
{
    if (m_patchInside.x == NO_INDEX || m_patchInside.y == NO_INDEX)
    {
        m_patchInside = GetPatchInside(cameraPosition);
    }
    else if(!IsInsidePatch(cameraPosition, m_patchInside.x, m_patchInside.y))
    {
        const Int2 direction(m_patchInside - GetPatchInside(cameraPosition));

        // Split the direction as calculations will assume 0,1 or 1,0 
        if (abs(direction.x) > 0)
        {
            ShiftPatches(Int2(direction.x, 0));
        }
        if (abs(direction.y) > 0)
        {
            ShiftPatches(Int2(0, direction.y));
        }

        m_patchInside = GetPatchInside(cameraPosition);
    }
}

void ScenePlacer::ShiftPatches(const Int2& direction)
{
    const int maxIndex = m_patchPerRow - 1;
    m_previous = m_patches;

    if (direction.x > 0)
    {
        // Shift the bottom row to the top
        for (int c = 0; c < m_patchPerRow; ++c)
        {
            m_patches[Index(0, c)] = m_previous[Index(maxIndex, c)];
        }

        // Move all other rows down
        for (int r = 0; r < m_patchPerRow-1; ++r)
        {
            for (int c = 0; c < m_patchPerRow; ++c)
            {
                m_patches[Index(r+1, c)] = m_previous[Index(r, c)];
            }
        }

        // Update the new position of the new top row
        for (int c = 0; c < m_patchPerRow; ++c)
        {
            UpdatePatch(0, c, Int2(-1, 0));
        }
    }
    else if (direction.x < 0)
    {
        // Shift the top row to the bottom
        for (int c = 0; c < m_patchPerRow; ++c)
        {
            m_patches[Index(maxIndex, c)] = m_previous[Index(0, c)];
        }

        // Move all other rows up
        for (int r = maxIndex; r > 0; --r)
        {
            for (int c = 0; c < m_patchPerRow; ++c)
            {
                m_patches[Index(r-1, c)] = m_previous[Index(r, c)];
            }
        }

        // Update the new position of the new top row
        for (int c = 0; c < m_patchPerRow; ++c)
        {
            UpdatePatch(maxIndex, c, Int2(1, 0));
        }
    }
    else if (direction.y > 0)
    {
        // Shift the right row to the left
        for (int r = 0; r < m_patchPerRow; ++r)
        {
            m_patches[Index(r, 0)] = m_previous[Index(r, maxIndex)];
        }

        // Move all other rows right
        for (int r = 0; r < m_patchPerRow; ++r)
        {
            for (int c = 0; c < m_patchPerRow-1; ++c)
            {
                m_patches[Index(r, c+1)] = m_previous[Index(r, c)];
            }
        }

        // Update the new position of the new left row
        for (int r = 0; r < m_patchPerRow; ++r)
        {
            UpdatePatch(r, 0, Int2(0, -1));
        }
    }
    else if (direction.y < 0) 
    {
        // Shift the left row to the right
        for (int r = 0; r < m_patchPerRow; ++r)
        {
            m_patches[Index(r, maxIndex)] = m_previous[Index(r, 0)];
        }

        // Move all other rows left
        for (int r = 0; r < m_patchPerRow; ++r)
        {
            for (int c = maxIndex; c > 0; --c)
            {
                m_patches[Index(r, c-1)] = m_previous[Index(r, c)];
            }
        }

        // Update the new position of the new top row
        for (int r = 0; r < m_patchPerRow; ++r)
        {
            UpdatePatch(r, maxIndex, Int2(0, 1));
        }
    }
}

bool ScenePlacer::IsValid(int index) const
{
    return index >= 0 && index < static_cast<int>(m_patches.size());
}

void ScenePlacer::UpdatePatch(int row,
                               int column,
                               const Int2& direction)
{
    auto& sand = *m_data.terrain[m_data.sandIndex];
    auto& water = *m_data.water[m_data.oceanIndex];

    // Look at one pace in opposite direction
    const int backIndex = Index(row-direction.x, column-direction.y);
    const auto& backInstance = water.GetInstance(m_patches[backIndex]);
    
    const Float2 position(
        backInstance.position.x + (direction.x * m_patchSize), 
        backInstance.position.z + (direction.y * m_patchSize));

    // Determine the tiling of the water so there are no seams
    const bool backFlippedX = backInstance.scale.x < 0;
    const bool backFlippedZ = backInstance.scale.z < 0;
    const bool directionRow = abs(direction.x) > 0;

    bool xFlipped = false;
    bool zFlipped = false;

    if ((directionRow && !backFlippedX && !backFlippedZ) ||
        (!directionRow && backFlippedX && backFlippedZ))
    {
        xFlipped = true;
        zFlipped = false;
    }
    else if ((directionRow && backFlippedX && backFlippedZ) ||
             (!directionRow && !backFlippedX && !backFlippedZ))
    {
        xFlipped = false;
        zFlipped = true;
    }
    else if ((directionRow && !backFlippedX && backFlippedZ) ||
             (!directionRow && backFlippedX && !backFlippedZ))
    {
        xFlipped = true;
        zFlipped = true;
    }

    // Update the patch instance with the new values
    const int index = Index(row, column);
    water.SetInstance(m_patches[index], position, xFlipped, zFlipped);
    sand.SetInstance(m_patches[index], position);

    // Update any foliage attached to this patch
    PlaceFoliage(index);
}
                               
bool ScenePlacer::Initialise(const Float3& camera)
{
    const float halfPatch = m_patchPerRow / 2.0f;

    assert(!m_data.water.empty());
    auto& sand = *m_data.terrain[m_data.sandIndex];
    auto& water = *m_data.water[m_data.oceanIndex];

    const float sandSize = sand.Size();
    const float waterSize = water.Size();
    assert(sandSize == waterSize);
    m_patchSize = sandSize;

    const float offset = (halfPatch * m_patchSize) - (m_patchSize / 2.0f);
    const Float2 start(camera.x - offset, camera.z - offset);

    int instance = 0;
    Float2 position;

    // Create the tiling water/sand instances
    for (int r = 0; r < m_patchPerRow; ++r)
    {
        for (int c = 0; c < m_patchPerRow; ++c)
        {
            const bool xFlipped = r % 2 == 0;
            const bool zFlipped = c % 2 == 0;

            position.x = start.x + (r * m_patchSize);
            position.y = start.y + (c * m_patchSize);

            water.AddInstance(position, xFlipped, zFlipped);
            sand.AddInstance(position);

            if (USE_DIAGNOSTICS)
            {
                const int index = Index(r,c);
                sand.Instances()[index].scale *= 0.98f;
            }

            m_patches[instance] = instance;
            ++instance;
        }
    }

    // Place meshes over the patches
    GenerateFoliage();

    m_patchInside = GetPatchInside(camera);
    if (m_patchInside.x == NO_INDEX || m_patchInside.y == NO_INDEX)
    {
        Logger::LogError("Did not start inside recognised patch");
    }

    return true;   
}

void ScenePlacer::GenerateFoliage()
{
    // Reset all the patch data
    for (Patch& patch : m_patchData)
    {
        patch.foliage.clear();
        patch.rocks.clear();
    }

    // Create all the instances for the foliage meshes
    std::vector<MeshKey> meshKeys;
    for (auto& foliage : m_data.foliage)
    {
        unsigned int meshID = foliage.first;
        const int instances = foliage.second;

        auto& mesh = *m_data.meshes[meshID];

        const int amount = Random::Generate(std::max(0, 
            instances - m_countRandom), instances + m_countRandom);

        mesh.ClearInstances();
        mesh.AddInstances(amount);

        for (unsigned int i = 0; i < mesh.Instances().size(); ++i)
        {
            const auto index = meshKeys.size();
            meshKeys.emplace_back();
            meshKeys[index].index = meshID;
            meshKeys[index].instance = i;
        }
    }

    std::random_shuffle(meshKeys.begin(), meshKeys.end());

    // Assign the foliage to the patches
    for (MeshKey& key : meshKeys)
    {
        const int index = Random::Generate(0, m_patchData.size()-1);
        m_patchData[index].foliage.push_back(key);
    }

    // Place the foliage on the rocks
    for (int ID : m_patches)
    {
        PlaceFoliage(ID);
    }
}

void ScenePlacer::PlaceFoliage(int ID)
{
    const int patchID = m_patches[ID];
    auto& patchData = m_patchData[patchID];

    const auto& sand = *m_data.terrain[m_data.sandIndex];
    const auto& sandInstance = sand.GetInstance(patchID);

    const auto& center = sandInstance.position;
    const float halfSize = m_patchSize * 0.5f;
    const Float2 minBounds(center.x - halfSize, center.z - halfSize);
    const Float2 maxBounds(center.x + halfSize, center.z + halfSize);

    const float minRotation = 0.0f;
    const float maxRotation = DegToRad(360.0f);
    const float minScale = 5.0f;
    const float maxScale = 10.0f;

    // Determines the average height at the given point
    auto GetHeight = [&](float x, float z) -> float
    {
        const float localX = ConvertRange(x, minBounds.x, maxBounds.x, 0.0f, m_patchSize);
        const float localZ = ConvertRange(z, minBounds.y, maxBounds.y, 0.0f, m_patchSize);
        return -20.0f; //Temporary
    };

    for (MeshKey& foliage : patchData.foliage)
    {
        const float x = Random::Generate(minBounds.x, maxBounds.x);
        const float z = Random::Generate(minBounds.y, maxBounds.y);
        const Float3 position(x, GetHeight(x, z), z);
        const Float3 rotation(0.0f, Random::Generate(minRotation, maxRotation), 0.0f);
        const float scale = Random::Generate(minScale, maxScale);

        auto& mesh = *m_data.meshes[foliage.index];
        mesh.SetInstance(foliage.instance, position, rotation, scale);
    }
}