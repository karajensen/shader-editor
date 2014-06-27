
#pragma once

#include <memory>

#include "ui_tweaker.h"

class Tweaker : public QWidget
{
    Q_OBJECT

public:
    Tweaker(QWidget* parent = nullptr);

private slots:


private:

    Ui::Tweaker ui;
};
