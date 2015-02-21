////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - camera.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "Camera.h"

Camera::Camera()
{
    Reset();
}

void Camera::SetCamera(Component component, float value)
{
    m_cameraNeedsUpdate = true;
    switch (component)
    {
    case POSITION_X:
        m_position.x = value;
        break;
    case POSITION_Y:
        m_position.y = value;
        break;
    case POSITION_Z:
        m_position.z = value;
        break;
    case ROTATION_PITCH:
        m_rotation.x = value;
        break;
    case ROTATION_YAW:
        m_rotation.y = value;
        break;
    case ROTATION_ROLL:
        m_rotation.z = value;
        break;
    }
}

float Camera::GetCamera(Component component) const
{
    switch (component)
    {
    case POSITION_X:
        return m_position.x;
    case POSITION_Y:
        return m_position.y;
    case POSITION_Z:
        return m_position.z;
    case ROTATION_PITCH:
        return m_rotation.x;
    case ROTATION_YAW:
        return m_rotation.y;
    case ROTATION_ROLL:
        return m_rotation.z;
    }
    return 0.0f;
}

bool Camera::HasMouseRotatedCamera() const
{
    return m_mouseRotatedCamera;
}

void Camera::RotateCamera(const Float2& mouseDir, bool isMouseDown, float speed)
{
    if(isMouseDown)
    {
        if(mouseDir.x != 0.0f)
        {
            m_rotation.y += mouseDir.x < 0.0f ? speed : -speed;
            m_cameraNeedsUpdate = true;
            m_mouseRotatedCamera = true;
        }

        if(mouseDir.y != 0.0f)
        {
            m_rotation.x += mouseDir.y < 0.0f ? speed : -speed;
            m_cameraNeedsUpdate = true;
            m_mouseRotatedCamera = true;
        }
    }
}

void Camera::Reset()
{
    m_mouseRotatedCamera = false;
    m_cameraNeedsUpdate = true;
    m_initialPos.x = 64.0f; //527.0f;
    m_initialPos.y = 1.0f;
    m_initialPos.z = 3.0f; //-6.0f;
    m_position = m_initialPos;
    m_rotation.y = 5.0f; 
}

void Camera::Update()
{
    m_cameraNeedsUpdate = false;
    m_mouseRotatedCamera = false;

    m_world.MakeIdentity();
    m_world.SetPosition(m_position);

    Matrix pitch = Matrix::CreateRotateX(m_rotation.x);
    Matrix yaw = Matrix::CreateRotateY(m_rotation.y);
    Matrix roll = Matrix::CreateRotateZ(m_rotation.z);
    m_world *= roll * yaw * pitch;
}