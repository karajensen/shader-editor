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

    /**
    * Constructor
    */
    Colour() :
        r(0.0f),
        g(0.0f),
        b(0.0f),
        a(0.0f)
    {
    }

    /**
    * Constructor
    */
    Colour(float R, float G, float B, float A) :
        r(R),
        g(G),
        b(B),
        a(A)
    {
    }
};