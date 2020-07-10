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
        NameRole = Qt::UserRole + 1,
        GroupRole,
        ValueRole,
        StepSizeRole,
        PrecisionRole,
        EnabledRole,
    };

    struct AttributeData
    {
        int index = -1;          ///< Index into the attribute enum
        int group = -1;          ///< Id for the group the attribute belongs to
        QString name;            ///< Attribute display name
        int precision = 0;       ///< Amount of decimals to display and step by
        bool enabled = true;     ///< Whether the attribute is editable
    };

    using QAbstractListModel::QAbstractListModel;

    /**
    * Sets the attributes of the model
    * @param attributeData The attribute data to set
    */
    void SetAttributes(const QVector<AttributeData>& attributeData);

    /**
    * Sets the value for an attribute
    * @param index The attribute to set
    * @param float Attribute value to set
    */
    void SetAttributeValue(int index, float value);

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
    * @param index The attribute to get
    * @param role The type of data to set from Roles enum
    * @return value The value of the attribute
    */
    virtual QVariant data(const QModelIndex& index, int role) const override;

    /**
    * Sets the value for a role for an attribute
    * @param index The attribute to set
    * @param value The new value to set
    * @param role The type of data to set from Roles enum
    */
    virtual bool setData(const QModelIndex& index, const QVariant& value, int role) override;

    /**
    * @return whether the index is valid to access an attribute item
    */
    bool IndexValid(const QModelIndex& index) const;

signals:

    void AttributeValueChanged(int index, float value);

private:

    QVector<Attribute*> m_attributes;  ///< Attribute items for the model
};
