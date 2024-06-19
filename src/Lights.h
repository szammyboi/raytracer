#pragma once

#include <glm/glm.hpp>

class Light
{
public:
    Light() = default;
    ~Light() = default;

    virtual void RenderEditUI() = 0;
public:
    glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
    glm::vec3 m_Color = {1.0f, 1.0f, 1.0f};
    float m_Intensity = 1.0f;
};

class PointLight : public Light
{
public:
    PointLight() = default;
    ~PointLight() = default;

    void RenderEditUI() override;
};