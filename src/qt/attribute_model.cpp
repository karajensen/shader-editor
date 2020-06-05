////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - attribute_model.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "attribute_model.h"
#include "attribute.h"

void AttributeModel::SetAttributes(const QVector<AttributeData>& attributeData)
{
    beginResetModel();

    m_attributes.resize(attributeData.size());
    for (const auto& data : attributeData)
    {
        m_attributes[data.id] = new Attribute(
            data.id,
            data.name,
            data.value,
            data.stepPrecision,
            this);

        connect(m_attributes.back(), &Attribute::ValueChanged, this, [this, id = data.id](float value)
            {
                const auto modelIndex = this->index(id);
                emit dataChanged(modelIndex, modelIndex, { Role::ValueRole });
                emit AttributeValueChanged(id, value);
            });
    }

    endResetModel();
}

bool AttributeModel::IndexValid(const QModelIndex& index) const
{
    return index.row() >= 0 && index.row() < m_attributes.size();
}

void AttributeModel::SetAttributeValue(int id, float value)
{
    m_attributes.at(id)->SetValue(value);
}

QHash<int, QByteArray> AttributeModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Role::NameRole] = "name";
    roles[Role::IdRole] = "id";
    roles[Role::StepSizeRole] = "stepSize";
    roles[Role::ValueRole] = "value";
    return roles;
}

int AttributeModel::rowCount(const QModelIndex& parent) const
{
    return m_attributes.size();
}

QVariant AttributeModel::data(const QModelIndex& index, int role) const
{
    if (IndexValid(index))
    {
        const auto attribute = m_attributes[index.row()];
        switch (role)
        {
        case Role::IdRole:
            return attribute->Id();
        case Role::NameRole:
            return attribute->Name();
        case Role::StepSizeRole:
            return attribute->StepSize();
        case Role::ValueRole:
            return attribute->Value();
        }
    }
    return QVariant();
}

bool AttributeModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (IndexValid(index))
    {
        const auto attribute = m_attributes[index.row()];
        switch (role)
        {
        case Role::ValueRole:
            attribute->SetValue(value.toFloat());
            return true;
        }
    }
    return false;
}