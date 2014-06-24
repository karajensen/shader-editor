
#pragma once

#include <memory>

#include "ui_gui.h"

class Gui : public QWidget
{
    Q_OBJECT

public:
    Gui(QWidget* parent = nullptr);

private slots:


private:

    Ui::Gui ui;
};
