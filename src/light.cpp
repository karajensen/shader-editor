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
    cache.Light[LIGHT_POSITION_X].SetUpdated(m_position.x);
    cache.Light[LIGHT_POSITION_Y].SetUpdated(m_position.y);
    cache.Light[LIGHT_POSITION_Z].SetUpdated(m_position.z);
    cache.Light[LIGHT_ATTENUATION_X].SetUpdated(m_attenuation.x);
    cache.Light[LIGHT_ATTENUATION_Y].SetUpdated(m_attenuation.y);
    cache.Light[LIGHT_ATTENUATION_Z].SetUpdated(m_attenuation.z);
    cache.Light[LIGHT_DIFFUSE_R].SetUpdated(m_diffuse.r);
    cache.Light[LIGHT_DIFFUSE_G].SetUpdated(m_diffuse.g);
    cache.Light[LIGHT_DIFFUSE_B].SetUpdated(m_diffuse.b);
    cache.Light[LIGHT_SPECULAR_R].SetUpdated(m_specular.r);
    cache.Light[LIGHT_SPECULAR_G].SetUpdated(m_specular.g);
    cache.Light[LIGHT_SPECULAR_B].SetUpdated(m_specular.b);
    cache.Light[LIGHT_SPECULARITY].SetUpdated(m_specularity);
    cache.Light[LIGHT_ACTIVE].SetUpdated(m_active);
}

void Light::Read(Cache& cache)
{
    m_position.x = cache.Light[LIGHT_POSITION_X].Get();
    m_position.y = cache.Light[LIGHT_POSITION_Y].Get();
    m_position.z = cache.Light[LIGHT_POSITION_Z].Get();
    m_attenuation.x = cache.Light[LIGHT_ATTENUATION_X].Get();
    m_attenuation.y = cache.Light[LIGHT_ATTENUATION_Y].Get();
    m_attenuation.z = cache.Light[LIGHT_ATTENUATION_Z].Get();
    m_diffuse.r = cache.Light[LIGHT_DIFFUSE_R].Get();
    m_diffuse.g = cache.Light[LIGHT_DIFFUSE_G].Get();
    m_diffuse.b = cache.Light[LIGHT_DIFFUSE_B].Get();
    m_specular.r = cache.Light[LIGHT_SPECULAR_R].Get();
    m_specular.g = cache.Light[LIGHT_SPECULAR_G].Get();
    m_specular.b = cache.Light[LIGHT_SPECULAR_B].Get();
    m_specularity = cache.Light[LIGHT_SPECULARITY].Get();
    m_active = cache.Light[LIGHT_ACTIVE].Get();
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
