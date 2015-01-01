////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - camera.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "common.h"

/**
* Maya styled camera class 
*/
class Camera
{
public:

    enum Component
    {
        POSITION_X,
        POSITION_Y,
        POSITION_Z,
        ROTATION_PITCH,
        ROTATION_YAW,
        ROTATION_ROLL
    };

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
    * Generates rotation for the camera around the camera position
    * @param isMouseDown Whether the mouse is pressed or not
    * @param mouseDir The direction the mouse has moved
    * @param speed The speed for the movement (negative for reverse)
    */
    void RotateCamera(const Float2& mouseDir, bool isMouseDown, float speed);

    /**
    * @return Whether the mouse has affected the camera this tick
    */
    bool HasMouseRotatedCamera() const;

    /**
    * Sets the given component of the camera
    * @param component The component to set
    * @param value The value to set the component to
    */
    void SetCamera(Component component, float value);

    /**
    * @return the component of the camera
    */
    float GetCamera(Component component) const;

    /**
    * @return the camera world matrix
    */
    const Matrix& GetWorld() const { return m_world; }

    /**
    * @return whether the camera requires an update
    */
    bool RequiresUpdate() const { return m_cameraNeedsUpdate; }

private:

    Matrix m_world;                     ///< World Matrix for the camera
    Float3 m_initialPos;                ///< Camera initial position in world space
    Float3 m_position;                  ///< Camera position in world space
    Float3 m_target;                    ///< Camera Look target
    Float3 m_rotation;                  ///< Rotation in radians (yaw, pitch, roll)
    bool m_cameraNeedsUpdate = false;   ///< Whether the camera requires updating or not
    bool m_mouseRotatedCamera = false;  ///< Whether the mouse has updated the camera this tick
};