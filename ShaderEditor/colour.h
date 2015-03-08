////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - colour.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

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
};

/**
* Single pixel (texel) for a texture
*/
class Pixel
{
public:

    union
    {
        unsigned int color;
        struct
        {
            unsigned char r, g, b, a;
        };
    };

    /**
    * Constructor
    */
    Pixel();

    /**
    * Get the color component as a float 
    */
    float Red() const;
    float Green() const;
    float Blue() const;
    float Alpha() const;

    /**
    * Set the color component as a float 
    */
    void Red(float red);
    void Green(float green);
    void Blue(float blue);
    void Alpha(float alpha);

    /**
    * Add to a particular colour component
    * Prevents going beyond the bounds of 0->255
    * @param amount the amount to add to the component
    */
    void AddRed(float amount);
    void AddGreen(float amount);
    void AddBlue(float amount);
    void AddAlpha(float amount);

private:

    /**
    * Adds an amount and prevents going beyond the bounds of 0->255 and 0.0->1.0
    * @param component the color component to add to
    * @param amount the amount to add to the component
    */
    void AddToComponent(float amount, unsigned char& component);
};
