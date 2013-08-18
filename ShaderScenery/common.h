////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - common.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <irrlicht.h>
#include <vector>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/smart_ptr.hpp>
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

/**
* Irrlicht engine callbacks
*/
struct IrrlichtEngine
{
    /**
    * Constructor
    */
    IrrlichtEngine() :
        device(nullptr),
        driver(nullptr),
        scene(nullptr),
        gui(nullptr)
    {
    }

    IrrlichtDevice* device; ///< Irrlicht engine device
    IVideoDriver* driver;   ///< Irrlicht video driver
    ISceneManager* scene;   ///< Irrlicht scene manager
    IGUIEnvironment* gui;   ///< Irrlicht gui device
};
typedef boost::shared_ptr<IrrlichtEngine> EnginePtr;

/**
* Gets the value if it exists in the tree or returns defaultValue
* @param itr Iteration from the tree to check 
* @param defaultValue The default value to use if name is not found
* @param node The name of the node to search
* @return the chosen value from either the tree or defaultValue
*/
template<typename T>
T GetPtreeValue(boost::property_tree::ptree::iterator& itr, T defaultValue, char* node)
{
    int count = itr->second.count(node);
    if(count > 0)
    {
        return boost::lexical_cast<T>(itr->second.get_child(node).data());
    }
    return defaultValue;
}