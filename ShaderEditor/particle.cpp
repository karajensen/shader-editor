////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - particle.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "particle.h"
#include "common.h"

Particle::Particle() :
    m_maxWaitTime(Random::Generate(0.0f,1.0f))
{
}

void Particle::Reset(float lifeTime,
                     float lifeFade,
                     float speed,
                     float waveSpeed,
                     float size,
                     float amplitude,
                     float frequency,
                     int texture,
                     const Float3& position)
{
    m_maxLifeTime = lifeTime;
    m_lifeFade = lifeFade;
    m_alpha = 0.0f;
    m_waitTime = 0.0f;
    m_lifeTime = 0.0f;
    m_alive = true;
    m_speed = speed;
    m_size = size;
    m_texture = texture;
    m_amplitude = amplitude;
    m_waveSpeed = waveSpeed;
    m_frequency = frequency;
    m_startPosition = position;
    m_position = position;
}

bool Particle::Tick(float deltatime, const Float3& direction)
{
    if (m_alive)
    {
        m_lifeTime += deltatime;
        m_alive = m_lifeTime < m_maxLifeTime;
        m_position += direction * m_speed;
   
        // Wave equation: y = a * sin(kx-wt+phase)
        const float wt = m_waveSpeed * m_lifeTime;
        const float kx = m_frequency * m_position.y;
        const float zOffset = 2.0f;
   
        m_position.x = m_startPosition.x + m_amplitude * std::sin(kx - wt);
        m_position.z = m_startPosition.z + m_amplitude * std::sin(kx - wt * zOffset);
   
        // Fade particle in/out of lifetime
        const float fadeEnd = m_maxLifeTime - m_lifeFade;
        if (m_lifeTime <= m_lifeFade)
        {
            m_alpha = Clamp(ConvertRange(m_lifeTime, 
                0.0f, m_lifeFade, 0.0f, 1.0f), 0.0f, 1.0f);
        }
        else if (m_lifeTime >= fadeEnd)
        {
            m_alpha = Clamp(ConvertRange(m_lifeTime, 
                fadeEnd, m_maxLifeTime, 1.0f, 0.0f), 0.0f, 1.0f);
        }
        return true;
    }
    else if (m_waitTime < m_maxWaitTime)
    {
        m_waitTime += deltatime;
        return true;
    }
    return false;
}

bool Particle::Alive() const
{
    return m_alive;
}

const float& Particle::Alpha() const
{
    return m_alpha;
}

int Particle::Texture() const
{
    return m_texture;
}

const Float3& Particle::Position() const
{
    return m_position;
}

float Particle::Size() const
{
    return m_size;
}