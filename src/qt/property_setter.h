////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - property_setter.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QObject>
#include <QVariant>

class PropertySetter : public QObject
{
    Q_OBJECT

public:

    /**
    * Allows setting a qml property without breaking its bindings
    * @param object The qml object to set
    * @param propertyName The name of the property to set
    * @param value The value to use when setting
    */
    Q_INVOKABLE void SetValue(QObject* object, QString propertyName, QVariant value);

};
