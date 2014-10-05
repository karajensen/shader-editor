////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - tweakable_button.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "tweakable_button.h"

TweakableButton::TweakableButton() :
    QWidget(nullptr),
    m_signalCallback(nullptr),
    m_button(nullptr)
{
}

void TweakableButton::Initialise(QPushButton* button,
                                 std::function<void(void)> signalCallback)
{
    m_button = button;
    m_signalCallback = signalCallback;

    connect(button, SIGNAL(pressed(void)), 
        this, SLOT(OnButtonPress(void)));  

    connect(button, SIGNAL(released(void)), 
        this, SLOT(OnButtonRelease(void))); 
}

void TweakableButton::OnButtonPress()
{
    m_button->setStyleSheet(QString(
        "background-color: rgb(220, 220, 220);\n"
        "border-top-color: rgb(180, 180, 180);\n"
        "border-left-color: rgb(180, 180, 180);\n"
        "border-bottom-color: rgb(255, 255, 255);\n"
        "border-right-color: rgb(255, 255, 255);\n"
        "border-style: solid;\nborder-width: 2px;"));
}

void TweakableButton::OnButtonRelease()
{
    m_button->setStyleSheet(QString(
        "background-color: rgb(230, 230, 230);\n"
        "border-top-color: rgb(255, 255, 255);\n"
        "border-left-color: rgb(255, 255, 255);\n"
        "border-bottom-color: rgb(180, 180, 180);\n"
        "border-right-color: rgb(180, 180, 180);\n"
        "border-style: solid;\nborder-width: 2px;"));

    m_signalCallback();
}