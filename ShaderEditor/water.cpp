////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - water.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "water.h"
#include "cache.h"

Water::Water(const std::string& name, 
             const std::string& shaderName, 
             int shaderID) :

    Grid(name, shaderName, shaderID),
    m_speed(1.0f),
    m_bump(1.0f),
    m_bumpVelocity(0.0f, 0.0f),
    m_uvScale(1.0f, 1.0f),
    m_fresnal(1.0f, 0.5f, 2.0f),
    m_shallowColour(1.0f, 1.0f, 1.0f, 0.5f),
    m_deepColour(0.8f, 0.8f, 0.8f, 1.0f),
    m_reflectionTint(1.0f, 1.0f, 1.0f, 1.0f),
    m_reflection(1.0f)
{
    BackfaceCull(false);
    m_waves.resize(Wave::MAX);

    m_waves[0].amplitude = 1.0f;
    m_waves[0].frequency = 1.0f;
    m_waves[0].phase = 1.0f;
    m_waves[0].directionX = -1.0f;
    m_waves[0].directionZ = 0.0f;

    m_waves[1].amplitude = 0.5f;
    m_waves[1].frequency = 2.0f;
    m_waves[1].phase = 1.0f;
    m_waves[1].directionX = -0.7f;
    m_waves[1].directionZ = 0.7f;
}
bool Water::Initialise(float height, float spacing, int size)
{
    m_height = height;

    if (CreateGrid(Float2(1.0f, 1.0f), spacing, size, size, false, false))
    {
        InitialiseMeshData();
        return true;
    }
    return false;
}

void Water::Write(Cache& cache)
{
    cache.Water[WATER_BUMP].SetUpdated(m_bump);
    cache.Water[WATER_BUMP_VELOCITY_X].SetUpdated(m_bumpVelocity.x);
    cache.Water[WATER_BUMP_VELOCITY_Y].SetUpdated(m_bumpVelocity.y);
    cache.Water[WATER_SPEED].SetUpdated(m_speed);
    cache.Water[WATER_DEEP_R].SetUpdated(m_deepColour.r);
    cache.Water[WATER_DEEP_G].SetUpdated(m_deepColour.g);
    cache.Water[WATER_DEEP_B].SetUpdated(m_deepColour.b);
    cache.Water[WATER_DEEP_A].SetUpdated(m_deepColour.a);
    cache.Water[WATER_SHALLOW_R].SetUpdated(m_shallowColour.r);
    cache.Water[WATER_SHALLOW_G].SetUpdated(m_shallowColour.g);
    cache.Water[WATER_SHALLOW_B].SetUpdated(m_shallowColour.b);
    cache.Water[WATER_SHALLOW_A].SetUpdated(m_shallowColour.a);
    cache.Water[WATER_REFLECTION_R].SetUpdated(m_reflectionTint.r);
    cache.Water[WATER_REFLECTION_G].SetUpdated(m_reflectionTint.g);
    cache.Water[WATER_REFLECTION_B].SetUpdated(m_reflectionTint.b);
    cache.Water[WATER_REFLECTION].SetUpdated(m_reflection);
    cache.Water[WATER_FRESNAL_SCALE].SetUpdated(m_fresnal.x);
    cache.Water[WATER_FRESNAL_BIAS].SetUpdated(m_fresnal.y);
    cache.Water[WATER_FRESNAL_POWER].SetUpdated(m_fresnal.z);
    cache.Water[WATER_SCALE_U].SetUpdated(m_uvScale.x);
    cache.Water[WATER_SCALE_V].SetUpdated(m_uvScale.y);
    cache.WaveAmount.SetUpdated(static_cast<int>(m_waves.size()));
}

void Water::Read(Cache& cache)
{
    m_bump = cache.Water[WATER_BUMP].Get();
    m_bumpVelocity.x = cache.Water[WATER_BUMP_VELOCITY_X].Get();
    m_bumpVelocity.y = cache.Water[WATER_BUMP_VELOCITY_Y].Get();
    m_speed = cache.Water[WATER_SPEED].Get();
    m_deepColour.r = cache.Water[WATER_DEEP_R].Get();
    m_deepColour.g = cache.Water[WATER_DEEP_G].Get();
    m_deepColour.b = cache.Water[WATER_DEEP_B].Get();
    m_deepColour.a = cache.Water[WATER_DEEP_A].Get();
    m_shallowColour.r = cache.Water[WATER_SHALLOW_R].Get();
    m_shallowColour.g = cache.Water[WATER_SHALLOW_G].Get();
    m_shallowColour.b = cache.Water[WATER_SHALLOW_B].Get();
    m_shallowColour.a = cache.Water[WATER_SHALLOW_A].Get();
    m_reflectionTint.r = cache.Water[WATER_REFLECTION_R].Get();
    m_reflectionTint.g = cache.Water[WATER_REFLECTION_G].Get();
    m_reflectionTint.b = cache.Water[WATER_REFLECTION_B].Get();
    m_reflection = cache.Water[WATER_REFLECTION].Get();
    m_fresnal.x = cache.Water[WATER_FRESNAL_SCALE].Get();
    m_fresnal.y = cache.Water[WATER_FRESNAL_BIAS].Get();
    m_fresnal.z = cache.Water[WATER_FRESNAL_POWER].Get();
    m_uvScale.x = cache.Water[WATER_SCALE_U].Get();
    m_uvScale.y = cache.Water[WATER_SCALE_V].Get();
}

void Water::ReadWave(Cache& cache, int index)
{
    Wave& wave = m_waves.at(index);
    wave.amplitude = cache.Wave[WAVE_AMPLITUDE].Get();
    wave.frequency = cache.Wave[WAVE_FREQUENCY].Get();
    wave.phase = cache.Wave[WAVE_PHASE].Get();
    wave.directionX = cache.Wave[WAVE_DIRECTION_X].Get();
    wave.directionZ = cache.Wave[WAVE_DIRECTION_Z].Get();
}

void Water::WriteWave(Cache& cache, int index)
{
    Wave& wave = m_waves.at(index);
    cache.Wave[WAVE_AMPLITUDE].SetUpdated(wave.amplitude);
    cache.Wave[WAVE_FREQUENCY].SetUpdated(wave.frequency);
    cache.Wave[WAVE_PHASE].SetUpdated(wave.phase);
    cache.Wave[WAVE_DIRECTION_X].SetUpdated(wave.directionX);
    cache.Wave[WAVE_DIRECTION_Z].SetUpdated(wave.directionZ);
}

const std::vector<Water::Wave>& Water::Waves() const
{
    return m_waves;
}

const Float2& Water::BumpVelocity() const
{
    return m_bumpVelocity;
}

const float& Water::Speed() const
{
    return m_speed;
}

const Float2& Water::UVScale() const
{
    return m_uvScale;
}

const Float3& Water::Fresnal() const
{
    return m_fresnal;
}

const Colour& Water::Shallow() const
{
    return m_shallowColour;
}

const Colour& Water::Deep() const
{
    return m_deepColour;
}

const Colour& Water::ReflectionTint() const
{
    return m_reflectionTint;
}

const float& Water::Bump() const
{
    return m_bump;
}

const float& Water::ReflectionIntensity() const
{
    return m_reflection;
}

void Water::SetInstance(int index, const Float2& position, bool flippedX, bool flippedZ)
{
    m_instances[index].position.x = position.x;
    m_instances[index].position.y = m_height;
    m_instances[index].position.z = position.y;
    m_instances[index].scale.x = flippedX ? -1.0f : 1.0f;
    m_instances[index].scale.z = flippedZ ? -1.0f : 1.0f;
    m_instances[index].requiresUpdate = true;
}

void Water::AddInstance(const Float2& position, bool flippedX, bool flippedZ)
{
    Water::AddInstances(1);
    SetInstance(static_cast<int>(m_instances.size()-1), position, flippedX, flippedZ);
}