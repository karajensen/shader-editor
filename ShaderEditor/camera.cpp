////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - camera.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "Camera.h"
#include "renderdata.h"

Camera::Camera() :
    m_initialPos(15.0f, 1.0f, 3.0f),
    m_rotation(0.0f, DegToRad(-75.0f), 0.0f),
    m_heightBounds(-20.0f, 2000.0f),
    m_forwardSpeed(45.0f),
    m_rotationSpeed(2.0f),
    m_bounds(std::make_unique<BoundingArea>())
{
    Reset();
}

void Camera::Forward(float value)
{
    m_cameraNeedsUpdate = true;
    m_position -= m_world.Forward() * value * m_forwardSpeed;
}

void Camera::Up(float value)
{
    m_cameraNeedsUpdate = true;
    m_position += m_world.Up() * value * m_forwardSpeed;
}

void Camera::Right(float value)
{
    m_cameraNeedsUpdate = true;
    m_position += m_world.Right() * value * m_forwardSpeed;
}

void Camera::Rotate(const Float2& mouseDir, float speed)
{
    speed *= m_rotationSpeed;

    if(mouseDir.x != 0.0f)
    {
        m_rotation.y += mouseDir.x < 0.0f ? speed : -speed;
        m_cameraNeedsUpdate = true;
    }

    if(mouseDir.y != 0.0f)
    {
        m_rotation.x += mouseDir.y < 0.0f ? speed : -speed;
        m_cameraNeedsUpdate = true;
    }
}

void Camera::Reset()
{
    m_cameraNeedsUpdate = true;
    m_position = m_initialPos;
}

void Camera::ToggleAutoMove()
{
    m_autoMove = !m_autoMove;
}

bool Camera::Update(float deltatime)
{
    if (m_autoMove)
    {
        Forward(deltatime);
    }

    if (m_cameraNeedsUpdate)
    {
        m_cameraNeedsUpdate = false;

        m_position.y = Clamp(m_position.y, 
            m_heightBounds.x, m_heightBounds.y);

        m_world.MakeIdentity();
        m_world.SetPosition(m_position);

        Matrix pitch = Matrix::CreateRotateX(m_rotation.x);
        Matrix yaw = Matrix::CreateRotateY(m_rotation.y);
        Matrix roll = Matrix::CreateRotateZ(m_rotation.z);
        m_world *= roll * yaw * pitch;

        GenerateBounds();

        return true;
    }
    return false;
}

void Camera::GenerateBounds()
{
    // Determine the largest forward bounding area based on the far plane
    // http://www.lighthouse3d.com/tutorials/view-frustum-culling/view-frustums-shape/

    const float farHeight = 2.0f * std::tan(DegToRad(FIELD_OF_VIEW) * 0.5f) * FRUSTRUM_BOUNDS_FAR;
    const float farWidth = farHeight * RATIO;

    const Float3 forward = -m_world.Forward();
    const Float3 nearPoint = m_position + (forward * FRUSTRUM_BOUNDS_NEAR);
    const Float3 farPoint = m_position + (forward * FRUSTRUM_BOUNDS_FAR);

    m_bounds->radius = std::max(farWidth * 0.5f, farHeight * 0.5f);
    m_bounds->center = (farPoint + nearPoint) * 0.5f;
}

const BoundingArea& Camera::GetBounds() const
{
    return *m_bounds;
}

void Camera::SetForwardSpeed(float speed)
{
    m_forwardSpeed = speed;
}

void Camera::SetRotationSpeed(float speed)
{
    m_rotationSpeed = speed;
}

float Camera::GetForwardSpeed() const
{
    return m_forwardSpeed;
}

float Camera::GetRotationSpeed() const
{
    return m_rotationSpeed;
}