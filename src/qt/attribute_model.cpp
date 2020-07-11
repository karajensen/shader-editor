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
        auto attribute = new Attribute(data.group, data.name, data.precision, data.enabled, this);
        m_attributes[data.index] = attribute;
        connect(attribute, &Attribute::ValueChanged, this, [this, index = data.index](float value)
            {
                const auto modelIndex = this->index(index);
                emit dataChanged(modelIndex, modelIndex, { Role::ValueRole });
                emit AttributeValueChanged(index, value);
            });
    }

    endResetModel();
}

bool AttributeModel::IndexValid(const QModelIndex& index) const
{
    return IndexValid(index.row());
}

bool AttributeModel::IndexValid(int index) const
{
    return index >= 0 && index < m_attributes.size();
}

void AttributeModel::SetAttributeValue(int index, float value)
{
    m_attributes.at(index)->SetValue(value);
}

QHash<int, QByteArray> AttributeModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "display";
    roles[Role::NameRole] = "name";
    roles[Role::StepSizeRole] = "stepSize";
    roles[Role::ValueRole] = "value";
    roles[Role::MinValueRole] = "minValue";
    roles[Role::MaxValueRole] = "maxValue";
    roles[Role::PrecisionRole] = "precision";
    roles[Role::EnabledRole] = "enabled";
    roles[Role::GroupRole] = "group";
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
        case Qt::DisplayRole:
        case Role::NameRole:
            return attribute->Name();
        case Role::PrecisionRole:
            return attribute->Precision();
        case Role::StepSizeRole:
            return attribute->StepSize();
        case Role::ValueRole:
            return attribute->Value();
        case Role::MinValueRole:
            return attribute->MinValue();
        case Role::MaxValueRole:
            return attribute->MaxValue();
        case Role::EnabledRole:
            return attribute->Enabled();
        case Role::GroupRole:
            return attribute->Group();
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

Attribute* AttributeModel::item(int index) const
{
    return IndexValid(index) ? m_attributes[index] : nullptr;
}

Attribute* AttributeModel::SelectedItem() const
{
    return item(m_selectedIndex);
}

int AttributeModel::SelectedIndex() const
{
    return m_selectedIndex;
}

void AttributeModel::SetSelectedIndex(int index)
{
    if (m_selectedIndex != index)
    {
        m_selectedIndex = index;
        emit SelectedIndexChanged();
    }
}