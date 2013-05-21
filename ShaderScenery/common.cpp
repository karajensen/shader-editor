#include "common.h"
#include "game.h"
#include <boost/assign/list_of.hpp>

IrrlichtDevice* Device()
{
    return Game::Get()->m_device;
}

IVideoDriver* Driver()
{
    return Game::Get()->m_driver;
}

ISceneManager* Scene()
{
    return Game::Get()->m_scene;
}

IGUIEnvironment* Gui()
{
    return Game::Get()->m_gui;
}