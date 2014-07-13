////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - tweakable_value.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "tweakable_value.h"

TweakableValue::TweakableValue() :
    QWidget(nullptr),
    m_signalCallback(nullptr),
    m_previousDialValue(0),
    m_box(nullptr)
{
}

void TweakableValue::Initialise(double step,
                                QDoubleSpinBox* box, 
                                QDial* dial,
                                std::function<void(float)> signalCallback)
{
    m_signalCallback = signalCallback;
    m_box = box;

    box->setValue(0.0);
    box->setSingleStep(step);

    connect(box, SIGNAL(valueChanged(double)), 
        this, SLOT(UpdateValue(double)));  

    connect(dial, SIGNAL(sliderMoved(int)), 
        this, SLOT(DialValue(int)));  

    box->update();
    dial->update();
}

void TweakableValue::Set(double value)
{
    m_box->setValue(value);
}

void TweakableValue::UpdateValue(double value)
{
    m_signalCallback(value);
}

void TweakableValue::DialValue(int value)
{
    if(value != m_previousDialValue)
    {
        value > m_previousDialValue ? m_box->stepUp() : m_box->stepDown();
        m_previousDialValue = value;
    }
}
