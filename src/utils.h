////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - utils.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>

/**
* Converts degrees to radians
*/
template<typename T> T DegToRad(T degrees)
{
    return static_cast<T>(M_PI / 180.0) * degrees;
}

/**
* Converts radians to degrees
*/
template<typename T> T RadToDeg(T radians)
{
    return static_cast<T>(180.0 / M_PI)*radians;
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
    return ((value-currentInner) * ((newOuter-newInner) /
        (currentOuter-currentInner))) + newInner;
}

/**
* Clamp a value between min and max
*/
template<typename T> T Clamp(T value, T minValue, T maxValue)
{
    return std::min(std::max(value, minValue), maxValue);
}
