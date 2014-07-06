////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - gui.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "common.h"

struct Cache;
class Tweaker;
class Editor;

/**
* Manages qt for the gui
*/
class Gui
{
public:

    /**
    * Constructor
    * @param cache Shared data between the gui and application
    */
    Gui(std::shared_ptr<Cache> cache);

    /**
    * Destructor
    */
    ~Gui();

    /**
    * Ticks the guid
    * @param argc Number of arguments passed into the application
    * @Param argv Arguments passed into the application
    */
    void Run(int argc, char *argv[]);

private:

    /**
    * Updates the cache
    * @param tweaker The gui for tweaking the application
    * @param editor The gui for editing the application
    */
    void UpdateCache(Tweaker& tweaker, Editor& editor);

    /**
    * Sets the functions required to update the cache from the gui
    * @param tweaker The gui for tweaking the application
    * @param editor The gui for editing the application
    */
    void SetSignalCallbacks(Tweaker& tweaker, Editor& editor);

    /**
    * Converts the string description to the page enum
    * @param page The string description of the page
    * @return the enum page id
    */
    GuiPage ConvertStringToPage(const std::string& page);

    GuiPage m_page;                     ///< Currently selected page of the gui
    std::shared_ptr<Cache> m_cache;     ///< Shared data between the gui and application
};