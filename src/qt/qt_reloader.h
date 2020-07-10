////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - qt_reloader.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QObject>

class QQmlApplicationEngine;

class QtReloader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString qmlSourcePath READ QmlSourcePath CONSTANT)

public:

    /**
    * Constructor
    * @param engine Qml engine required for reloading
    */
    explicit QtReloader(QQmlApplicationEngine& engine, QObject* parent = nullptr);

    /**
    * Reloads the tweaker and editor content
    */
    Q_INVOKABLE void Reload();

    /**
    * @return the qml source path
    */
    QString QmlSourcePath() const;

private:
    QQmlApplicationEngine& m_engine; ///< Qml engine required for reloading
};
