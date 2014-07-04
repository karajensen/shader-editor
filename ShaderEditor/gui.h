////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - gui.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "common.h"

class Cache;

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
    * Ticks the guid
    * @param argc Number of arguments passed into the application
    * @Param argv Arguments passed into the application
    */
    void Run(int argc, char *argv[]);

private:

    std::shared_ptr<Cache> m_cache; ///< Shared data between the gui and application
};