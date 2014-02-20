////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - common.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include <sstream>
#include <memory>
#include <array>
#include <functional>

#include "boost/property_tree/ptree.hpp"
#include "boost/lexical_cast.hpp"

#include "logger.h"
#include "float3.h"
#include "matrix.h"

static const int WINDOW_WIDTH = 800;
static const int WINDOW_HEIGHT = 600;
static const int WINDOW_X = (GetSystemMetrics(SM_CXSCREEN)/2)-(WINDOW_WIDTH/2); 
static const int WINDOW_Y = (GetSystemMetrics(SM_CYSCREEN)/2)-(WINDOW_HEIGHT/2);
static const float CAMERA_NEAR = 0.1f;
static const float CAMERA_FAR = 100.0f;
static const float FOV = 60.0f;
static const int NO_INDEX = -1;

const std::string FRAGMENT_EXTENSION(".frag");   
const std::string VERTEX_EXTENSION(".vert");  
const std::string ASSETS_PATH(".//Assets//");
const std::string SHADER_PATH(ASSETS_PATH + "Shaders//");
const std::string GENERATED_FOLDER(SHADER_PATH + "Generated//");

/**
* Colour avaliable in the application
*/
struct Colour
{
    Colour() : r(0), g(0), b(0), a(0) 
    {
    }

    float r, g, b, a;
};

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