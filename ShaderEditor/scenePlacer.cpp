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
    m_data(data),
    m_ocean(*data.water[data.oceanIndex]),
    m_sand(*data.terrain[data.sandIndex]),
    m_rockMinScale(2.5f, 0.75f, 2.5f),
    m_rockMaxScale(4.0f, 1.25f, 4.0f),
    m_meshMinScale(0.75f),
    m_meshMaxScale(2.0f),
    m_rockOffset(1.0f),
    m_minClusters(1),
    m_maxClusters(5)
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
    m_data.lights[m_data.sunIndex]->PositionX(cameraPosition.x);
    m_data.lights[m_data.sunIndex]->PositionZ(cameraPosition.z);

    // Rerrange the patches if the camera has moved out
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

    // Any patches flagged for mesh update do after being rearranged
    for (int r = 0; r < m_patchPerRow; ++r)
    {
        for (int c = 0; c < m_patchPerRow; ++c)
        {
            UpdatePatchMeshes(r, c);
        }
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

void ScenePlacer::UpdatePatch(int row,
                              int column,
                              const Int2& direction)
{
    // Look at one pace in opposite direction
    const int backIndex = Index(row-direction.x, column-direction.y);
    const auto& backInstance = m_ocean.GetInstance(m_patches[backIndex]);
    
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
    // Other meshes rely on this data being updated first
    const int instanceID = m_patches[Index(row, column)];
    m_ocean.SetInstance(instanceID, position, xFlipped, zFlipped);
    m_sand.SetInstance(instanceID, position);
    UpdatePatchData(row, column);
}
                               
bool ScenePlacer::Initialise(const Float3& cameraPosition)
{
    const float halfPatch = m_patchPerRow / 2.0f;

    const float sandSize = m_sand.Size();
    const float waterSize = m_ocean.Size();
    assert(sandSize == waterSize);
    m_patchSize = sandSize;

    const float offset = (halfPatch * m_patchSize) - (m_patchSize / 2.0f);
    const Float2 start(cameraPosition.x - offset, cameraPosition.z - offset);

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

            m_ocean.AddInstance(position, xFlipped, zFlipped);
            m_sand.AddInstance(position);

            if (USE_DIAGNOSTICS)
            {
                const int index = Index(r,c);
                m_sand.Instances()[index].scale *= 0.98f;
            }

            m_patches[instance] = instance;
            ++instance;
        }
    }

    m_patchInside = GetPatchInside(cameraPosition);
    if (m_patchInside.x == NO_INDEX || m_patchInside.y == NO_INDEX)
    {
        Logger::LogError("Did not start inside recognised patch");
    }

    if (GeneratePatchData())
    {
        Logger::LogInfo("Scene: Successfully placed assets");
        return true;
    }
    return false;
}

bool ScenePlacer::GeneratePatchData()
{
    // Reset all the patch data
    for (Patch& patch : m_patchData)
    {
        patch.foliage.clear();
        patch.emitters.clear();
        patch.rock.index = NO_INDEX;
        patch.rock.instance = NO_INDEX;
    }

    // Grab all avaliable emitters to assign
    std::vector<InstanceKey> emitterKeys;
    for (unsigned int i = 0; i < m_data.emitters.size(); ++i)
    {
        const auto count = m_data.emitters[i]->InstanceCount();
        for (unsigned int j = 0; j < count; ++j)
        {
            const auto index = emitterKeys.size();
            emitterKeys.emplace_back();
            emitterKeys[index].index = i;
            emitterKeys[index].instance = j;
        }
    }

    // Ensure too many rocks will not occlude the patches
    if(m_patches.size() < m_data.rocks.size())
    {
        Logger::LogError("Too many rocks created for scene");
        return false;
    }

    // Assign the rocks to the patches
    unsigned int rock = 0;
    const int startingIndex = Index(m_patchInside.x, m_patchInside.y);

    while (rock < m_data.rocks.size())
    {
        const int index = Random::Generate(0, m_patchData.size()-1);
        if (index != startingIndex && m_patchData[index].rock.index == NO_INDEX)
        {
            m_patchData[index].rock = m_data.rocks[rock];
            ++rock;
        }
    }

    // Assign the elements to the patches
    std::random_shuffle(emitterKeys.begin(), emitterKeys.end());
    std::random_shuffle(m_data.foliage.begin(), m_data.foliage.end());

    for (auto& key : m_data.foliage)
    {
        const int index = Random::Generate(0, m_patchData.size()-1);
        m_patchData[index].foliage.push_back(key);
    }

    for (auto& key : emitterKeys)
    {
        const int index = Random::Generate(0, m_patchData.size()-1);
        m_patchData[index].emitters.push_back(key);
    }

    ResetPatches();
    return true;
}

void ScenePlacer::ResetFoliage()
{
    for (int r = 0; r < m_patchPerRow; ++r)
    {
        for (int c = 0; c < m_patchPerRow; ++c)
        {
            const auto instanceID = m_patches[Index(r, c)];
            m_patchData[instanceID].requiresUpdate = true;
            UpdatePatchMeshes(r, c);
        }
    }
}

void ScenePlacer::ResetPatches()
{
    for (int r = 0; r < m_patchPerRow; ++r)
    {
        for (int c = 0; c < m_patchPerRow; ++c)
        {
            UpdatePatchData(r, c);
        }
    }

    for (int r = 0; r < m_patchPerRow; ++r)
    {
        for (int c = 0; c < m_patchPerRow; ++c)
        {
            UpdatePatchMeshes(r, c);
        }
    }
}

void ScenePlacer::UpdatePatchData(int row, int column)
{
    const auto instanceID = m_patches[Index(row, column)];
    auto& data = m_patchData[instanceID];

    data.coordinates.x = row;
    data.coordinates.y = column;
    data.requiresUpdate = true;
    PlaceRock(instanceID);
}

void ScenePlacer::UpdatePatchMeshes(int row, int column)
{
    const auto instanceID = m_patches[Index(row, column)];
    auto& data = m_patchData[instanceID];

    if (data.requiresUpdate)
    {
        data.requiresUpdate = false;
        PlaceFoliage(instanceID);
        PlaceEmitters(instanceID);
    }
}

Float3 ScenePlacer::GetPatchPosition(int instanceID, float x, float z)
{
    using namespace std;
    Float3 position = m_sand.GetAbsolutePosition(instanceID, x, z);

    auto FindRockHeight = [this, &position](int patchID)
    {
        if (patchID >= 0 && patchID < static_cast<int>(m_patches.size()))
        {
            const auto& key = m_patchData[m_patches[patchID]].rock;
            if (key.index != NO_INDEX)
            {
                const auto& rock = m_data.terrain[key.index];
                const auto& minBounds = rock->GetMinBounds(key.instance);
                const auto& maxBounds = rock->GetMaxBounds(key.instance);
                if (position.x >= minBounds.x && position.x <= maxBounds.x && 
                    position.z >= minBounds.y && position.z <= maxBounds.y)
                {
                    const Float3 rockPosition = 
                        rock->GetAbsolutePosition(key.instance, position.x, position.z);

                    if (rockPosition.y > position.y)
                    {
                        position = rockPosition;
                    }
                }
            }
        }
    };

    const auto& data = m_patchData[instanceID];
    const int r = data.coordinates.x;
    const int c = data.coordinates.y;

    // Need to check surrounding patches as rocks can overlap
    FindRockHeight(Index(r, c));
    FindRockHeight(Index(r + 1, c));
    FindRockHeight(Index(r - 1, c));
    FindRockHeight(Index(r, c + 1));
    FindRockHeight(Index(r, c - 1));
    FindRockHeight(Index(r + 1, c + 1));
    FindRockHeight(Index(r + 1, c - 1));
    FindRockHeight(Index(r - 1, c - 1));
    FindRockHeight(Index(r - 1, c + 1));
    
    return position;
}

void ScenePlacer::PlaceFoliage(int instanceID)
{
    auto& patchData = m_patchData[instanceID];
    const Float2& minBounds = m_sand.GetMinBounds(instanceID);
    const Float2& maxBounds = m_sand.GetMaxBounds(instanceID);

    int clusterCounter = 0;
    Float2 clusterCenter;
    Int2 clusterOffset(1, 2);

    for (auto& foliage : patchData.foliage)
    {
        if (clusterCounter <= 0)
        {
            clusterCounter = Random::Generate(m_minClusters, m_maxClusters);
            clusterCenter.x = Random::Generate(minBounds.x, maxBounds.x);
            clusterCenter.y = Random::Generate(minBounds.y, maxBounds.y);
        }

        Float2 position;
        bool withinPatchBounds = false;

        while (!withinPatchBounds)
        {
            const float x = static_cast<float>(Random::Generate(0, 1) == 0 ? -1 : 1);
            const float z = static_cast<float>(Random::Generate(0, 1) == 0 ? -1 : 1);
            const float offset = Random::Generate(clusterOffset.x, clusterOffset.y) * m_sand.Spacing();
            
            position.x = clusterCenter.x + (x * offset);
            position.y = clusterCenter.y + (z * offset);

            withinPatchBounds = position.x > minBounds.x && position.x < maxBounds.x &&
                position.y > minBounds.y && position.y < maxBounds.y;
        }

        const Float3 rotation(0.0f, Random::Generate(0.0f, 360.0f), 0.0f);
        const float scale = Random::Generate(m_meshMinScale, m_meshMaxScale);

        for (auto& key : foliage.GetKeys())
        {
            auto& mesh = *m_data.meshes[key.index];
            mesh.SetInstance(key.instance, 
                GetPatchPosition(instanceID, position.x, position.y), 
                rotation, scale);
        }

        --clusterCounter;
    }
}

void ScenePlacer::PlaceEmitters(int instanceID)
{
    auto& patchData = m_patchData[instanceID];
    const Float2& minBounds = m_sand.GetMinBounds(instanceID);
    const Float2& maxBounds = m_sand.GetMaxBounds(instanceID);

    for (auto& emitter : patchData.emitters)
    {
        const float x = Random::Generate(minBounds.x, maxBounds.x);
        const float z = Random::Generate(minBounds.y, maxBounds.y);
        const Float3 position(GetPatchPosition(instanceID, x, z));
        m_data.emitters[emitter.index]->SetInstance(emitter.instance, position);
    }
}

void ScenePlacer::PlaceRock(int instanceID)
{
    const auto& patchData = m_patchData[instanceID];
    if (patchData.rock.index != NO_INDEX)
    {
        Float3 position = m_sand.GetInstance(instanceID).position;
        position.x += Random::Generate(-m_rockOffset, m_rockOffset);
        position.z += Random::Generate(-m_rockOffset, m_rockOffset);
        const Float3 rotation(0.0f, Random::Generate(0.0f, 360.0f), 0.0f);

        const Float3 scale(
            Random::Generate(m_rockMinScale.x, m_rockMaxScale.x),
            Random::Generate(m_rockMinScale.y, m_rockMaxScale.y),
            Random::Generate(m_rockMinScale.z, m_rockMaxScale.z));

        auto& terrain = m_data.terrain[patchData.rock.index];
        terrain->SetInstance(patchData.rock.instance, position, rotation, scale);
    }
}