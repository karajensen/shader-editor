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

Emitter::Emitter(const boost::property_tree::ptree& node, int shaderID) :
    m_shaderIndex(shaderID)
{
    Resize(GetValue<int>(node, "Amount"));
    m_width = GetValue<float>(node, "Width");
    m_length = GetValue<float>(node, "Length");
    m_lifeTime = GetValue<float>(node, "LifeTime");
    m_lifeFade = GetValue<float>(node, "LifeFade");
    m_maxWaveSpeed = GetValue<float>(node, "MaxWaveSpeed");
    m_minWaveSpeed = GetValue<float>(node, "MinWaveSpeed");
    m_maxAmplitude = GetValue<float>(node, "MaxAmplitude");
    m_minAmplitude = GetValue<float>(node, "MinAmplitude");
    m_maxFrequency = GetValue<float>(node, "MaxFrequency");
    m_minFrequency = GetValue<float>(node, "MinFrequency");
    m_maxSpeed = GetValue<float>(node, "MaxSpeed");
    m_minSpeed = GetValue<float>(node, "MinSpeed");
    m_minSize = GetValue<float>(node, "MinSize");
    m_maxSize = GetValue<float>(node, "MaxSize");
    m_name = GetValue<std::string>(node, "Name");
    m_position.x = GetAttribute<float>(node, "Position", "x");
    m_position.y = GetAttribute<float>(node, "Position", "y");
    m_position.z = GetAttribute<float>(node, "Position", "z");
    m_direction.x = GetAttribute<float>(node, "Direction", "x");
    m_direction.y = GetAttribute<float>(node, "Direction", "y");
    m_direction.z = GetAttribute<float>(node, "Direction", "z");
    m_tint.r = GetAttribute<float>(node, "Tint", "r");
    m_tint.g = GetAttribute<float>(node, "Tint", "g");
    m_tint.b = GetAttribute<float>(node, "Tint", "b");
    m_tint.a = GetAttribute<float>(node, "Tint", "a");

    for (auto child = node.begin(); child != node.end(); ++child)
    {
        if (child->first == "Texture")
        {
            m_textureNames.push_back(child->second.data());
        }
    }
}

void Emitter::Write(boost::property_tree::ptree& node) const
{
    node.add("Name", m_name.c_str());
    node.add("Width", m_width);
    node.add("Length", m_length);
    node.add("Amount", m_particles.size());
    node.add("LifeTime", m_lifeTime);
    node.add("LifeFade", m_lifeFade);
    node.add("MaxWaveSpeed", m_maxWaveSpeed);
    node.add("MinWaveSpeed", m_minWaveSpeed);
    node.add("MaxAmplitude", m_maxAmplitude);
    node.add("MinAmplitude", m_minAmplitude);
    node.add("MaxFrequency", m_maxFrequency);
    node.add("MinFrequency", m_minFrequency);
    node.add("MaxSpeed", m_maxSpeed);
    node.add("MinSpeed", m_minSpeed);
    node.add("MaxSize", m_maxSize);
    node.add("MinSize", m_minSize);
    node.add("Position.<xmlattr>.x", m_position.x);
    node.add("Position.<xmlattr>.y", m_position.y);
    node.add("Position.<xmlattr>.z", m_position.z);
    node.add("Direction.<xmlattr>.x", m_direction.x);
    node.add("Direction.<xmlattr>.y", m_direction.y);
    node.add("Direction.<xmlattr>.z", m_direction.z);
    node.add("Tint.<xmlattr>.r", m_tint.r);
    node.add("Tint.<xmlattr>.g", m_tint.g);
    node.add("Tint.<xmlattr>.b", m_tint.b);
    node.add("Tint.<xmlattr>.a", m_tint.a);

    for (const std::string& texture : m_textureNames)
    {
        node.add("Texture", texture);
    }
}

void Emitter::Write(Cache& cache)
{
    cache.Emitter[EMITTER_LENGTH].SetUpdated(m_length);
    cache.Emitter[EMITTER_WIDTH].SetUpdated(m_width);
    cache.Emitter[EMITTER_POS_X].SetUpdated(m_position.x);
    cache.Emitter[EMITTER_POS_Y].SetUpdated(m_position.y);
    cache.Emitter[EMITTER_POS_Z].SetUpdated(m_position.z);
    cache.Emitter[EMITTER_DIR_X].SetUpdated(m_direction.x);
    cache.Emitter[EMITTER_DIR_Y].SetUpdated(m_direction.y);
    cache.Emitter[EMITTER_DIR_Z].SetUpdated(m_direction.z);
    cache.Emitter[EMITTER_LIFETIME].SetUpdated(m_lifeTime);
    cache.Emitter[EMITTER_LIFEFADE].SetUpdated(m_lifeFade);
    cache.Emitter[EMITTER_TINT_R].SetUpdated(m_tint.r);
    cache.Emitter[EMITTER_TINT_G].SetUpdated(m_tint.g);
    cache.Emitter[EMITTER_TINT_B].SetUpdated(m_tint.b);
    cache.Emitter[EMITTER_MAX_SPEED].SetUpdated(m_maxSpeed);
    cache.Emitter[EMITTER_MIN_SPEED].SetUpdated(m_minSpeed);
    cache.Emitter[EMITTER_MAX_SIZE].SetUpdated(m_maxSize);
    cache.Emitter[EMITTER_MIN_SIZE].SetUpdated(m_minSize);
    cache.Emitter[EMITTER_MAX_FREQ].SetUpdated(m_maxFrequency);
    cache.Emitter[EMITTER_MIN_FREQ].SetUpdated(m_minFrequency);
    cache.Emitter[EMITTER_MAX_AMP].SetUpdated(m_maxAmplitude);
    cache.Emitter[EMITTER_MIN_AMP].SetUpdated(m_minAmplitude);
    cache.Emitter[EMITTER_MAX_WAVE].SetUpdated(m_maxWaveSpeed);
    cache.Emitter[EMITTER_MIN_WAVE].SetUpdated(m_minWaveSpeed);
    cache.ParticleAmount.SetUpdated(static_cast<int>(m_particles.size()));
}

void Emitter::Read(Cache& cache)
{
    m_length = cache.Emitter[EMITTER_LENGTH].Get();
    m_width = cache.Emitter[EMITTER_WIDTH].Get();
    m_position.x = cache.Emitter[EMITTER_POS_X].Get();
    m_position.y = cache.Emitter[EMITTER_POS_Y].Get();
    m_position.z = cache.Emitter[EMITTER_POS_Z].Get();
    m_direction.x = cache.Emitter[EMITTER_DIR_X].Get();
    m_direction.y = cache.Emitter[EMITTER_DIR_Y].Get();
    m_direction.z = cache.Emitter[EMITTER_DIR_Z].Get();
    m_lifeTime = cache.Emitter[EMITTER_LIFETIME].Get();
    m_lifeFade = cache.Emitter[EMITTER_LIFEFADE].Get();
    m_tint.r = cache.Emitter[EMITTER_TINT_R].Get();
    m_tint.g = cache.Emitter[EMITTER_TINT_G].Get();
    m_tint.b = cache.Emitter[EMITTER_TINT_B].Get();
    Resize(cache.ParticleAmount.Get());

    RestrictMinMax(
        cache.Emitter[EMITTER_MIN_SIZE],
        cache.Emitter[EMITTER_MAX_SIZE],
        m_minSize, m_maxSize, true);

    RestrictMinMax(
        cache.Emitter[EMITTER_MIN_SPEED],
        cache.Emitter[EMITTER_MAX_SPEED],
        m_minSpeed, m_maxSpeed);

    RestrictMinMax(
        cache.Emitter[EMITTER_MIN_AMP],
        cache.Emitter[EMITTER_MAX_AMP],
        m_minAmplitude, m_maxAmplitude);

    RestrictMinMax(
        cache.Emitter[EMITTER_MIN_WAVE],
        cache.Emitter[EMITTER_MAX_WAVE],
        m_minWaveSpeed, m_maxWaveSpeed);

    RestrictMinMax(
        cache.Emitter[EMITTER_MIN_FREQ],
        cache.Emitter[EMITTER_MAX_FREQ],
        m_minFrequency, m_maxFrequency);
}

void Emitter::Resize(int size)
{
    const int currentSize = static_cast<int>(m_particles.size());
    if (size < currentSize)
    {
        m_particles.erase(m_particles.begin() + size, m_particles.end());
    }
    else if (size > currentSize)
    {
        m_particles.resize(size);
    }
}

void Emitter::TogglePaused()
{
    m_paused = !m_paused;
}

const std::string& Emitter::Name() const
{
    return m_name;
}

const std::vector<std::string>& Emitter::TextureNames() const
{
    return m_textureNames;
}

const std::vector<int>& Emitter::Textures() const
{
    return m_textures;
}

const std::vector<Particle>& Emitter::Particles() const
{
    return m_particles;
}

int Emitter::ShaderID() const
{
    return m_shaderIndex;
}

const Colour& Emitter::Tint() const
{
    return m_tint;
}

void Emitter::AddTexture(int ID)
{
    m_textures.push_back(ID);
}

bool Emitter::ShouldRender() const
{
    return m_render;
}

bool Emitter::ShouldRender(const Float3& position, 
                           const BoundingArea& bounds)
{
    // Radius requires a buffer as particles can move outside bounds
    const float radius = std::max(m_width, m_length) * m_maxAmplitude * 2.0f;
    const Float3 centerToMesh = m_position - bounds.center;
    return centerToMesh.Length() <= radius + bounds.radius;
}

void Emitter::Tick(float deltatime,
                   const Float3& cameraPosition,
                   const BoundingArea& cameraBounds)
{
    m_render = ShouldRender(cameraPosition, cameraBounds);

    if (!m_render || m_paused)
    {
        return;
    }

    for (Particle& particle : m_particles)
    {
        if (!particle.Tick(deltatime, m_direction))
        {
             Float3 particlePosition(m_position);
             particlePosition.x += Random::Generate(-m_width, m_width) * 0.5f;
             particlePosition.z += Random::Generate(-m_length, m_length) * 0.5f;
    
             const int textureID = m_textures[Random::Generate(
                 0, static_cast<int>(m_textures.size()-1))];
    
             particle.Reset(m_lifeTime, 
                            m_lifeFade,
                            Random::Generate(m_minSpeed, m_maxSpeed),
                            Random::Generate(m_minWaveSpeed, m_maxWaveSpeed),
                            Random::Generate(m_minSize, m_maxSize),
                            Random::Generate(m_minAmplitude, m_maxAmplitude),
                            Random::Generate(m_minFrequency, m_maxFrequency),
                            textureID,
                            particlePosition);
    
        }
    }
}
