////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - colour.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <algorithm>

/**
* Colour with red, green, blue and alpha components
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

    static void SetRed(unsigned int& colour, int value)
    {
        SetColour(colour, value, GreenAsInt(colour), BlueAsInt(colour), AlphaAsInt(colour));
    }

    static void SetGreen(unsigned int& colour, int value)
    {
        SetColour(colour, RedAsInt(colour), value, BlueAsInt(colour), AlphaAsInt(colour));
    }

    static void SetBlue(unsigned int& colour, int value)
    {
        SetColour(colour, RedAsInt(colour), GreenAsInt(colour), value, AlphaAsInt(colour));
    }

    static void SetAlpha(unsigned int& colour, int value)
    {
        SetColour(colour, RedAsInt(colour), GreenAsInt(colour), BlueAsInt(colour), value);
    }

    static void SetRed(unsigned int& colour, float value)
    {
        SetRed(colour, static_cast<int>(value * 255));
    }

    static void SetGreen(unsigned int& colour, float value)
    {
        SetGreen(colour, static_cast<int>(value * 255));
    }

    static void SetBlue(unsigned int& colour, float value)
    {
        SetBlue(colour, static_cast<int>(value * 255));
    }

    static void SetAlpha(unsigned int& colour, float value)
    {
        SetAlpha(colour, static_cast<int>(value * 255));
    }

    static void SetColour(unsigned int& colour, int r, int g, int b, int a)
    {
        using namespace std;
        r = min(255, max(0, r));
        g = min(255, max(0, g));
        b = min(255, max(0, b));
        a = min(255, max(0, a));
        colour = ((a & 0xFF) << 24) + 
            ((b & 0xFF) << 16) + 
            ((g & 0xFF) << 8) + 
            (r & 0xFF);
    }

    static int RedAsInt(unsigned int colour)
    {
        return colour & 0xFF;
    }

    static int GreenAsInt(unsigned int colour)
    {
        return (colour >> 8) & 0xFF;
    }

    static int BlueAsInt(unsigned int colour)
    {
        return (colour >> 16) & 0xFF;
    }

    static int AlphaAsInt(unsigned int colour)
    {
        return (colour >> 24) & 0xFF;
    }

    static float RedAsFlt(unsigned int colour)
    {
        return static_cast<float>(RedAsInt(colour)) / 255.0f;
    }

    static float GreenAsFlt(unsigned int colour)
    {
        return static_cast<float>(GreenAsInt(colour)) / 255.0f;
    }

    static float BlueAsFlt(unsigned int colour)
    {
        return static_cast<float>(BlueAsInt(colour)) / 255.0f;
    }

    static float AlphaAsFlt(unsigned int colour)
    {
        return static_cast<float>(AlphaAsInt(colour)) / 255.0f;
    }
};