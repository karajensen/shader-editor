////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - emitter.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "emitter.h"
#include "common.h"

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

void Emitter::Tick(float deltatime)
{
    if (m_paused)
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
