////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - light.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "light.h"
#include "cache.h"

Light::Light(const std::string& name)
    : m_name(name)
{
}

void Light::Write(Cache& cache)
{
    cache.Light[Tweakable::Light::PositionX].SetUpdated(m_position.x);
    cache.Light[Tweakable::Light::PositionY].SetUpdated(m_position.y);
    cache.Light[Tweakable::Light::PositionZ].SetUpdated(m_position.z);
    cache.Light[Tweakable::Light::AttenuationX].SetUpdated(m_attenuation.x);
    cache.Light[Tweakable::Light::AttenuationY].SetUpdated(m_attenuation.y);
    cache.Light[Tweakable::Light::AttenuationZ].SetUpdated(m_attenuation.z);
    cache.Light[Tweakable::Light::DiffuseR].SetUpdated(m_diffuse.r);
    cache.Light[Tweakable::Light::DiffuseG].SetUpdated(m_diffuse.g);
    cache.Light[Tweakable::Light::DiffuseB].SetUpdated(m_diffuse.b);
    cache.Light[Tweakable::Light::SpecularR].SetUpdated(m_specular.r);
    cache.Light[Tweakable::Light::SpecularG].SetUpdated(m_specular.g);
    cache.Light[Tweakable::Light::SpecularB].SetUpdated(m_specular.b);
    cache.Light[Tweakable::Light::Specularity].SetUpdated(m_specularity);
    cache.Light[Tweakable::Light::Active].SetUpdated(m_active);
}

void Light::Read(Cache& cache)
{
    m_position.x = cache.Light[Tweakable::Light::PositionX].Get();
    m_position.y = cache.Light[Tweakable::Light::PositionY].Get();
    m_position.z = cache.Light[Tweakable::Light::PositionZ].Get();
    m_attenuation.x = cache.Light[Tweakable::Light::AttenuationX].Get();
    m_attenuation.y = cache.Light[Tweakable::Light::AttenuationY].Get();
    m_attenuation.z = cache.Light[Tweakable::Light::AttenuationZ].Get();
    m_diffuse.r = cache.Light[Tweakable::Light::DiffuseR].Get();
    m_diffuse.g = cache.Light[Tweakable::Light::DiffuseG].Get();
    m_diffuse.b = cache.Light[Tweakable::Light::DiffuseB].Get();
    m_specular.r = cache.Light[Tweakable::Light::SpecularR].Get();
    m_specular.g = cache.Light[Tweakable::Light::SpecularG].Get();
    m_specular.b = cache.Light[Tweakable::Light::SpecularB].Get();
    m_specularity = cache.Light[Tweakable::Light::Specularity].Get();
    m_active = cache.Light[Tweakable::Light::Active].Get();
}

const std::string& Light::Name() const
{
    return m_name;
}

const Colour& Light::Diffuse() const
{
    return m_diffuse;
}

const Colour& Light::Specular() const
{
    return m_specular;
}

const Float3& Light::Attenuation() const
{
    return m_attenuation;
}

const Float3& Light::Position() const
{
    return m_position;
}

const float& Light::Specularity() const
{
    return m_specularity;
}

const float& Light::Active() const
{
    return m_active;
}

void Light::Diffuse(const Colour& value)
{
    m_diffuse = value;
}

void Light::Specular(const Colour& value)
{
    m_specular = value;
}

void Light::Attenuation(const Float3& value)
{
    m_attenuation = value;
}

void Light::Position(const Float3& value)
{
    m_position = value;
}

void Light::PositionX(float x)
{
    m_position.x = x;
}

void Light::PositionZ(float z)
{
    m_position.z = z;
}

void Light::Specularity(float value)
{
    m_specularity = value;
}

void Light::Active(float value)
{
    m_active = value;
}
