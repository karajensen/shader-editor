////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - sceneUpdater.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "sceneUpdater.h"
#include "sceneData.h"

SceneUpdater::SceneUpdater(SceneData& data) :
    m_data(data)
{
    const int patchAmount = 9;
    double unused = 0.0;
    if (std::modf(std::sqrt(static_cast<double>(patchAmount)), &unused) != 0.0)
    {
        Logger::LogError("Area patch size must be a cube");
    }

    m_patches.resize(patchAmount);
    m_patchPerRow = static_cast<int>(
        std::sqrt(static_cast<double>(patchAmount)));
}

SceneUpdater::~SceneUpdater() = default;

int SceneUpdater::Index(int row, int column)
{
    return row * m_patchPerRow + column;
}

void SceneUpdater::Initialise(const Float3& camera)
{
    const float halfPatch = m_patchPerRow / 2.0f;

    assert(!m_data.water.empty());
    auto& sand = *m_data.terrain[m_data.sandIndex];
    auto& water = *m_data.water[0];

    m_sandOffset = sand.Size();
    const float sandOffset = (halfPatch * m_sandOffset) - (m_sandOffset / 2.0f);
    const Float2 sandStart(camera.x - sandOffset, camera.z - sandOffset);

    m_waterOffset = water.Size();
    const float waterOffset = (halfPatch * m_waterOffset) - (m_waterOffset / 2.0f);
    const Float2 waterStart(camera.x - waterOffset, camera.z - waterOffset);

    int instance = 0;
    Float2 waterPosition, sandPosition;

    for (int r = 0; r < m_patchPerRow; ++r)
    {
        for (int c = 0; c < m_patchPerRow; ++c)
        {
            const int index = Index(r, c);
            const bool xFlipped = r % 2 == 0;
            const bool zFlipped = c % 2 == 0;

            waterPosition.x = waterStart.x + (r * m_waterOffset);
            waterPosition.y = waterStart.y + (c * m_waterOffset);
            water.AddInstance(waterPosition, xFlipped, zFlipped);

            sandPosition.x = sandStart.x + (r * m_sandOffset);
            sandPosition.y = sandStart.y + (c * m_sandOffset);            
            sand.AddInstance(sandPosition);

            m_patches[instance] = instance;
            ++instance;
        }
    }
}