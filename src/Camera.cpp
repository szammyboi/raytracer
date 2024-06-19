#include "Camera.h"

void Camera::PointTo(glm::vec3 point)
{
    m_LookAt = -glm::normalize(glm::vec3(0.0f) - m_Position);
    m_Right = glm::cross(m_WorldUp, -m_LookAt);
    m_Up = glm::cross(m_Right, m_LookAt);
}


Ray Camera::GenerateRay(float x, float y)
{   
    // -viewPortScale to viewPortScale
    x = (2.0f*x - 1.0f)*m_ViewportScale;
    y = ((1.0f - 2.0f*y)*-1.0f)*m_ViewportScale;

    if (m_AspectRatio > 1.0f)
        x *= m_AspectRatio;
    else
        y *= 1/m_AspectRatio;

    Ray pixel_ray;

    if (m_Type == CameraType::Perspective)
    {
        pixel_ray.origin = m_Position;
        pixel_ray.direction = glm::normalize(-m_Distance*m_LookAt + m_Right*x + m_Up*y);
    }
    else
    {
        pixel_ray.origin = m_Position + m_Right*x + m_Up*y;
        pixel_ray.direction = glm::normalize(-1.0f * m_Distance*m_LookAt);
    }
    return pixel_ray;
}