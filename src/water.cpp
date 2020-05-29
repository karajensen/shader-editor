////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - water.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "water.h"
#include "cache.h"

Water::Water(const std::string& name, 
             const std::string& shaderName, 
             int shaderID)
    : Grid(name, shaderName, shaderID)
    , m_speed(1.0f)
    , m_bump(1.0f)
    , m_bumpScale(4.0f, 6.0f)
    , m_uvScale(1.0f, 1.0f)
    , m_fresnal(1.0f, 0.5f, 2.0f)
    , m_shallowColour(1.0f, 1.0f, 1.0f, 0.5f)
    , m_deepColour(0.8f, 0.8f, 0.8f, 1.0f)
    , m_reflectionTint(1.0f, 1.0f, 1.0f, 1.0f)
    , m_reflection(1.0f)
{
    BackfaceCull(false);
    m_waves.resize(Wave::MAX);

    m_waves[0].amplitude = 1.0f;
    m_waves[0].frequency = 1.0f;
    m_waves[0].phase = 1.0f;
    m_waves[0].directionX = -1.0f;
    m_waves[0].directionZ = 0.0f;

    m_waves[1].amplitude = 0.5f;
    m_waves[1].frequency = 1.0f;
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
    cache.Water[Tweakable::Water::Bump].SetUpdated(m_bump);
    cache.Water[Tweakable::Water::BumpScaleX].SetUpdated(m_bumpScale.x);
    cache.Water[Tweakable::Water::BumpScaleY].SetUpdated(m_bumpScale.y);
    cache.Water[Tweakable::Water::Speed].SetUpdated(m_speed);
    cache.Water[Tweakable::Water::DeepR].SetUpdated(m_deepColour.r);
    cache.Water[Tweakable::Water::DeepG].SetUpdated(m_deepColour.g);
    cache.Water[Tweakable::Water::DeepB].SetUpdated(m_deepColour.b);
    cache.Water[Tweakable::Water::DeepA].SetUpdated(m_deepColour.a);
    cache.Water[Tweakable::Water::ShallowR].SetUpdated(m_shallowColour.r);
    cache.Water[Tweakable::Water::ShallowG].SetUpdated(m_shallowColour.g);
    cache.Water[Tweakable::Water::ShallowB].SetUpdated(m_shallowColour.b);
    cache.Water[Tweakable::Water::ShallowA].SetUpdated(m_shallowColour.a);
    cache.Water[Tweakable::Water::ReflectionR].SetUpdated(m_reflectionTint.r);
    cache.Water[Tweakable::Water::ReflectionG].SetUpdated(m_reflectionTint.g);
    cache.Water[Tweakable::Water::ReflectionB].SetUpdated(m_reflectionTint.b);
    cache.Water[Tweakable::Water::Reflection].SetUpdated(m_reflection);
    cache.Water[Tweakable::Water::FresnalScale].SetUpdated(m_fresnal.x);
    cache.Water[Tweakable::Water::FresnalBias].SetUpdated(m_fresnal.y);
    cache.Water[Tweakable::Water::FresnalPower].SetUpdated(m_fresnal.z);
    cache.Water[Tweakable::Water::ScaleU].SetUpdated(m_uvScale.x);
    cache.Water[Tweakable::Water::ScaleV].SetUpdated(m_uvScale.y);
    cache.WaveAmount.SetUpdated(static_cast<int>(m_waves.size()));
}

void Water::Read(Cache& cache)
{
    m_bump = cache.Water[Tweakable::Water::Bump].Get();
    m_bumpScale.x = cache.Water[Tweakable::Water::BumpScaleX].Get();
    m_bumpScale.y = cache.Water[Tweakable::Water::BumpScaleY].Get();
    m_speed = cache.Water[Tweakable::Water::Speed].Get();
    m_deepColour.r = cache.Water[Tweakable::Water::DeepR].Get();
    m_deepColour.g = cache.Water[Tweakable::Water::DeepG].Get();
    m_deepColour.b = cache.Water[Tweakable::Water::DeepB].Get();
    m_deepColour.a = cache.Water[Tweakable::Water::DeepA].Get();
    m_shallowColour.r = cache.Water[Tweakable::Water::ShallowR].Get();
    m_shallowColour.g = cache.Water[Tweakable::Water::ShallowG].Get();
    m_shallowColour.b = cache.Water[Tweakable::Water::ShallowB].Get();
    m_shallowColour.a = cache.Water[Tweakable::Water::ShallowA].Get();
    m_reflectionTint.r = cache.Water[Tweakable::Water::ReflectionR].Get();
    m_reflectionTint.g = cache.Water[Tweakable::Water::ReflectionG].Get();
    m_reflectionTint.b = cache.Water[Tweakable::Water::ReflectionB].Get();
    m_reflection = cache.Water[Tweakable::Water::Reflection].Get();
    m_fresnal.x = cache.Water[Tweakable::Water::FresnalScale].Get();
    m_fresnal.y = cache.Water[Tweakable::Water::FresnalBias].Get();
    m_fresnal.z = cache.Water[Tweakable::Water::FresnalPower].Get();
    m_uvScale.x = cache.Water[Tweakable::Water::ScaleU].Get();
    m_uvScale.y = cache.Water[Tweakable::Water::ScaleV].Get();
}

void Water::ReadWave(Cache& cache, int index)
{
    Wave& wave = m_waves.at(index);
    wave.amplitude = cache.Wave[Tweakable::Wave::Amplitude].Get();
    wave.frequency = cache.Wave[Tweakable::Wave::Frequency].Get();
    wave.phase = cache.Wave[Tweakable::Wave::Phase].Get();
    wave.directionX = cache.Wave[Tweakable::Wave::DirectionX].Get();
    wave.directionZ = cache.Wave[Tweakable::Wave::DirectionZ].Get();
}

void Water::WriteWave(Cache& cache, int index)
{
    Wave& wave = m_waves.at(index);
    cache.Wave[Tweakable::Wave::Amplitude].SetUpdated(wave.amplitude);
    cache.Wave[Tweakable::Wave::Frequency].SetUpdated(wave.frequency);
    cache.Wave[Tweakable::Wave::Phase].SetUpdated(wave.phase);
    cache.Wave[Tweakable::Wave::DirectionX].SetUpdated(wave.directionX);
    cache.Wave[Tweakable::Wave::DirectionZ].SetUpdated(wave.directionZ);
}

const std::vector<Water::Wave>& Water::Waves() const
{
    return m_waves;
}

const Float2& Water::BumpScale() const
{
    return m_bumpScale;
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