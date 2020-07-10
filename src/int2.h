////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - int2.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <cmath>

/**
* Two dimensional integer vector
*/
class Int2
{
public:

    int x = 0;
    int y = 0;

    /**
    * Constructor
    */
    Int2() = default;

    /**
    * Constructor
    * @param x/y/z The components
    */
    Int2(int X, int Y) : 
        x(X), 
        y(Y)
    {
    }
    
    /**
    * Inverts all components of the vector
    * @return A new vector from the vector inverted
    */
    Int2 operator-() const
    {
        return Int2(-x,-y);
    }
    
    /**
    * Multiplication against an integer
    * @param value Integer to multiply against
    * @return A new vector from the vector multiplied by integer
    */
    Int2 operator*(int value) const
    {
        return Int2(value*x, value*y); 
    }

    /**
    * Multiplication against an interger
    * @param value Interger to multiply against
    */
    void operator*=(int value)
    {
        x = x*value;
        y = y*value;
    }

    /**
    * Division against an interger
    * @param value Interger to divide against
    * @return A new vector from the vector divided by interger
    */
    Int2 operator/(int value) const
    {
        return Int2(x/value, y/value);
    }

    /**
    * Division against a interger
    * @param value Interger to divide against
    */
    void operator/=(int value)
    {
        x = x/value;
        y = y/value;
    }

    /**
    * Int2 addition with another vector
    * @param vec The vector to add
    * @return A new vector from the two added vectors
    */
    Int2 operator+(const Int2& vec) const
    {
        return Int2(x + vec.x, y + vec.y);
    }
    
    /**
    * Int2 addition with another vector
    * @param vec The vector to add
    */    
    void operator+=(const Int2& vec)
    {
        x = x + vec.x;
        y = y + vec.y;
    }

    /**
    * Int2 subtraction with another vector
    * @param vec The vector to subtract
    * @return A new vector from the two subtracted vectors
    */
    Int2 operator-(const Int2& vec) const
    {
        return Int2(x - vec.x, y - vec.y);
    }
    
    /**
    * Int2 subtraction with another vector
    * @param vec The vector to subtract
    */    
    void operator-=(const Int2& vec)
    {
        x = x - vec.x;
        y = y - vec.y;
    }
};
