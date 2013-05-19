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

void ShowDiagnosticText(const std::wstring& text)
{
    Game::Get()->m_diagText->setBackgroundColor(Game::Get()->m_diagBgroundCol);
    Game::Get()->m_diagText->setDrawBackground(true);
    Game::Get()->m_diagText->setText(text.c_str());
    Game::Get()->m_diagRunTimer = true;
}