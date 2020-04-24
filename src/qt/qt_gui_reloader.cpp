////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - qt_gui_reloader.h
////////////////////////////////////////////////////////////////////////////////////////

#include "qt_gui_reloader.h"
#include "logger.h"

#include <QtQml/qqmlapplicationengine.h>

QtGuiReloader::QtGuiReloader(QQmlApplicationEngine& engine)
    : m_engine(engine)
{
}

void QtGuiReloader::reload()
{
    m_engine.clearComponentCache();
    Logger::LogInfo("Reloaded QML");
}

QString QtGuiReloader::tweakerQmlSourcePath() const
{
#ifdef QML_FILE_PATH
    return "file:///" QML_FILE_PATH "Tweaker.qml";
#endif
    return "qrc/Tweaker.qml";
}

QString QtGuiReloader::editorQmlSourcePath() const
{
#ifdef QML_FILE_PATH
    return "file:///" QML_FILE_PATH "Editor.qml";
#endif
    return "qrc/Editor.qml";
}