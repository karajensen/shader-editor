////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - stringlist_model.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QStringListModel>

/**
* Model for a string list with selection
*/
class StringListModel : public QStringListModel
{
    Q_OBJECT
    Q_PROPERTY(int selectedIndex READ SelectedIndex WRITE SetSelectedIndex NOTIFY SelectedIndexChanged)

public:

    using QStringListModel::QStringListModel;

    /**
    * Setter for std vector and string
    * @param list The string list to set
    */
    void SetStringList(const std::vector<std::string>& list);

    /**
    * Property setter/getter for the selected string index
    */
    int SelectedIndex() const;
    void SetSelectedIndex(int index);

    /**
    * @return whether this model has strings or not
    */
    bool Empty() const;

signals:

    void SelectedIndexChanged(int index);

private:

    int m_selectedIndex = -1; ///< Selected string index
};
