#ifdef _DEBUG
#include "vld/include/vld.h"
#endif

#include <irrlicht.h>
#include "game.h"
#include "logger.h"

#ifdef _IRR_WINDOWS_
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

/**
* Main Entry Point for Application
*/
int main()
{
    Logger::Initialise();
    Game* game = Game::Get();

    if(!game->Initialise())
    {
        Game::Release();
        Logger::LogError("Game failed to initialise");
        return EXIT_FAILURE;
    }

    if(!game->GameLoop())
    {
        Game::Release();
        Logger::LogError("Game loop exited unexpectedly");
        return EXIT_FAILURE;
    }

    Game::Release();
    Logger::LogInfo("Game exited successfully");
    return EXIT_SUCCESS;
}
