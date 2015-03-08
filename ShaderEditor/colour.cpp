////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - colour.cpp
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "colour.h"
#include "common.h"

Pixel::Pixel() :
    r(0),
    g(0),
    b(0),
    a(0)
{
}

float Pixel::Red() const 
{ 
    return static_cast<float>(r) / 255.0f; 
}

float Pixel::Green() const 
{ 
    return static_cast<float>(g) / 255.0f; 
}

float Pixel::Blue() const 
{ 
    return static_cast<float>(b) / 255.0f; 
}

float Pixel::Alpha() const 
{ 
    return static_cast<float>(a) / 255.0f; 
}

void Pixel::Red(float red)
{ 
    r = static_cast<unsigned char>(Clamp(red, 0.0f, 1.0f) * 255.0f); 
}

void Pixel::Green(float green)
{ 
    g = static_cast<unsigned char>(Clamp(green, 0.0f, 1.0f) * 255.0f); 
}

void Pixel::Blue(float blue)
{ 
    b = static_cast<unsigned char>(Clamp(blue, 0.0f, 1.0f) * 255.0f); 
}

void Pixel::Alpha(float alpha)
{ 
    a = static_cast<unsigned char>(Clamp(alpha, 0.0f, 1.0f) * 255.0f); 
}

void Pixel::AddRed(float amount)
{
    AddToComponent(amount, r);
}

void Pixel::AddGreen(float amount)
{
    AddToComponent(amount, g);
}

void Pixel::AddBlue(float amount)
{
    AddToComponent(amount, b);
}

void Pixel::AddAlpha(float amount)
{
    AddToComponent(amount, a);
}

void Pixel::AddToComponent(float amount, unsigned char& component)
{
    component = static_cast<char>(Clamp(
        static_cast<float>(component) + amount, 0.0f, 255.0f));
}

