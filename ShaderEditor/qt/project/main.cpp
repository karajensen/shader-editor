////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - main.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include <QApplication>
#include "editor.h"
#include "tweaker.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Editor editor;
    editor.show();

    Tweaker tweaker;
    tweaker.show();

    return app.exec();
}

