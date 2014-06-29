////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - tweaker.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <memory>

#ifdef _VS
#include "generated/ui_tweaker.h"
#else
#include "ui_tweaker.h"
#endif

class Tweaker : public QWidget
{
    Q_OBJECT

public:
    Tweaker(QWidget* parent = nullptr);

private slots:


private:

    Ui::Tweaker ui;
};
