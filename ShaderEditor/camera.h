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
    * @return Whether the application has moved the camera this tick
    * @note used by the gui to deterimine if it should update
    */
    bool HasCameraMoved() const;

    /**
    * @return the component of the camera
    */
    float GetCamera(Component component) const;

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
    * Toggles whether the camera moves forward automatically
    */
    void ToggleAutoMove();

private:

    Matrix m_world;                     ///< World Matrix for the camera
    Float3 m_initialPos;                ///< Camera initial position in world space
    Float3 m_position;                  ///< Camera position in world space
    Float3 m_target;                    ///< Camera Look target
    Float3 m_rotation;                  ///< Rotation in radians (yaw, pitch, roll)
    bool m_autoMove = false;            ///< Whether to automatically move the camera
    bool m_cameraNeedsUpdate = false;   ///< Whether the camera requires updating or not
    bool m_hasCameraMoved = false;      ///< Whether the application has updated the camera this tick
};