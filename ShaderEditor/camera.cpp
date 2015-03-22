////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - camera.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "Camera.h"

namespace
{
    const float MOVE_SPEED = 45.0f; ///< Speed the camera will translate
    const float ROT_SPEED = 2.0f;   ///< Speed the camera will rotate
}

Camera::Camera() :
    m_initialPos(15.0f, 1.0f, 3.0f),
    m_rotation(0.0f, DegToRad(-75.0f), 0.0f)
{
    Reset();
}

void Camera::Forward(float value)
{
    m_cameraNeedsUpdate = true;
    m_hasCameraMoved = true;
    m_position += m_world.Forward() * value * MOVE_SPEED;
}

void Camera::Up(float value)
{
    m_cameraNeedsUpdate = true;
    m_hasCameraMoved = true;
    m_position += m_world.Up() * value * MOVE_SPEED;
}

void Camera::Right(float value)
{
    m_cameraNeedsUpdate = true;
    m_hasCameraMoved = true;
    m_position += m_world.Right() * value * MOVE_SPEED;
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

bool Camera::HasCameraMoved() const
{
    return m_hasCameraMoved;
}

void Camera::Rotate(const Float2& mouseDir, float speed)
{
    speed *= ROT_SPEED;

    if(mouseDir.x != 0.0f)
    {
        m_rotation.y += mouseDir.x < 0.0f ? speed : -speed;
        m_cameraNeedsUpdate = true;
        m_hasCameraMoved = true;
    }

    if(mouseDir.y != 0.0f)
    {
        m_rotation.x += mouseDir.y < 0.0f ? speed : -speed;
        m_cameraNeedsUpdate = true;
        m_hasCameraMoved = true;
    }
}

void Camera::Reset()
{
    m_hasCameraMoved = true;
    m_cameraNeedsUpdate = true;
    m_position = m_initialPos;
}

void Camera::Update()
{
    m_cameraNeedsUpdate = false;
    m_hasCameraMoved = false;

    m_world.MakeIdentity();
    m_world.SetPosition(m_position);

    Matrix pitch = Matrix::CreateRotateX(m_rotation.x);
    Matrix yaw = Matrix::CreateRotateY(m_rotation.y);
    Matrix roll = Matrix::CreateRotateZ(m_rotation.z);
    m_world *= roll * yaw * pitch;
}