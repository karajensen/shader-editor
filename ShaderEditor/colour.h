////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - colour.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <algorithm>

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
    * Sets the colour component
    */
    static void SetRed(unsigned int& colour, int value)
    {
        SetColour(colour, value, GreenAsInt(colour),
            BlueAsInt(colour), AlphaAsInt(colour));
    }

    /**
    * Sets the colour component
    */
    static void SetGreen(unsigned int& colour, int value)
    {
        SetColour(colour, RedAsInt(colour), value, 
            BlueAsInt(colour), AlphaAsInt(colour));
    }

    /**
    * Sets the colour component
    */
    static void SetBlue(unsigned int& colour, int value)
    {
        SetColour(colour, RedAsInt(colour), 
            GreenAsInt(colour), value, AlphaAsInt(colour));
    }

    /**
    * Sets the colour component
    */
    static void SetAlpha(unsigned int& colour, int value)
    {
        SetColour(colour, RedAsInt(colour), 
            GreenAsInt(colour), BlueAsInt(colour), value);
    }

    /**
    * Sets the colour component
    */
    static void SetRed(unsigned int& colour, float value)
    {
        SetRed(colour, static_cast<int>(value * 255));
    }

    /**
    * Sets the colour component
    */
    static void SetGreen(unsigned int& colour, float value)
    {
        SetGreen(colour, static_cast<int>(value * 255));
    }

    /**
    * Sets the colour component
    */
    static void SetBlue(unsigned int& colour, float value)
    {
        SetBlue(colour, static_cast<int>(value * 255));
    }

    /**
    * Sets the colour component
    */
    static void SetAlpha(unsigned int& colour, float value)
    {
        SetAlpha(colour, static_cast<int>(value * 255));
    }

    /**
    * Sets the colour
    */
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

    /**
    * Gets the colour component
    */
    static unsigned char RedAsChar(unsigned int colour)
    {
        return colour & 0xFF;
    }

    /**
    * Gets the colour component
    */
    static unsigned char GreenAsChar(unsigned int colour)
    {
        return (colour >> 8) & 0xFF;
    }

    /**
    * Gets the colour component
    */
    static unsigned char BlueAsChar(unsigned int colour)
    {
        return (colour >> 16) & 0xFF;
    }

    /**
    * Gets the colour component
    */
    static unsigned char AlphaAsChar(unsigned int colour)
    {
        return (colour >> 24) & 0xFF;
    }

    /**
    * Gets the colour component
    */
    static int RedAsInt(unsigned int colour)
    {
        return RedAsChar(colour);
    }

    /**
    * Gets the colour component
    */
    static int GreenAsInt(unsigned int colour)
    {
        return GreenAsChar(colour);
    }

    /**
    * Gets the colour component
    */
    static int BlueAsInt(unsigned int colour)
    {
        return BlueAsChar(colour);
    }

    /**
    * Gets the colour component
    */
    static int AlphaAsInt(unsigned int colour)
    {
        return AlphaAsChar(colour);
    }

    /**
    * Gets the colour component
    */
    static float RedAsFlt(unsigned int colour)
    {
        return static_cast<float>(RedAsChar(colour)) / 255.0f;
    }

    /**
    * Gets the colour component
    */
    static float GreenAsFlt(unsigned int colour)
    {
        return static_cast<float>(GreenAsChar(colour)) / 255.0f;
    }

    /**
    * Gets the colour component
    */
    static float BlueAsFlt(unsigned int colour)
    {
        return static_cast<float>(BlueAsChar(colour)) / 255.0f;
    }

    /**
    * Gets the colour component
    */
    static float AlphaAsFlt(unsigned int colour)
    {
        return static_cast<float>(AlphaAsChar(colour)) / 255.0f;
    }
};