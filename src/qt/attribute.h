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
    * @param name Attribute display name
    * @param value Initial value
    * @param stepPrecision Amount of decimals places to step the value by
    */
    Attribute(const QString& name, float value, int stepPrecision, QObject* parent = nullptr);

    /**
    * Property getter for attribute display name
    */
    const QString& Name() const;

    /**
    * Property getter for attribute value step amount
    */
    float StepSize() const;

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
};
