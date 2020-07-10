////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - stringlist_model.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "stringlist_model.h"

void StringListModel::SetStringList(const std::vector<std::string>& list)
{
    QStringList stringList;
    stringList.reserve(static_cast<int>(list.size()));
    for (const auto& str : list)
    {
        stringList.push_back(QString::fromStdString(str));
    }
    setStringList(stringList);
}

int StringListModel::SelectedIndex() const
{
    return m_selectedIndex;
}

void StringListModel::SetSelectedIndex(int index)
{
    if (m_selectedIndex != index)
    {
        m_selectedIndex = index;
        emit SelectedIndexChanged(index);
    }
}

bool StringListModel::Empty() const
{
    return rowCount() == 0;
}