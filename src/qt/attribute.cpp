////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - attribute.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "attribute.h"

Attribute::Attribute(int id, const QString& name, float value, int stepPrecision, QObject* parent)
    : QObject(parent)
    , m_id(id)
    , m_name(name)
    , m_value(value)
    , m_stepSize(0.1f / stepPrecision)
{
}

int Attribute::Id() const
{
    return m_id;
}

const QString& Attribute::Name() const
{
    return m_name;
}

float Attribute::StepSize() const
{
    return m_stepSize;
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
