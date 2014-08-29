////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - common.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifdef _DEBUG
#include "vld/include/vld.h"
#endif

#define USE_CONSOLE
#ifndef USE_CONSOLE
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <Windows.h>
#include <string>
#include <vector>
#include <sstream>
#include <memory>
#include <array>
#include <functional>

#include "boost/noncopyable.hpp"
#include "boost/property_tree/ptree.hpp"
#include "boost/lexical_cast.hpp"

#include "logger.h"
#include "elements.h"

typedef std::ostringstream sstream;

static const int WINDOW_WIDTH = 800;
static const int WINDOW_HEIGHT = 600;
static const int WINDOW_X = (GetSystemMetrics(SM_CXSCREEN)/2)-(WINDOW_WIDTH/2); 
static const int WINDOW_Y = (GetSystemMetrics(SM_CYSCREEN)/2)-(WINDOW_HEIGHT/2);
static const int MULTISAMPLING_COUNT = 4;
static const float FRUSTRUM_NEAR = 1.0f;
static const float FRUSTRUM_FAR = 1000.0f;
static const float FIELD_OF_VIEW = 60.0f;

const std::string SHADER_EXTENSION(".fx");
const std::string ASM_EXTENSION(".as");
const std::string GLSL_VERTEX("_glsl_vert");
const std::string GLSL_FRAGMENT("_glsl_frag");
const std::string HLSL_SHADER("_hlsl");
const std::string GLSL_HEADER("#version");
const std::string GLSL_FRAGMENT_EXTENSION(GLSL_FRAGMENT + SHADER_EXTENSION);   
const std::string GLSL_VERTEX_EXTENSION(GLSL_VERTEX + SHADER_EXTENSION);
const std::string HLSL_SHADER_EXTENSION(HLSL_SHADER + SHADER_EXTENSION);
const std::string ASSETS_PATH(".//Assets//");
const std::string SHADER_PATH(ASSETS_PATH + "Shaders//");
const std::string TEXTURE_PATH(ASSETS_PATH + "Textures//");
const std::string GENERATED_PATH(SHADER_PATH + "Generated//");
const std::string SAMPLES(boost::lexical_cast<std::string>(MULTISAMPLING_COUNT));
const std::string POST_NAME("PostShader");
const std::string NORMAL_NAME("NormalShader");

/**
* Converts degrees to radians
*/
template<typename T> T DegToRad(T degrees)
{
    return static_cast<T>(M_PI/180.0)*degrees;
}

/**
* Converts radians to degrees
*/
template<typename T> T RadToDeg(T radians)
{
    return static_cast<T>(180.0/M_PI)*radians;
}

/**
* Gets the value if it exists in the tree or returns defaultValue
* @param itr Iteration from the tree to check 
* @param defaultValue The default value to use if name is not found
* @param node The name of the node to search
* @return the chosen value from either the tree or defaultValue
*/
template<typename T> T GetPtreeValue(boost::property_tree::ptree::iterator& itr, 
                                     T defaultValue, 
                                     const char* node)
{
    int count = itr->second.count(node);
    if(count > 0)
    {
        return boost::lexical_cast<T>(
            itr->second.get_child(node).data());
    }
    return defaultValue;
}

/**
* Displays a popup message box
* @param message The message to display
* @param title The header of the popup box
*/
inline void ShowMessage(const std::string& message, const std::string& title)
{
    MessageBox(nullptr, message.c_str(), title.c_str(), MB_OK);
}