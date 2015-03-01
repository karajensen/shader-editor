////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - light.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "light.h"
#include "cache.h"

Light::Light(const boost::property_tree::ptree& node)
{
    m_name = GetValue<std::string>(node, "Name");
    m_diffuse.r = GetAttribute<float>(node, "Diffuse", "r");
    m_diffuse.g = GetAttribute<float>(node, "Diffuse", "g");
    m_diffuse.b = GetAttribute<float>(node, "Diffuse", "b");
    m_specular.r = GetAttribute<float>(node, "Specular", "r");
    m_specular.g = GetAttribute<float>(node, "Specular", "g");
    m_specular.b = GetAttribute<float>(node, "Specular", "b");
    m_position.x = GetAttribute<float>(node, "Position", "x");
    m_position.y = GetAttribute<float>(node, "Position", "y");
    m_position.z = GetAttribute<float>(node, "Position", "z");
    m_attenuation.x = GetAttribute<float>(node, "Attenuation", "x");
    m_attenuation.y = GetAttribute<float>(node, "Attenuation", "y");
    m_attenuation.z = GetAttribute<float>(node, "Attenuation", "z");
    m_specularity = GetValue<float>(node, "Specularity");
}

void Light::Write(boost::property_tree::ptree& node) const
{
    node.add("Name", m_name.c_str());
    node.add("Position.<xmlattr>.x", m_position.x);
    node.add("Position.<xmlattr>.y", m_position.y);
    node.add("Position.<xmlattr>.z", m_position.z);
    node.add("Attenuation.<xmlattr>.x", m_attenuation.x);
    node.add("Attenuation.<xmlattr>.y", m_attenuation.y);
    node.add("Attenuation.<xmlattr>.z", m_attenuation.z);
    node.add("Diffuse.<xmlattr>.r", m_diffuse.r);
    node.add("Diffuse.<xmlattr>.g", m_diffuse.g);
    node.add("Diffuse.<xmlattr>.b", m_diffuse.b);
    node.add("Specular.<xmlattr>.r", m_specular.r);
    node.add("Specular.<xmlattr>.g", m_specular.g);
    node.add("Specular.<xmlattr>.b", m_specular.b);
    node.add("Specularity", m_specularity);
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