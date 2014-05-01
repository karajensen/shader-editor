////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - camera.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "common.h"

struct CTwBar;

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
    * Updates the view matrix
    */
    void Update();

    /**
    * Resets the camera to the initial state
    */
    void Reset();

    /**
    * Generates forward movement for the camera
    * @param isMouseDown Whether the mouse is pressed or not
    * @param mouseDir The direction the mouse has moved
    * @param speed The speed for the movement (negative for reverse)
    */
    void ForwardMovement(const Float2& mouseDir, bool isMouseDown, float speed);

    /**
    * Generates side movement for the camera
    * @param isMouseDown Whether the mouse is pressed or not
    * @param mouseDir The direction the mouse has moved
    * @param speed The speed for the movement (negative for reverse)
    */
    void SideMovement(const Float2& mouseDir, bool isMouseDown, float speed);

    /**
    * Generates rotation for the camera around the world origin
    * @param isMouseDown Whether the mouse is pressed or not
    * @param mouseDir The direction the mouse has moved
    * @param speed The speed for the movement (negative for reverse)
    */
    void Rotation(const Float2& mouseDir, bool isMouseDown, float speed);

    /**
    * @return the camera world matrix
    */
    const Matrix& GetWorld() const { return m_world; }

    /**
    * @return whether the camera requires an update
    */
    bool RequiresUpdate() const { return m_cameraNeedsUpdate; }

    /**
    * Adds tweakable timer parameters to the tweak bar
    * @param tweakbar Used for runtime diagnostics
    * @param group The header to put any diagnostics under
    */
    void InitialiseTweakBar(CTwBar* tweakbar, const std::string& group);

private:

    /**
    * Rotate the camera
    * @param angle The angle to rotate in radians
    */
    void Yaw(float angle);

    /**
    * Rotate the camera
    * @param angle The angle to rotate in radians
    */
    void Pitch(float angle);

    /**
    * Rotate the camera
    * @param angle The angle to rotate in radians
    */
    void Roll(float angle);

    /**
    * Translates the camera
    * @param val The amount to move the camera
    */
    void Forward(float val);

    /**
    * Translates the camera
    * @param val The amount to move the camera
    */
    void Right(float val);

    /**
    * Translates the camera
    * @param val The amount to move the camera
    */
    void Up(float val);

    Matrix m_world;            ///< World Matrix for the camera
    bool m_cameraNeedsUpdate;  ///< Whether the camera requires updating or not
    Float3 m_initialPos;       ///< Camera initial position in world space
    Float3 m_pos;              ///< Camera position in world space
    Float3 m_target;           ///< Camera Look target
    float m_yaw;               ///< Radian amount of yaw
    float m_pitch;             ///< Radian amount of pitch
    float m_roll;              ///< Radian amount of roll
};