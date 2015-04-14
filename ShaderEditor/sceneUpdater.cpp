////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - sceneUpdater.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "sceneUpdater.h"
#include "sceneData.h"

namespace
{
    const int WATER_ID = 0;
    const bool USE_DIAGNOSTICS = false;
}

SceneUpdater::SceneUpdater(SceneData& data) :
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

    m_patches.resize(patchAmount);
    m_previous.resize(patchAmount);
    m_patchPerRow = static_cast<int>(
        std::sqrt(static_cast<double>(patchAmount)));
}

SceneUpdater::~SceneUpdater() = default;

int SceneUpdater::Index(int row, int column) const
{
    return row * m_patchPerRow + column;
}

bool SceneUpdater::IsInsidePatch(const Float3& position, int row, int column) const
{
    const float halfSize = m_patchSize * 0.5f;
    const int instanceID = m_patches[Index(row, column)];
    const auto& instance = m_data.water[WATER_ID]->Instances()[instanceID];
        
    return position.x < instance.position.x + halfSize &&
        position.z < instance.position.z + halfSize &&
        position.x >= instance.position.x - halfSize &&
        position.z >= instance.position.z - halfSize;
}

Int2 SceneUpdater::GetPatchInside(const Float3& position) const
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

void SceneUpdater::Update(const Float3& camera)
{
    if (m_patchInside.x == NO_INDEX || m_patchInside.y == NO_INDEX)
    {
        m_patchInside = GetPatchInside(camera);
    }
    else if(!IsInsidePatch(camera, m_patchInside.x, m_patchInside.y))
    {
        const Int2 direction(m_patchInside - GetPatchInside(camera));

        // Split the direction as calculations will assume 0,1 or 1,0 
        if (abs(direction.x) > 0)
        {
            ShiftPatches(Int2(direction.x, 0));
        }
        if (abs(direction.y) > 0)
        {
            ShiftPatches(Int2(0, direction.y));
        }

        m_patchInside = GetPatchInside(camera);
    }
}

void SceneUpdater::ShiftPatches(const Int2& direction)
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

bool SceneUpdater::IsValid(int index) const
{
    return index >= 0 && index < static_cast<int>(m_patches.size());
}

void SceneUpdater::UpdatePatch(int row,
                               int column,
                               const Int2& direction)
{
    auto& sand = *m_data.terrain[m_data.sandIndex];
    auto& water = *m_data.water[WATER_ID];

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
}
                               
bool SceneUpdater::Initialise(const Float3& camera)
{
    const float halfPatch = m_patchPerRow / 2.0f;

    assert(!m_data.water.empty());
    auto& sand = *m_data.terrain[m_data.sandIndex];
    auto& water = *m_data.water[WATER_ID];

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

    // Fill the patches so the current patch is not occluded by terrain
    m_patchInside = GetPatchInside(camera);
    if (m_patchInside.x == NO_INDEX || m_patchInside.y == NO_INDEX)
    {
        Logger::LogError("Did not start inside recognised patch");
    }

    return true;   
}