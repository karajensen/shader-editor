////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - emitter.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "emitter.h"
#include "common.h"
#include <random>

namespace
{
    static std::default_random_engine generator; ///< Used for random generation

    /**
    * Utility class to get a random value
    */
    int GetRandom(int min, int max)
    {
        std::uniform_int_distribution<int> distribution(min, max);
        return distribution(generator);
    }

    /**
    * Utility class to get a random value
    */
    float GetRandom(float min, float max)
    {
        std::uniform_real_distribution<float> distribution(min, max);
        return distribution(generator);
    }
}

Particle::Particle() :
    maxWaitTime(GetRandom(0.0f,1.0f))
{
}

void Emitter::Resize(int size)
{
    const int currentSize = static_cast<int>(particles.size());
    if (size < currentSize)
    {
        particles.erase(particles.begin() + size, particles.end());
    }
    else if (size > currentSize)
    {
        particles.resize(size);
    }
}

void Emitter::TogglePaused()
{
    paused = !paused;
}

void Emitter::Tick(float deltatime)
{
    if (paused)
    {
        return;
    }

    for (Particle& particle : particles)
    {
        if (particle.alive)
        {
            particle.lifeTime += deltatime;
            particle.alive = particle.lifeTime < lifeTime;
            particle.position += direction * particle.speed;

            // Wave equation: y = a * sin(kx-wt+phase)
            const float wt = particle.waveSpeed * particle.lifeTime;
            const float kx = particle.frequency * particle.position.y;
            const float zOffset = 2.0f;

            particle.position.x = particle.startPosition.x +
                particle.amplitude * std::sin(kx - wt);

            particle.position.z = particle.startPosition.z +
                particle.amplitude * std::sin(kx - wt * zOffset);

            // Fade particle in/out of lifetime
            const float fadeEnd = lifeTime - lifeFade;
            if (particle.lifeTime <= lifeFade)
            {
                particle.alpha = Clamp(ConvertRange(particle.lifeTime, 
                    0.0f, lifeFade, 0.0f, 1.0f), 0.0f, 1.0f);
            }
            else if (particle.lifeTime >= fadeEnd)
            {
                particle.alpha = Clamp(ConvertRange(particle.lifeTime, 
                    fadeEnd, lifeTime, 1.0f, 0.0f), 0.0f, 1.0f);
            }
        }
        else if (particle.waitTime < particle.maxWaitTime)
        {
            particle.waitTime += deltatime;
        }
        else
        {
            particle.alpha = 0.0f;
            particle.waitTime = 0.0f;
            particle.lifeTime = 0.0f;
            particle.alive = true;
            particle.speed = GetRandom(minSpeed, maxSpeed);
            particle.size = GetRandom(minSize, maxSize);
            particle.texture = textures[GetRandom(0, static_cast<int>(textures.size()-1))];
            particle.amplitude = GetRandom(minAmplitude, maxAmplitude);
            particle.waveSpeed = GetRandom(minWaveSpeed, maxWaveSpeed);
            particle.frequency = GetRandom(minFrequency, maxFrequency);

            const float xOffset = GetRandom(-width, width) * 0.5f;
            const float zOffset = GetRandom(-length, length) * 0.5f;
            particle.startPosition.x = position.x + xOffset;
            particle.startPosition.z = position.z + zOffset;
            particle.startPosition.y = position.y;
            particle.position = particle.startPosition;
        }
    }
}