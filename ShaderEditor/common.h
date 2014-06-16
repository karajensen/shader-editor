////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - common.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
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
static const int MULTISAMPLING_COUNT = 4;
static const float CAMERA_NEAR = 1.0f;
static const float CAMERA_FAR = 1000.0f;
static const float FIELD_OF_VIEW = 60.0f;
static const int NO_INDEX = -1;

const std::string SHADER_EXTENSION(".fx");
const std::string ASM_EXTENSION(".as");
const std::string GLSL_VERTEX("_glsl_vert");
const std::string GLSL_FRAGMENT("_glsl_frag");
const std::string HLSL_SHADER("_hlsl");
const std::string GLSL_FRAGMENT_EXTENSION(GLSL_FRAGMENT + SHADER_EXTENSION);   
const std::string GLSL_VERTEX_EXTENSION(GLSL_VERTEX + SHADER_EXTENSION);
const std::string HLSL_SHADER_EXTENSION(HLSL_SHADER + SHADER_EXTENSION);
const std::string ASSETS_PATH(".//Assets//");
const std::string SHADER_PATH(ASSETS_PATH + "Shaders//");
const std::string TEXTURE_PATH(ASSETS_PATH + "Textures//");
const std::string GENERATED_PATH(SHADER_PATH + "Generated//");
const std::string SAMPLES(boost::lexical_cast<std::string>(MULTISAMPLING_COUNT));

/**
* Simple 2D point
*/
struct Float2
{
    Float2() : x(0), y(0)
    {
    }

    Float2(float X, float Y) : x(X), y(Y)
    {
    }

    float x, y;
};

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