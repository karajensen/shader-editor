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
#include "logger.h"
#include "float3.h"

static const int WINDOW_WIDTH = 800;
static const int WINDOW_HEIGHT = 600;
static const int WINDOW_X = (GetSystemMetrics(SM_CXSCREEN)/2)-(WINDOW_WIDTH/2); 
static const int WINDOW_Y = (GetSystemMetrics(SM_CYSCREEN)/2)-(WINDOW_HEIGHT/2);

static const float CAMERA_NEAR = 0.1f;
static const float CAMERA_FAR = 100.0f;
static const float FOV = 60.0f;
static const int NO_INDEX = -1;

const std::string ASSETS_PATH(".//Assets//");

/**
* Colour avaliable in the application
*/
enum ColourComponent
{
    RED,
    GREEN,
    BLUE,
    ALPHA,
    MAX_COMPONENTS
};
typedef std::array<float, MAX_COMPONENTS> Colour;

/**
* Output the given value as a string
*/
template<typename T> std::string StringCast(const T& value)
{
    return static_cast<std::stringstream&>(
        std::stringstream() << value).str();
}