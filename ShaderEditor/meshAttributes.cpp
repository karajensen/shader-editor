////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - meshAttributes.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "meshAttributes.h"

MeshAttributes::MeshAttributes(const boost::property_tree::ptree& node)
{
    m_causticsAmount = GetAttributeOptional<float>(node, "Caustics", "amount", 1.0f);
    m_causticsScale = GetAttributeOptional<float>(node, "Caustics", "scale", 1.0f);
    m_specularity = GetValueOptional<float>(node, 0.0f, "Specularity");
    m_ambience = GetValueOptional<float>(node, 1.0f, "Ambience");
    m_bump = GetValueOptional<float>(node, 0.0f, "Bump");
    m_diffuse = GetValueOptional<float>(node, 1.0f, "Diffuse");
    m_specular = GetValueOptional<float>(node, 1.0f, "Specular");
}

void MeshAttributes::Write(boost::property_tree::ptree& node) const
{
    AddValueOptional(node, "Bump", m_bump, 0.0f);
    AddValueOptional(node, "Ambience", m_ambience, 1.0f);
    AddValueOptional(node, "Specularity", m_specularity, 0.0f);
    AddValueOptional(node, "Caustics.<xmlattr>.amount", m_causticsAmount, 1.0f);
    AddValueOptional(node, "Caustics.<xmlattr>.scale", m_causticsScale, 1.0f);
    AddValueOptional(node, "Diffuse", m_diffuse, 1.0f);
    AddValueOptional(node, "Specular", m_specular, 1.0f);
}

const float& MeshAttributes::Bump() const
{
    return m_bump;
}

const float& MeshAttributes::Specularity() const
{
    return m_specularity;
}

const float& MeshAttributes::Ambience() const
{
    return m_ambience;
}

const float& MeshAttributes::Specular() const
{
    return m_specular;
}

const float& MeshAttributes::Diffuse() const
{
    return m_diffuse;
}

const float& MeshAttributes::CausticsAmount() const
{
    return m_causticsAmount;
}

const float& MeshAttributes::CausticsScale() const
{
    return m_causticsScale;
}

void MeshAttributes::Specularity(float value)
{
    m_specularity = value;
}

void MeshAttributes::Bump(float value)
{
    m_bump = value;
}

void MeshAttributes::Ambience(float value)
{
    m_ambience = value;
}

void MeshAttributes::Diffuse(float value)
{
    m_diffuse = value;
}

void MeshAttributes::Specular(float value)
{
    m_specular = value;
}

void MeshAttributes::CausticsAmount(float value)
{
    m_causticsAmount = value;
}

void MeshAttributes::CausticsScale(float value)
{
    m_causticsScale = value;
}
