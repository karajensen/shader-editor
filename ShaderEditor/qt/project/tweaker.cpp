////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - tweaker.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include <QtGui>
#include "tweaker.h"

Tweaker::Tweaker(QWidget *parent) :
    QWidget(parent)
{
    m_ui.setupUi(this);
}

std::string Tweaker::GetSelectedPage() const
{
    return m_ui.TabMenu->currentWidget()->objectName().toStdString();
}

void Tweaker::SetDeltaTime(const std::string& dt)
{
    m_ui.deltaTime_text->setText(QString(dt.c_str()));
    m_ui.deltaTime_text->update();
}

void Tweaker::SetMouse(const std::string& positionX, 
                       const std::string& positionY,
                       const std::string& directionX,
                       const std::string& directionY)
{
    m_ui.mousePosX_text->setText(QString(positionX.c_str()));
    m_ui.mousePosX_text->update();

    m_ui.mousePosY_text->setText(QString(positionY.c_str()));
    m_ui.mousePosY_text->update();

    m_ui.mouseDirX_text->setText(QString(directionX.c_str()));
    m_ui.mouseDirX_text->update();

    m_ui.mouseDirY_text->setText(QString(directionY.c_str()));
    m_ui.mouseDirY_text->update();
}
