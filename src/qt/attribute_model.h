////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - attribute_model.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QAbstractListModel>

class Attribute;

/**
* Model for a group of tweakable attributes
*/
class AttributeModel : public QAbstractListModel
{
    Q_OBJECT

public:

    enum Role
    {
        IdRole = Qt::UserRole + 1,
        NameRole,
        ValueRole,
        StepSizeRole
    };

    struct AttributeData
    {
        int id = -1;             ///< Unique attribute id that maps to its enum value
        QString name;            ///< Attribute display name
        float value = 0.0f;      ///< Initial value
        int stepPrecision = 0;   ///< Amount of decimals places to step the value by
    };

    using QAbstractListModel::QAbstractListModel;

    /**
    * Sets the attributes of the model
    * @param attributeData The attribute data to set
    */
    void SetAttributes(const QVector<AttributeData>& attributeData);

    /**
    * Sets the value for an attribute
    * @param id Unique attribute id that maps to its enum value
    * @param float Attribute value to set
    */
    void SetAttributeValue(int id, float value);

    /**
    * @return the names of the model roles to use in QML to access attribute data
    */
    virtual QHash<int, QByteArray> roleNames() const override;

    /**
    * @return The number of attributes this model holds
    */
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    /**
    * Gets the value for a role for an attribute
    * @param index The index into the attributes to set
    * @param role The type of data to set from Roles enum
    * @return value The value of the attribute
    */
    virtual QVariant data(const QModelIndex& index, int role) const override;

    /**
    * Sets the value for a role for an attribute
    * @param index The index into the attributes to set
    * @param value The new value to set
    * @param role The type of data to set from Roles enum
    */
    virtual bool setData(const QModelIndex& index, const QVariant& value, int role) override;

signals:

    void AttributeValueChanged(int id, float value);

private:
    
    /**
    * @return whether the index is valid to access an attribute item
    */
    bool IndexValid(const QModelIndex& index) const;

private:

    QVector<Attribute*> m_attributes;  ///< Attribute items for the model
};
