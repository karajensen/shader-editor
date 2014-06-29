////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - editor.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <memory>

#ifdef _VS
#include "generated/ui_editor.h"
#else
#include "ui_editor.h"
#endif

class Editor : public QWidget
{
    Q_OBJECT

public:

    Editor(QWidget* parent = nullptr);

private slots:


private:

    Ui::Editor m_ui;
};
