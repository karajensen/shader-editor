////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - attribute.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "attribute.h"

Attribute::Attribute(int group, const QString& name, int stepPrecision, bool enabled, QObject* parent)
    : QObject(parent)
    , m_name(name)
    , m_stepSize(0.1f / stepPrecision)
    , m_enabled(enabled)
    , m_group(group)
{
}

const QString& Attribute::Name() const
{
    return m_name;
}

int Attribute::Group() const
{
    return m_group;
}

float Attribute::StepSize() const
{
    return m_stepSize;
}

bool Attribute::Enabled() const
{
    return m_enabled;
}

float Attribute::Value() const
{
    return m_value;
}

void Attribute::SetValue(float value)
{
    if (m_value != value)
    {
        m_value = value;
        emit ValueChanged(value);
    }
}
