////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - attribute_filter_model.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QSortFilterProxyModel>

class Attribute;
class AttributeModel;

/**
* Model for a group of tweakable attributes
*/
class AttributeFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(Attribute* selectedItem READ SelectedItem NOTIFY SelectedIndexChanged)
    Q_PROPERTY(int selectedIndex READ SelectedIndex WRITE SetSelectedIndex NOTIFY SelectedIndexChanged)

public:

    /**
    * Constructor
    */
    explicit AttributeFilterModel(QObject* parent = nullptr);

    /**
    * Property setter/getter for the selected string index and item
    */
    Attribute* SelectedItem() const;
    int SelectedIndex() const;
    void SetSelectedIndex(int index);

    /**
    * Sets the source attribute model to filter
    */
    virtual void setSourceModel(QAbstractItemModel* sourceModel) override;

signals:

    void SelectedIndexChanged();

private:

    int m_selectedIndex = 0;                   ///< Selected attribute index
    AttributeModel* m_attributeModel{nullptr}; ///< The source attribute model to filter
};
