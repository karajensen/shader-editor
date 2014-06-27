
#include <QApplication>
#include "gui.h"
#include "tweaker.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Gui gui;
    gui.show();

    Tweaker tweaker;
    tweaker.show();

    return app.exec();
}

