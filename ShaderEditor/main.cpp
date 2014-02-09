////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - main.cpp
////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#include "vld/include/vld.h"
#endif

#include "application.h"
#include "logger.h"

int main()
{
    Logger::Initialise();
    std::unique_ptr<Application> game(new Application());

    if(!game->Initialise())
    {
        Logger::LogError("Application failed to initialise");
        return EXIT_FAILURE;
    }

    if(!game->Run())
    {
        Logger::LogError("Application loop exited unexpectedly");
        return EXIT_FAILURE;
    }

    Logger::LogInfo("Application exited successfully");
    return EXIT_SUCCESS;
}
