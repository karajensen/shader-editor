////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - tweakable_box.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "tweakable_box.h"

TweakableBox::TweakableBox() :
    QWidget(nullptr),
    m_signalCallback(nullptr),
    m_comboBox(nullptr)
{
}

void TweakableBox::Initialise(QComboBox* comboBox, 
                              int selectedItem,
                              const std::vector<std::string>& items,
                              std::function<void(int)> signalCallback)
{
    m_signalCallback = signalCallback;
    m_comboBox = comboBox;

    for(const std::string& item : items)
    {
        comboBox->addItem(QString((" " + item).c_str()));
    }

    comboBox->setCurrentIndex(selectedItem);

    connect(comboBox, SIGNAL(currentIndexChanged(int)), 
        this, SLOT(UpdateSelected(int)));  
}

bool TweakableBox::IsInitialised() const
{
    return m_comboBox ? m_comboBox->count() > 0 : false;
}

void TweakableBox::UpdateSelected(int index)
{
    m_signalCallback(index);
}
