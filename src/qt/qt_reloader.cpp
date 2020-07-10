////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - qt_reloader.h
////////////////////////////////////////////////////////////////////////////////////////

#include "qt_reloader.h"
#include "logger.h"

#include <QQmlApplicationEngine>

QtReloader::QtReloader(QQmlApplicationEngine& engine, QObject* parent)
    : QObject(parent)
    , m_engine(engine)
{
}

void QtReloader::Reload()
{
#ifdef QML_FILE_PATH
    m_engine.clearComponentCache();
    Logger::LogInfo("Reloaded QML");
#endif
}

QString QtReloader::QmlSourcePath() const
{
#ifdef QML_FILE_PATH
    return "file:///" QML_FILE_PATH;
#endif
    return "qrc:/";
}
