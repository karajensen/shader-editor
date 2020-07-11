////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - attribute.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "attribute.h"

Attribute::Attribute(QObject* parent)
    : QObject(parent)
{
}

Attribute::Attribute(int group, const QString& name, int precision, bool enabled, QObject* parent)
    : QObject(parent)
    , m_name(name)
    , m_stepSize(std::pow(0.1, precision))
    , m_precision(precision)
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

int Attribute::Precision() const
{
    return m_precision;
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
    if (std::fabs(m_value - value) > std::numeric_limits<float>::epsilon())
    {
        m_value = value;
        emit ValueChanged(value);
    }
}

float Attribute::MinValue() const
{
    return -10000.0f;
}

float Attribute::MaxValue() const
{
    return 10000.0f;
}
