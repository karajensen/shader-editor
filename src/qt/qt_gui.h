////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - qt_gui.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "cache.h"

#include <QObject>

class TweakerModel;
class EditorModel;
class QtReloader;
class QGuiApplication;
class QQmlApplicationEngine;

/**
* Manages qt for the gui
*/
class QtGui : public QObject
{
    Q_OBJECT

public:

    /**
    * Constructor
    * @param cache Shared data between the gui and application
    */
    QtGui(std::shared_ptr<Cache> cache);

    /**
    * Destructor
    */
    ~QtGui();

    /**
    * Ticks the gui
    * @param argc Number of arguments passed into the application
    * @param argv Arguments passed into the application
    */
    void Run(int argc, char *argv[]);

private:

    /**
    * Updates the tweak bar
    */
    void UpdateTweaker();

    /**
    * Updates the editor
    */
    void UpdateEditor();

    /**
    * Updates the cache and gui for the scene page
    */
    void UpdateScene();

    /**
    * Updates the light tweakable entries
    */
    void UpdateLight();

    /**
    * Updates the mesh tweakable entries
    */
    void UpdateMesh();

    /**
    * Updates the water tweakable entries
    */
    void UpdateWater();

    /**
    * Updates the emitter tweakable entries
    */
    void UpdateEmitter();

    /**
    * Updates the texture tweakable entries
    */
    void UpdateTextures();

    /**
    * Updates the terrain tweakable entries
    */
    void UpdateTerrain();

    /**
    * Updates the cache and gui for the post page
    */
    void UpdatePost();

    /**
    * Initializes connections between the models and application
    * @param editor The gui for editing shaders
    * @param tweaker The gui for tweaking the application
    */
    void SetupConnections();

    /**
    * Registers all classes to be used in QML
    */
    void RegisterQmlTypes();

private:

    std::unique_ptr<QGuiApplication> m_app;           ///< Qt main application
    std::unique_ptr<QQmlApplicationEngine> m_engine;  ///< Qt QML engine
    std::unique_ptr<TweakerModel> m_tweaker;          ///< The gui for tweaking the application
    std::unique_ptr<EditorModel> m_editor;            ///< The gui for editing shaders
    std::unique_ptr<QtReloader> m_reloader;           ///< Allows reloading qml files
    std::shared_ptr<Cache> m_cache;                   ///< Shared data between the gui and application
    Tweakable::GuiPage::Page m_page;                  ///< Currently selected page of the gui
};