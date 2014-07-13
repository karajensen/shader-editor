////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - editor.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include <QtGui>
#include "editor.h"
#include "signals.h"

Editor::Editor(QWidget *parent) :
    QWidget(parent)
{
    m_ui.setupUi(this);
}

Editor::Editor(const SignalCallbacks& callbacks, QWidget *parent) :
    QWidget(parent)
{
    m_ui.setupUi(this);
}

void Editor::InitialiseShaders(int selected, const std::vector<std::string>& shaders)
{



}

bool Editor::HasShaders() const
{
    return true;
}