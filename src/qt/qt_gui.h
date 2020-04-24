////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - qt_gui.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "common.h"
#include "cache.h"

class Tweaker;
class Editor;

/**
* Manages qt for the gui
*/
class QtGui
{
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
    * @param tweaker The gui for tweaking the application
    */
    void UpdateTweaker(Tweaker& tweaker);

    /**
    * Updates the editor
    * @param editor The gui for editing shaders
    */
    void UpdateEditor(Editor& editor);

    /**
    * Updates the cache and gui for the scene page
    * @param tweaker The gui for tweaking the application
    */
    void UpdateScene(Tweaker& tweaker);

    /**
    * Updates the light tweakable entries
    * @param tweaker The gui for tweaking the application
    */
    void UpdateLight(Tweaker& tweaker);

    /**
    * Updates the mesh tweakable entries
    * @param tweaker The gui for tweaking the application
    */
    void UpdateMesh(Tweaker& tweaker);

    /**
    * Updates the water tweakable entries
    * @param tweaker The gui for tweaking the application
    */
    void UpdateWater(Tweaker& tweaker);

    /**
    * Updates the emitter tweakable entries
    * @param tweaker The gui for tweaking the application
    */
    void UpdateEmitter(Tweaker& tweaker);

    /**
    * Updates the texture tweakable entries
    * @param tweaker The gui for tweaking the application
    */
    void UpdateTextures(Tweaker& tweaker);

    /**
    * Updates the terrain tweakable entries
    * @param tweaker The gui for tweaking the application
    */
    void UpdateTerrain(Tweaker& tweaker);

    /**
    * Updates the cache and gui for the post page
    * @param tweaker The gui for tweaking the application
    */
    void UpdatePost(Tweaker& tweaker);

    /**
    * Converts the string description to the page enum
    * @param page The string description of the page
    * @return the enum page id
    */
    GuiPage ConvertStringToPage(const std::string& page);

    /**
    * Registers all qobjects for qml usage
    */
    void RegisterQmlTypes();

    GuiPage m_page;                     ///< Currently selected page of the gui
    std::shared_ptr<Cache> m_cache;     ///< Shared data between the gui and application
};