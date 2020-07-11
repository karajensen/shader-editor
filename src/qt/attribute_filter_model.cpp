////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - attribute_filter_model.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "attribute_filter_model.h"
#include "attribute_model.h"
#include "attribute.h"
#include "logger.h"

AttributeFilterModel::AttributeFilterModel(QObject* parent)
    : QSortFilterProxyModel(parent)
{
    setFilterRole(AttributeModel::Role::GroupRole);
}

void AttributeFilterModel::setSourceModel(QAbstractItemModel* sourceModel)
{
    auto attributeModel = dynamic_cast<AttributeModel*>(sourceModel);
    if (!attributeModel)
    {
        Logger::LogError("Invalid source model");
        return;
    }

    m_attributeModel = attributeModel;
    QSortFilterProxyModel::setSourceModel(sourceModel);
}

Attribute* AttributeFilterModel::SelectedItem() const
{
    return m_attributeModel ? m_attributeModel->item(
        mapToSource(this->index(m_selectedIndex, 0)).row()) : nullptr;
}

int AttributeFilterModel::SelectedIndex() const
{
    return m_selectedIndex;
}

void AttributeFilterModel::SetSelectedIndex(int index)
{
    if (m_selectedIndex != index)
    {
        m_selectedIndex = index;
        emit SelectedIndexChanged();
    }
}
