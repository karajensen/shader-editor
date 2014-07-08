////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - tweakable.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include <QtGui>
#include "tweakable.h"

Tweakable::Tweakable() :
    QWidget(nullptr),
    m_signalCallback(nullptr),
    m_previousDialValue(0),
    m_box(nullptr)
{
}

void Tweakable::Initialise(double value, 
                           double step,
                           QDoubleSpinBox* box, 
                           QDial* dial,
                           std::function<void(float)> signalCallback)
{
    m_signalCallback = signalCallback;
    m_box = box;

    box->setValue(value);
    box->setSingleStep(step);

    connect(box, SIGNAL(valueChanged(double)), 
        this, SLOT(UpdateValue(double)));  

    connect(dial, SIGNAL(sliderMoved(int)), 
        this, SLOT(DialValue(int)));  

    box->update();
    dial->update();
}

void Tweakable::UpdateValue(double value)
{
    m_signalCallback(value);
}

void Tweakable::DialValue(int value)
{
    if(value != m_previousDialValue)
    {
        const bool clockwise = value > m_previousDialValue;
        clockwise ? m_box->stepUp() : m_box->stepDown();
        m_previousDialValue = value;
    }
}

bool Tweakable::IsInitialised() const
{
    return m_signalCallback != nullptr;
}