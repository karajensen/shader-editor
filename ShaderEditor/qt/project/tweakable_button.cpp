////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - tweakable_button.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "tweakable_button.h"

TweakableButton::TweakableButton() :
    QWidget(nullptr),
    m_signalCallback(nullptr)
{
    //background-color: rgb(230, 230, 230);\nborder-top-color: rgb(255, 255, 255);\nborder-left-color: rgb(255, 255, 255);\nborder-bottom-color: rgb(180, 180, 180);\nborder-right-color: rgb(180, 180, 180);\nborder-style: solid;\nborder-width: 2px;
}

void TweakableButton::Initialise(QPushButton* button,
                                 std::function<void(void)> signalCallback)
{
    m_signalCallback = signalCallback;

    connect(button, SIGNAL(pressed(void)), 
        this, SLOT(OnButtonPress(void)));  
}

void TweakableButton::OnButtonPress()
{
    m_signalCallback();
}
