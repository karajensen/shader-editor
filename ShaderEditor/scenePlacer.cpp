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
    m_sand(*data.terrain[data.sandIndex])
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
            UpdatePatch(0, c, Int2(-1.0, 0.0));
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
            UpdatePatch(maxIndex, c, Int2(1.0, 0.0));
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
            UpdatePatch(r, 0, Int2(0.0, -1.0));
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
            UpdatePatch(r, maxIndex, Int2(0.0, 1.0));
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
    const int index = Index(row, column);
    const int instance = m_patches[index];
    m_ocean.SetInstance(instance, position, xFlipped, zFlipped);
    m_sand.SetInstance(instance, position);
    UpdatePatchData(instance);
}
                               
bool ScenePlacer::Initialise(const Float3& camera)
{
    const float halfPatch = m_patchPerRow / 2.0f;

    const float sandSize = m_sand.Size();
    const float waterSize = m_ocean.Size();
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

    // Place meshes over the patches
    GeneratePatchData();

    m_patchInside = GetPatchInside(camera);
    if (m_patchInside.x == NO_INDEX || m_patchInside.y == NO_INDEX)
    {
        Logger::LogError("Did not start inside recognised patch");
    }

    return true;   
}

void ScenePlacer::GeneratePatchData()
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
    ResetPatches();
}

void ScenePlacer::ResetPatches()
{
    for (int instanceID : m_patches)
    {
        UpdatePatchData(instanceID);
    }
}

void ScenePlacer::UpdatePatchData(int instanceID)
{
    auto& data = m_patchData[instanceID];
    const auto& instance = m_sand.GetInstance(instanceID);

    const float halfSize = m_patchSize * 0.5f;
    data.minBounds.x = instance.position.x - halfSize;
    data.minBounds.y = instance.position.z - halfSize;
    data.maxBounds.x = instance.position.x + halfSize;
    data.maxBounds.y = instance.position.z + halfSize; 

    PlaceFoliage(instanceID);
}

float ScenePlacer::GetPatchHeight(int instanceID, float x, float z) const
{
    const auto& data = m_patchData[instanceID];
    const float maxIndex = m_sand.Rows() - 1.0f;

    const float row = Clamp(ConvertRange(x, data.minBounds.x, 
        data.maxBounds.x, 0.0f, maxIndex), 0.0f, maxIndex);

    const float column = Clamp(ConvertRange(z, data.minBounds.y, 
        data.maxBounds.y, 0.0f, maxIndex), 0.0f, maxIndex);
    
    const float localHeight = m_sand.GetHeight(
        static_cast<int>(std::round(row)), 
        static_cast<int>(std::round(column)));

    return m_sand.GetInstance(instanceID).position.y + localHeight;
}

void ScenePlacer::PlaceFoliage(int instanceID)
{
    auto& patchData = m_patchData[instanceID];
    const Float2& minBounds = patchData.minBounds;
    const Float2& maxBounds = patchData.maxBounds;

    const float minRotation = 0.0f;
    const float maxRotation = DegToRad(360.0f);
    const float minScale = 5.0f;
    const float maxScale = 10.0f;

    for (MeshKey& foliage : patchData.foliage)
    {
        const float x = Random::Generate(minBounds.x, maxBounds.x);
        const float z = Random::Generate(minBounds.y, maxBounds.y);
        const Float3 position(x, GetPatchHeight(instanceID, x, z), z);
        const Float3 rotation(0.0f, Random::Generate(minRotation, maxRotation), 0.0f);
        const float scale = Random::Generate(minScale, maxScale);

        auto& mesh = *m_data.meshes[foliage.index];
        mesh.SetInstance(foliage.instance, position, rotation, scale);
    }
}