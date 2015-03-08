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

ComboEntry::ComboEntry(int Attribute,
                       const std::string& Name,
                       double Step,
                       int Precision,
                       std::function<void(float)> Callback) :

    attribute(Attribute),
    step(Step),
    value(0.0),
    callback(Callback),
    name(Name),
    precision(Precision)
{
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

    m_box->setSingleStep(m_entries[index].step);
    m_box->setDecimals(m_entries[index].precision);
    m_box->setValue(m_entries[index].value);

    m_box->update();
    m_dial->update();
}

bool TweakableBoxedValue::SetValue(int attribute, double value)
{
    for (unsigned int i = 0; i < m_entries.size(); ++i)
    {
        if (m_entries[i].attribute == attribute)
        {
            m_entries[i].value = value;
            if (i == m_selectedIndex)
            {
                m_box->setValue(m_entries[i].value);
            }
            return true;
        }
    }
    return false;
}
