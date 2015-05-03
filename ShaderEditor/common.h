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
#include <sstream>
#include <memory>
#include <array>
#include <functional>
#include <array>
#include <vector>
#include "logger.h"
#include "float3.h"
#include "matrix.h"
#include "colour.h"
#include "random_generator.h"
#include "boost/noncopyable.hpp"

static const int NO_INDEX = -1;

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
* Change the range a value is between
*/
template<typename T> T ConvertRange(T value,    
                                    T currentInner, 
                                    T currentOuter, 
                                    T newInner, 
                                    T newOuter)
{
    return ((value-currentInner)*((newOuter-newInner)/
        (currentOuter-currentInner)))+newInner;
}

/**
* Clamp a value between min and max
*/
template<typename T> T Clamp(T value, T minValue, T maxValue)
{
    return std::min(std::max(value, minValue), maxValue);
}