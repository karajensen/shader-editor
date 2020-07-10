////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - attribute.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QObject>

/**
* Data for a tweakable attribute
*/
class Attribute : public QObject
{
    Q_OBJECT

public:

    /**
    * Constructor
    * @param group Id for the group the attribute belongs to
    * @param name Attribute display name
    * @param precision Amount of decimals places to display and step the value by
    * @param enabled Whether the attribute is editable
    */
    Attribute(int group, const QString& name, int precision, bool enabled, QObject* parent = nullptr);

    /**
    * Property getter for attribute display name
    */
    const QString& Name() const;

    /**
    * Property getter for the group the attribute belongs to
    */
    int Group() const;

    /**
    * Property getter for attribute value step amount
    */
    float StepSize() const;

    /**
    * Property getter for amount of decimals places to display and step the value by
    */
    int Precision() const;

    /**
    * Property getter for whether the attribute is editable
    */
    bool Enabled() const;

    /**
    * Property setter/getter for attribute value
    */
    float Value() const;
    void SetValue(float value);

signals:

    void ValueChanged(float value);

private:

    QString m_name;             ///< Attribute display name
    float m_value = 0.0f;       ///< Attribute value
    float m_stepSize = 0.0f;    ///< Attribute value step amount
    int m_precision = 1.0;      ///< Amount of decimals places to display and step the value by
    bool m_enabled = true;      ///< Whether the attribute is editable
    int m_group = -1;           ///< Id for the group the attribute belongs to
};
