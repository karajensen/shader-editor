////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - colour.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

/**
* Utility class for manipulating colour
*/
struct Colour
{
    enum Component
    {
        RED,
        GREEN,
        BLUE,
        ALPHA
    };

    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
    float a = 0.0f;
};