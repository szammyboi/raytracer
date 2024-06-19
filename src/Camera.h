#pragma once

#include <glm/glm.hpp>

struct Ray
{
    glm::vec3 origin;
    glm::vec3 direction;

    glm::vec3 At(float t) const
    {
        return origin + t*direction;
    }
};

enum class CameraType
{
    Orthographic = 0,
    Perspective
};

class Camera
{
public:
    Camera() = default;
    ~Camera() = default;

    Ray GenerateRay(float x, float y);
    void PointTo(glm::vec3 point);

public:
    glm::vec3 m_Position = {0.0f, 0.0f, -2.0f};
    float m_AspectRatio = 1.0f;
    float m_Distance = 1.0f;
    float m_ViewportScale = 1.0f;
    CameraType m_Type = CameraType::Perspective;
private:  
    glm::vec3 m_Right = {1.0f, 0.0f, 0.0f};
    glm::vec3 m_Up = {0.0f, 1.0f, 0.0f};
    glm::vec3 m_LookAt = {0.0f, 0.0f, -1.0f};
    glm::vec3 m_WorldUp = {0.0f, 1.0f, 0.0f};
};