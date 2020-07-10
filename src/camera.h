////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - camera.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "float3.h"
#include "matrix.h"

struct BoundingArea;

/**
* Maya styled camera class 
*/
class Camera
{
public:

    /**
    * Constructor
    */
    Camera();

    /**
    * Updates the view matrix if needed
    * @param deltatime The time passed between ticks
    * @return whether the camera was updated
    */
    bool Update(float deltatime);

    /**
    * Resets the camera to the initial state
    */
    void Reset();

    /**
    * Generates rotation for the camera around the camera position
    * @param mouseDir The direction the mouse has moved
    * @param speed The speed for the movement (negative for reverse)
    */
    void Rotate(const Float2& mouseDir, float speed);

    /**
    * Translates the camera on its forward vector
    */
    void Forward(float value);

    /**
    * Translates the camera on its up vector
    */
    void Up(float value);

    /**
    * Translates the camera on its right vector
    */
    void Right(float value);

    /**
    * @return the camera world matrix
    */
    const Matrix& GetWorld() const { return m_world; }

    /**
    * @return the position of the camera
    */
    const Float3& Position() const { return m_position; }

    /**
    * @return the position of the camera
    */
    const Float3& Rotation() const { return m_rotation; }
    
    /**
    * Toggles whether the camera moves forward automatically
    */
    void ToggleAutoMove();

    /**
    * @return the bounding area in front of the area
    */
    const BoundingArea& GetBounds() const;

    /**
    * Sets the forward speed of the camera
    */
    void SetForwardSpeed(float speed);

    /**
    * Sets the rotation speed of the camera
    */
    void SetRotationSpeed(float speed);

    /**
    * @return the forward speed of the camera
    */
    float GetForwardSpeed() const;

    /**
    * @return the rotation speed of the camera
    */
    float GetRotationSpeed() const;

private:

    /**
    * Determines the bounding area in front of the camera
    */
    void GenerateBounds();

private:

    Matrix m_world;                         ///< World Matrix for the camera
    Float3 m_initialPos;                    ///< Camera initial position in world space
    Float3 m_position;                      ///< Camera position in world space
    Float3 m_target;                        ///< Camera Look target
    Float3 m_rotation;                      ///< Rotation in radians (yaw, pitch, roll)
    Float2 m_heightBounds;                  ///< The min/max height the camera can move
    float m_forwardSpeed;                   ///< The speed to move the camera forward
    float m_rotationSpeed;                  ///< The speed to rotate the camera
    bool m_autoMove = false;                ///< Whether to automatically move the camera
    bool m_cameraNeedsUpdate = false;       ///< Whether the camera requires updating or not
    std::unique_ptr<BoundingArea> m_bounds; ///< The bounding area in front of the area
};
