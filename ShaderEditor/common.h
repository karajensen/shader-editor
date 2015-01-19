////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - common.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

//#define TEST_LEAKS
#ifdef TEST_LEAKS
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

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int WINDOW_X = (GetSystemMetrics(SM_CXSCREEN)/2)-(WINDOW_WIDTH/2); 
const int WINDOW_Y = (GetSystemMetrics(SM_CYSCREEN)/2)-(WINDOW_HEIGHT/2);
const int MULTISAMPLING_COUNT = 4;
const float FRUSTRUM_NEAR = 1.0f;
const float FRUSTRUM_FAR = 2000.0f;
const float FIELD_OF_VIEW = 60.0f;

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
const std::string CUBEMAP_FOLDER("cubemap");
const std::string SAMPLES(boost::lexical_cast<std::string>(MULTISAMPLING_COUNT));

const std::string BLUR_SHADER("blur");
const std::string POST_SHADER("post");
const std::string WATER_SHADER("water");
const std::string WATER_NORMAL_SHADER("water_normal");
const std::string PARTICLE_SHADER("particle");
const std::string PARTICLE_NORMAL_SHADER("particle_normal");
const std::string NORMAL_SHADER("normal");
const std::string BASE_SHADER("shader");

enum ShaderIndex
{
    POST_SHADER_INDEX,
    BLUR_SHADER_INDEX,
    WATER_SHADER_INDEX,
    WATER_NORMAL_SHADER_INDEX,
    PARTICLE_SHADER_INDEX,
    PARTICLE_NORMAL_SHADER_INDEX
};

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
* Displays a popup message box
* @param message The message to display
* @param title The header of the popup box
*/
inline void ShowMessage(const std::string& message, const std::string& title)
{
    MessageBox(nullptr, message.c_str(), title.c_str(), MB_OK);
}