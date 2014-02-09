////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - common.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <memory>
#include "logger.h"

static const int WINDOW_WIDTH = 800;
static const int WINDOW_HEIGHT = 600;
static const int NO_INDEX = -1;
const std::string ASSETS_PATH(".//Assets//");

/**
* Output the given value as a string
*/
template<typename T> std::string StringCast(const T& value)
{
    return static_cast<std::stringstream&>(
        std::stringstream() << value).str();
}