////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - camera.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "Camera.h"
#include <AntTweakBar.h>

Camera::Camera()
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
    m_initialPos.x = 3.0f;
    m_initialPos.y = 52.0f;
    m_initialPos.z = -12.0f;
    m_pos.x = 3.0f;
    m_pos.y = 52.0f;
    m_pos.z = -12.0f;
    m_yaw = -2.0f;
    m_pitch = 0.35f;
    m_roll = 0.0f;
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

void Camera::InitialiseTweakBar(CTwBar* tweakbar, const std::string& group)
{
    TwAddVarRO(tweakbar, "Camera X", TW_TYPE_FLOAT, &m_pos.x, group.c_str());
    TwAddVarRO(tweakbar, "Camera Y", TW_TYPE_FLOAT, &m_pos.y, group.c_str());
    TwAddVarRO(tweakbar, "Camera Z", TW_TYPE_FLOAT, &m_pos.z, group.c_str());
    TwAddVarRO(tweakbar, "Pitch", TW_TYPE_FLOAT, &m_pitch, group.c_str());
    TwAddVarRO(tweakbar, "Yaw", TW_TYPE_FLOAT, &m_yaw, group.c_str());
    TwAddVarRO(tweakbar, "Roll", TW_TYPE_FLOAT, &m_roll, group.c_str());
}