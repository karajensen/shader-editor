////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - emitter.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "emitter.h"
#include "common.h"
#include "renderdata.h"
#include "cache.h"

namespace
{
    /**
    * Utility to restrict whether the min/max values are accepted from the cache
    * @param minCached The cache for the minimum attribute
    * @param maxCached The cache for the maximum attribute
    * @param restrictNegative Whether to enforce values should be positive
    * @param minValue The minimum value to set
    * @param maxValue The maximum value to set
    */
    void RestrictMinMax(Lockable<float>& minCached,
                        Lockable<float>& maxCached,
                        float& minValue,
                        float& maxValue,
                        bool restrictNegative = false)
    {
        const float minCachedValue = minCached.Get();
        const float maxCachedValue = maxCached.Get();

        if (minCachedValue >= maxCachedValue ||
            (restrictNegative && minCachedValue < 0.0f) ||
            (restrictNegative && maxCachedValue < 0.0f))
        {
            minCached.SetUpdated(minValue);
            maxCached.SetUpdated(maxValue);
        }
        else
        {
            minValue = minCachedValue;
            maxValue = maxCachedValue;
        }
    }
}

Emitter::Emitter(const std::string& name, int shaderID) :
    m_shaderIndex(shaderID),
    m_name(name)
{
}

void Emitter::Write(Cache& cache)
{
    cache.Emitter[EMITTER_LENGTH].SetUpdated(m_data.length);
    cache.Emitter[EMITTER_WIDTH].SetUpdated(m_data.width);
    cache.Emitter[EMITTER_DIR_X].SetUpdated(m_data.direction.x);
    cache.Emitter[EMITTER_DIR_Y].SetUpdated(m_data.direction.y);
    cache.Emitter[EMITTER_DIR_Z].SetUpdated(m_data.direction.z);
    cache.Emitter[EMITTER_LIFETIME].SetUpdated(m_data.lifeTime);
    cache.Emitter[EMITTER_LIFEFADE].SetUpdated(m_data.lifeFade);
    cache.Emitter[EMITTER_TINT_R].SetUpdated(m_data.tint.r);
    cache.Emitter[EMITTER_TINT_G].SetUpdated(m_data.tint.g);
    cache.Emitter[EMITTER_TINT_B].SetUpdated(m_data.tint.b);
    cache.Emitter[EMITTER_MAX_SPEED].SetUpdated(m_data.maxSpeed);
    cache.Emitter[EMITTER_MIN_SPEED].SetUpdated(m_data.minSpeed);
    cache.Emitter[EMITTER_MAX_SIZE].SetUpdated(m_data.maxSize);
    cache.Emitter[EMITTER_MIN_SIZE].SetUpdated(m_data.minSize);
    cache.Emitter[EMITTER_MAX_FREQ].SetUpdated(m_data.maxFrequency);
    cache.Emitter[EMITTER_MIN_FREQ].SetUpdated(m_data.minFrequency);
    cache.Emitter[EMITTER_MAX_AMP].SetUpdated(m_data.maxAmplitude);
    cache.Emitter[EMITTER_MIN_AMP].SetUpdated(m_data.minAmplitude);
    cache.Emitter[EMITTER_MAX_WAVE].SetUpdated(m_data.maxWaveSpeed);
    cache.Emitter[EMITTER_MIN_WAVE].SetUpdated(m_data.minWaveSpeed);
    cache.Emitter[EMITTER_MAX_WAIT].SetUpdated(m_data.minWaitTime);
    cache.Emitter[EMITTER_MIN_WAIT].SetUpdated(m_data.maxWaitTime);
}

void Emitter::Read(Cache& cache)
{
    m_data.length = cache.Emitter[EMITTER_LENGTH].Get();
    m_data.width = cache.Emitter[EMITTER_WIDTH].Get();
    m_data.direction.x = cache.Emitter[EMITTER_DIR_X].Get();
    m_data.direction.y = cache.Emitter[EMITTER_DIR_Y].Get();
    m_data.direction.z = cache.Emitter[EMITTER_DIR_Z].Get();
    m_data.lifeTime = cache.Emitter[EMITTER_LIFETIME].Get();
    m_data.lifeFade = cache.Emitter[EMITTER_LIFEFADE].Get();
    m_data.tint.r = cache.Emitter[EMITTER_TINT_R].Get();
    m_data.tint.g = cache.Emitter[EMITTER_TINT_G].Get();
    m_data.tint.b = cache.Emitter[EMITTER_TINT_B].Get();

    RestrictMinMax(
        cache.Emitter[EMITTER_MIN_SIZE],
        cache.Emitter[EMITTER_MAX_SIZE],
        m_data.minSize, m_data.maxSize, true);

    RestrictMinMax(
        cache.Emitter[EMITTER_MIN_SPEED],
        cache.Emitter[EMITTER_MAX_SPEED],
        m_data.minSpeed, m_data.maxSpeed);

    RestrictMinMax(
        cache.Emitter[EMITTER_MIN_AMP],
        cache.Emitter[EMITTER_MAX_AMP],
        m_data.minAmplitude, m_data.maxAmplitude);

    RestrictMinMax(
        cache.Emitter[EMITTER_MIN_WAVE],
        cache.Emitter[EMITTER_MAX_WAVE],
        m_data.minWaveSpeed, m_data.maxWaveSpeed);

    RestrictMinMax(
        cache.Emitter[EMITTER_MIN_FREQ],
        cache.Emitter[EMITTER_MAX_FREQ],
        m_data.minFrequency, m_data.maxFrequency);

    RestrictMinMax(
        cache.Emitter[EMITTER_MIN_WAIT],
        cache.Emitter[EMITTER_MAX_WAIT],
        m_data.minWaitTime, m_data.maxWaitTime);
}

bool Emitter::Initialise(const EmitterData& data)
{
    m_data = data;
    m_data.radius = std::max(m_data.length, m_data.width);
    m_instances.resize(data.instances);
    m_totalParticles = data.instances * data.particles;

    for (Instance& instance : m_instances)
    {
        instance.particles.resize(data.particles);
    }

    return true;
}

std::string Emitter::GetRenderedInstances() const
{
    return std::to_string(m_visibleInstances) + " / " +
        std::to_string(m_instances.size());
}

const Emitter::Instance& Emitter::GetInstance(int index) const
{
    return m_instances[index];
}

unsigned int Emitter::InstanceCount() const
{
    return m_instances.size();
}

void Emitter::SetInstance(int index, const Float3& position)
{
    m_instances[index].position = position;
}

void Emitter::TogglePaused()
{
    m_paused = !m_paused;
}

const std::string& Emitter::Name() const
{
    return m_name;
}

const std::vector<int>& Emitter::Textures() const
{
    return m_textures;
}

int Emitter::ShaderID() const
{
    return m_shaderIndex;
}

const Colour& Emitter::Tint() const
{
    return m_data.tint;
}

void Emitter::AddTexture(int ID)
{
    m_textures.push_back(ID);
}

const std::vector<Emitter::Instance>& Emitter::Instances() const
{
    return m_instances;
}

bool Emitter::ShouldRender(const Float3& instancePosition,
                           const BoundingArea& bounds)
{
    // Radius requires a buffer as particles can move outside bounds
    const float radius = std::max(m_data.width, m_data.length) * m_data.maxAmplitude * 2.0f;
    const Float3 centerToMesh = instancePosition - bounds.center;
    return centerToMesh.Length() <= radius + bounds.radius;
}

void Emitter::Tick(float deltatime,
                   const BoundingArea& cameraBounds)
{
    if (m_paused)
    {
        return;
    }

    m_visibleInstances = 0;
    for (Instance& instance : m_instances)
    {
        instance.render = ShouldRender(instance.position, cameraBounds);
        if (instance.render)
        {
            ++m_visibleInstances;

            for (Particle& particle : instance.particles)
            {
                if (!particle.Tick(deltatime, m_data.direction))
                {
                     Float3 particlePosition(instance.position);
                     particlePosition.x += Random::Generate(-m_data.width, m_data.width) * 0.5f;
                     particlePosition.z += Random::Generate(-m_data.length, m_data.length) * 0.5f;
    
                     const int textureID = m_textures[Random::Generate(
                         0, static_cast<int>(m_textures.size()-1))];
    
                     particle.Reset(m_data.lifeTime, 
                                    m_data.lifeFade,
                                    Random::Generate(m_data.minWaitTime, m_data.maxWaitTime),
                                    Random::Generate(m_data.minSpeed, m_data.maxSpeed),
                                    Random::Generate(m_data.minWaveSpeed, m_data.maxWaveSpeed),
                                    Random::Generate(m_data.minSize, m_data.maxSize),
                                    Random::Generate(m_data.minAmplitude, m_data.maxAmplitude),
                                    Random::Generate(m_data.minFrequency, m_data.maxFrequency),
                                    textureID,
                                    particlePosition);
    
                }
            }
        }
    }
}
