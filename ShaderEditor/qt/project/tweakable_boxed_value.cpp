////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - tweakable_boxed_value.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "tweakable_boxed_value.h"

TweakableBoxedValue::TweakableBoxedValue() :
    QWidget(nullptr),
    m_comboBox(nullptr),
    m_previousDialValue(0),
    m_box(nullptr),
    m_dial(nullptr),
    m_selectedIndex(0)
{
}

ComboEntry::ComboEntry() :
    step(0.0),
    value(0.0),
    callback(nullptr)
{
}

void ComboEntry::Set(const std::string& Name,
                     double Step,
                     std::function<void(float)> Callback)
{
    step = Step;
    callback = Callback;
    name = Name;
}

void TweakableBoxedValue::Initialise(QComboBox* comboBox,
                                     QDoubleSpinBox* box,
                                     QDial* dial,
                                     const std::vector<ComboEntry>& entries)
{
    m_entries = entries;
    m_comboBox = comboBox;
    m_dial = dial;
    m_box = box;

    for (const auto& entry : m_entries)
    {
        comboBox->addItem(QString((" " + entry.name).c_str()));
    }

    comboBox->setCurrentIndex(m_selectedIndex);
    UpdateSelected(m_selectedIndex);

    connect(comboBox, SIGNAL(currentIndexChanged(int)),
        this, SLOT(UpdateSelected(int)));

    connect(m_box, SIGNAL(valueChanged(double)),
        this, SLOT(UpdateValue(double)));

    connect(m_dial, SIGNAL(sliderMoved(int)),
        this, SLOT(DialValue(int)));
}

void TweakableBoxedValue::UpdateValue(double value)
{
    m_entries[m_selectedIndex].callback(value);
}

void TweakableBoxedValue::DialValue(int value)
{
    if (value != m_previousDialValue)
    {
        value > m_previousDialValue ? m_box->stepUp() : m_box->stepDown();
        m_previousDialValue = value;
    }
}

void TweakableBoxedValue::UpdateSelected(int index)
{
    m_entries[m_selectedIndex].value = m_box->value();
    m_selectedIndex = index;

    m_box->setValue(m_entries[index].value);
    m_box->setSingleStep(m_entries[index].step);

    m_box->update();
    m_dial->update();
}

void TweakableBoxedValue::SetValue(int index, double value)
{
    m_entries[index].value = value;

    if (index == m_selectedIndex)
    {
        m_box->setValue(m_entries[index].value);
    }
}
