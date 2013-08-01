/****************************************************************
* Kara Jensen (mail@karajensen.com)
* Common includes/headers/functions
*****************************************************************/
#pragma once

#include <irrlicht.h>
#include <vector>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include "logger.h"

using namespace irr;
using namespace irr::core;
using namespace irr::scene;
using namespace irr::video;
using namespace irr::io;
using namespace irr::gui;

static const int WINDOW_WIDTH = 800;
static const int WINDOW_HEIGHT = 600;
static const int NO_INDEX = -1;
const std::string ASSETS_PATH(".//Assets//");

IrrlichtDevice*  Device();
IVideoDriver*    Driver();
ISceneManager*   Scene();
IGUIEnvironment* Gui();

/**
* Gets the value if it exists in the tree or returns defaultValue
* @param the tree to check 
* @param the default value to use if name is not found
* @param the name of the node to search
* @return the chosen value from either the tree or defaultValue
*/
template<typename T>
T GetPtreeValue(boost::property_tree::ptree::iterator& it, T defaultValue, char* node)
{
    int count = it->second.count(node);
    if(count > 0)
    {
        return boost::lexical_cast<T>(it->second.get_child(node).data());
    }
    return defaultValue;
}