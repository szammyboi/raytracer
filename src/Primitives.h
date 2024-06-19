#pragma once

#include "Camera.h"

// Note: there are a lot of public variables being accessed which is not ideal.  
// This was done to quickly and easily experiment without getting wrapped up in object types and getter/setter functions. 

struct Material
{
    glm::vec3 Color = {1.0f, 1.0f, 1.0f};
    float Ambient = 0.1f;
    float Diffuse = 1.0f;
    float Specular = 0.6f;
    int Shininess = 35;
    float Reflectivity = 0.0f;

    Material() = default;
	Material(const Material&) = default;
};

class Object
{
public:
    Object() = default;
    ~Object() = default;

    virtual void RenderEditUI() = 0;
public:
    Material m_Material;
    glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
    float m_Scale = 1.0f;
};

class Primitive : public Object
{
public:
    Primitive() = default;
    ~Primitive() = default; 

    virtual float Hit(Ray ray) = 0;
    virtual glm::vec3 NormalAt(glm::vec3 point) = 0;
    virtual void RenderEditUI() = 0;
};

class Sphere : public Primitive
{
public:
    Sphere() = default;
    ~Sphere() = default;

    float Hit(Ray ray) override;
    glm::vec3 NormalAt(glm::vec3 point) override;
    void RenderEditUI() override;
};

class Triangle : public Primitive
{
public:
    Triangle() = default;
    ~Triangle() = default;

    float Hit(Ray ray) override;
    glm::vec3 NormalAt(glm::vec3 point) override;
    void RenderEditUI() override;

    bool PointIn(Ray ray, glm::vec3 point);
public:
    glm::vec3 m_RelativePoints[3] = 
    {
        glm::vec3(1.0, 0.0, 0.0),
        glm::vec3(-1.0, 0.0, 0.0),
        glm::vec3(0.0, 0.0, 3.0)
    };
};