////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - meshAttributes.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "meshAttributes.h"

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
