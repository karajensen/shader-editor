////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - camera.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "Camera.h"

Camera::Camera() : 
    m_cameraNeedsUpdate(true),
    m_initialPos(0.0f, 0.0f, 10.0f),
    m_pos(0.0f, 0.0f, 0.0f),
    m_yaw(0.0f),     
    m_pitch(0.0f),
    m_roll(0.0f)
{
    Reset();
}

void Camera::ForwardMovement(const Float2& mouseDir, bool isMouseDown, float speed)
{
    if(isMouseDown)
    {
        Forward(speed*mouseDir.y);
        m_cameraNeedsUpdate = true;
    }
}

void Camera::SideMovement(const Float2& mouseDir, bool isMouseDown, float speed)
{
    if(isMouseDown)
    {
        Up(-speed*mouseDir.y);
        Right(speed*mouseDir.x);
        m_cameraNeedsUpdate = true;
    }
}

void Camera::Rotation(const Float2& mouseDir, bool isMouseDown, float speed)
{
    if(isMouseDown)
    {
        if(mouseDir.x != 0.0f)
        {
            Yaw(mouseDir.x < 0.0f ? speed : -speed);
            m_cameraNeedsUpdate = true;
        }

        if(mouseDir.y != 0.0f)
        {
            Pitch(mouseDir.y < 0.0f ? speed : -speed);
            m_cameraNeedsUpdate = true;
        }
    }
}

void Camera::Forward(float val)
{
    m_pos.z += val;
}

void Camera::Right(float val)
{
    m_pos.x += val;
}

void Camera::Up(float val)
{
    m_pos.y += val;
}

void Camera::Yaw(float angle)
{
    m_yaw += angle;
}

void Camera::Pitch(float angle)
{
    m_pitch += angle;
}

void Camera::Roll(float angle)
{
    m_roll += angle;
}

void Camera::Reset()
{
    m_cameraNeedsUpdate = true;
    m_pos = m_initialPos; 
    m_yaw = 0;
    m_roll = 0;
    m_pitch = 0;
}

void Camera::Update()
{
    m_cameraNeedsUpdate = false;

    m_world.MakeIdentity();
    m_world.SetPosition(m_pos);

    Matrix pitch = Matrix::CreateRotateX(m_pitch);
    Matrix yaw = Matrix::CreateRotateY(m_yaw);
    Matrix roll = Matrix::CreateRotateZ(m_roll);
    m_world *= roll * yaw * pitch;
}