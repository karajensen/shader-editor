
#pragma once

#include <memory>
#include "ui_editor.h"

class Editor : public QWidget
{
    Q_OBJECT

public:

    Editor(QWidget* parent = nullptr);

private slots:


private:

    Ui::Editor m_ui;
};
