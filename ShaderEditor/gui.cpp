////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - gui.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "gui.h"
#include "cache.h"
#include "qt/project/editor.h"
#include "qt/project/tweaker.h"
#include "qt/include/QtWidgets/qapplication.h"

Gui::Gui(std::shared_ptr<Cache> cache) :
    m_cache(cache)
{
}

void Gui::Run(int argc, char *argv[])
{
    Logger::LogInfo("Initialising Qt");
    QApplication app(argc, argv);

    Editor editor;
    editor.setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowTitleHint);
    editor.show();

    Tweaker tweaker;
    tweaker.setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowTitleHint);
    tweaker.show();
    
    while(m_cache->ApplicationRunning())
    {
        app.processEvents();
    }

    app.exit();
    Logger::LogInfo("Exiting Qt");
}