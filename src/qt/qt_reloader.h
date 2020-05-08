////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - qt_reloader.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QObject>

class QQmlApplicationEngine;

class QtReloader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString tweakerQmlSourcePath READ tweakerQmlSourcePath CONSTANT)
    Q_PROPERTY(QString editorQmlSourcePath READ editorQmlSourcePath CONSTANT)

public:

    /**
    * Constructor
    * @param engine Qml engine required for reloading
    */
    explicit QtReloader(QQmlApplicationEngine& engine, QObject* parent = nullptr);

    /**
    * Reloads the tweaker and editor content
    */
    Q_INVOKABLE void reload();

    /**
    * @return qml path to the tweaker content, will be a local path for debug builds
    */
    QString tweakerQmlSourcePath() const;

    /**
    * @return qml path to the tweaker content, will be a local path for debug builds
    */
    QString editorQmlSourcePath() const;

private:
    QQmlApplicationEngine& m_engine; ///< Qml engine required for reloading
};
