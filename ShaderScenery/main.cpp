////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - main.cpp
////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#include "vld/include/vld.h"
#endif

#include <irrlicht.h>
#include "game.h"
#include "logger.h"

#ifdef _IRR_WINDOWS_
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

int main()
{
    Logger::Initialise();
    boost::scoped_ptr<Game> game(new Game());

    if(!game->Initialise())
    {
        Logger::LogError("Game failed to initialise");
        return EXIT_FAILURE;
    }

    if(!game->GameLoop())
    {
        Logger::LogError("Game loop exited unexpectedly");
        return EXIT_FAILURE;
    }

    Logger::LogInfo("Game exited successfully");
    return EXIT_SUCCESS;
}
