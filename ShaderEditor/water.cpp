////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - water.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "water.h"
#include "cache.h"

Water::Water(const boost::property_tree::ptree& node) :
    MeshData(node)
{
    m_bump = GetValueOptional<float>(node, 0.0f, "Bump");
    m_bumpVelocity.x = GetAttribute<float>(node, "BumpVelocity", "x");
    m_bumpVelocity.y = GetAttribute<float>(node, "BumpVelocity", "y");
    m_fresnal.x = GetAttribute<float>(node, "Fresnal", "scale");
    m_fresnal.y = GetAttribute<float>(node, "Fresnal", "bias");
    m_fresnal.z = GetAttribute<float>(node, "Fresnal", "power");
    m_reflection = GetValue<float>(node, "ReflectionIntensity");
    m_speed = GetValue<float>(node, "Speed");
    m_uvScale.x = GetAttribute<float>(node, "UVScale", "u");
    m_uvScale.y = GetAttribute<float>(node, "UVScale", "v");
    m_deepColour.r = GetAttribute<float>(node, "DeepColour", "r");
    m_deepColour.g = GetAttribute<float>(node, "DeepColour", "g");
    m_deepColour.b = GetAttribute<float>(node, "DeepColour", "b");
    m_deepColour.a = GetAttribute<float>(node, "DeepColour", "a");
    m_shallowColour.r = GetAttribute<float>(node, "ShallowColour", "r");
    m_shallowColour.g = GetAttribute<float>(node, "ShallowColour", "g");
    m_shallowColour.b = GetAttribute<float>(node, "ShallowColour", "b");
    m_shallowColour.a = GetAttribute<float>(node, "ShallowColour", "a");
    m_reflectionTint.r = GetAttribute<float>(node, "ReflectionTint", "r");
    m_reflectionTint.g = GetAttribute<float>(node, "ReflectionTint", "g");
    m_reflectionTint.b = GetAttribute<float>(node, "ReflectionTint", "b");    

    for (auto itr = node.begin(); itr != node.end(); ++itr)
    {
        if (itr->first == "Wave")
        {
            m_waves.emplace_back();
            Wave& wave = m_waves[m_waves.size()-1];

            const boost::property_tree::ptree& child = itr->second;
            wave.amplitude = GetValue<float>(child, "Amplitude");
            wave.frequency = GetValue<float>(child, "Frequency");
            wave.phase = GetValue<float>(child, "Phase");
            wave.directionX = GetAttribute<float>(child, "Direction", "x");
            wave.directionZ = GetAttribute<float>(child, "Direction", "z");
        }
    }

    // Currently don't support dynamic arrays in shaders
    if (m_waves.size() != GetMaxWaves())
    {
        Logger::LogError("Water: " + Name() + 
            " Did not have required amount of waves");
    }
}

void Water::Write(boost::property_tree::ptree& node,
                  std::function<boost::property_tree::ptree&()> createNode) const
{
    MeshData::Write(node);

    AddValueOptional(node, "Bump", m_bump, 0.0f);
    node.add("Speed", m_speed);
    node.add("BumpVelocity.<xmlattr>.x", m_bumpVelocity.x);
    node.add("BumpVelocity.<xmlattr>.y", m_bumpVelocity.y);
    node.add("Fresnal.<xmlattr>.scale", m_fresnal.x);
    node.add("Fresnal.<xmlattr>.bias", m_fresnal.y);
    node.add("Fresnal.<xmlattr>.power", m_fresnal.z);
    node.add("ReflectionIntensity", m_reflection);
    node.add("UVScale.<xmlattr>.u", m_uvScale.x);
    node.add("UVScale.<xmlattr>.v", m_uvScale.y);
    node.add("DeepColour.<xmlattr>.r", m_deepColour.r);
    node.add("DeepColour.<xmlattr>.g", m_deepColour.g);
    node.add("DeepColour.<xmlattr>.b", m_deepColour.b);
    node.add("DeepColour.<xmlattr>.a", m_deepColour.a);
    node.add("ShallowColour.<xmlattr>.r", m_shallowColour.r);
    node.add("ShallowColour.<xmlattr>.g", m_shallowColour.g);
    node.add("ShallowColour.<xmlattr>.b", m_shallowColour.b);
    node.add("ShallowColour.<xmlattr>.a", m_shallowColour.a);
    node.add("ReflectionTint.<xmlattr>.r", m_reflectionTint.r);
    node.add("ReflectionTint.<xmlattr>.g", m_reflectionTint.g);
    node.add("ReflectionTint.<xmlattr>.b", m_reflectionTint.b);

    for (const Wave& wave : m_waves)
    {
        boost::property_tree::ptree& wavenode = createNode();
        wavenode.add("Frequency", wave.frequency);
        wavenode.add("Amplitude", wave.amplitude);
        wavenode.add("Phase", wave.phase);
        wavenode.add("Direction.<xmlattr>.x", wave.directionX);
        wavenode.add("Direction.<xmlattr>.z", wave.directionZ);
        node.add_child("Wave", wavenode);
    }
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

int Water::GetMaxWaves()
{
    return 2;
}