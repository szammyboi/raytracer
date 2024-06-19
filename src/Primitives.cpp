#include "Primitives.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_stdlib.h>

#include <glm/gtc/type_ptr.hpp>

float Sphere::Hit(Ray ray)
{
    glm::vec3 ec = ray.origin - m_Position;
    float a = glm::dot(ray.direction, ray.direction);
    float b = glm::dot(ec, ray.direction);
    float c = glm::dot(ec, ec) - pow(m_Scale, 2); 

    float discriminant = pow(b, 2) - a*c;

    if (discriminant < 0)
        return -1;
        
    float t1 = (-b + sqrt(discriminant));
    float t2 = (-b - sqrt(discriminant));
    
    if (t1 < 0 && t2 < 0)
        return -1;
    return std::min(t1, t2);
}

glm::vec3 Sphere::NormalAt(glm::vec3 point)
{
    return glm::normalize(point - m_Position);
}

void Sphere::RenderEditUI()
{
    std::string title = "Sphere";
    if (ImGui::CollapsingHeader(title.c_str()))
    {
        ImGui::Text("Radius:");
        ImGui::SliderFloat("## Radius", &m_Scale, 0.0f, 3.0f);
        
        ImGui::Text("Position:");
        ImGui::DragFloat3("## Position", glm::value_ptr(m_Position), 0.005f, -10.0f, 10.0f);

        ImGui::Text("Color");
        ImGui::ColorEdit3("## Color", glm::value_ptr(m_Material.Color));

        ImGui::Text("Ambient:"); ImGui::SameLine();
        ImGui::DragFloat("## Ambient", &m_Material.Ambient, 0.005f, 0.0f, 1.0f);

        ImGui::Text("Diffuse:"); ImGui::SameLine();
        ImGui::DragFloat("## Diffuse", &m_Material.Diffuse, 0.005f, 0.0f, 1.0f);

        ImGui::Text("Specular:"); ImGui::SameLine();
        ImGui::DragFloat("## Specular", &m_Material.Specular, 0.005f, 0.0f, 1.0f);

        ImGui::Text("Shininess:"); ImGui::SameLine();
        ImGui::DragInt("## Shininess", &m_Material.Shininess, 1, 0, 100);

        ImGui::Text("Reflectivity:"); ImGui::SameLine();
        ImGui::DragFloat("## Reflectivity", &m_Material.Reflectivity, 0.005f, 0.0f, 1.0f);
    }
}

float Triangle::Hit(Ray ray)
{   
    glm::vec3 normal = NormalAt(ray.origin);

    if (glm::dot(normal, ray.direction) > 0)
        return -1;

    glm::vec3 a = (m_Scale*m_RelativePoints[0]) + m_Position;
    glm::vec3 b = (m_Scale*m_RelativePoints[1]) + m_Position;
    glm::vec3 c = (m_Scale*m_RelativePoints[2]) + m_Position;

    float d = -1.0f * glm::dot(normal, a);


    if (glm::dot(ray.direction, normal) == 0.0f) return -1;
    float t = -((glm::dot(normal, ray.origin)+d) / glm::dot(normal, ray.direction));

    glm::vec3 point = ray.At(t);

    glm::vec3 ba = glm::normalize(b-a);
    glm::vec3 cb = glm::normalize(c-b);
    glm::vec3 ac = glm::normalize(a-c);

    if (t < 0.0f)
        return -1;

    if (!PointIn(ray, point))
        return -1;

    return t;
}

bool Triangle::PointIn(Ray ray, glm::vec3 point)
{
    glm::vec3 normal = NormalAt(ray.origin);

    glm::vec3 a = (m_Scale*m_RelativePoints[0]) + m_Position;
    glm::vec3 b = (m_Scale*m_RelativePoints[1]) + m_Position;
    glm::vec3 c = (m_Scale*m_RelativePoints[2]) + m_Position;

    glm::vec3 ba = glm::normalize(b-a);
    glm::vec3 cb = glm::normalize(c-b);
    glm::vec3 ac = glm::normalize(a-c);

    if (glm::dot(glm::cross(ba, point-a), normal) < 0.0f)
        return false;
    if (glm::dot(glm::cross(cb, point-b), normal) < 0.0f)
        return false;
    if (glm::dot(glm::cross(ac, point-c), normal) < 0.0f)
        return false;

    return true;
}

glm::vec3 Triangle::NormalAt(glm::vec3 point)
{
    glm::vec3 a = (m_Scale*m_RelativePoints[0]) + m_Position;
    glm::vec3 b = (m_Scale*m_RelativePoints[1]) + m_Position;
    glm::vec3 c = (m_Scale*m_RelativePoints[2]) + m_Position;
    
    glm::vec3 ba = glm::normalize(b-a);
    glm::vec3 ca = glm::normalize(c-a);
    glm::vec3 normal = glm::normalize(glm::cross(ba, ca));
    return normal;
}

void Triangle::RenderEditUI()
{
    std::string title = "Triangle";
    if (ImGui::CollapsingHeader(title.c_str()))
    {
        ImGui::Text("Position:");
        ImGui::DragFloat3("## Position", glm::value_ptr(m_Position), 0.005f, -10.0f, 10.0f);

        ImGui::Text("Scale:"); ImGui::SameLine();
        ImGui::DragFloat("## Scale", &m_Scale, 0.005f, 0.0f, 10.0f);

        ImGui::Text("A:");
        ImGui::DragFloat3("## A", glm::value_ptr(m_RelativePoints[0]), 0.005f, -10.0f, 10.0f);

        ImGui::Text("B:");
        ImGui::DragFloat3("## B", glm::value_ptr(m_RelativePoints[1]), 0.005f, -10.0f, 10.0f);

        ImGui::Text("C:");
        ImGui::DragFloat3("## C", glm::value_ptr(m_RelativePoints[2]), 0.005f, -10.0f, 10.0f);

        ImGui::Text("Color");
        ImGui::ColorEdit3("## color picker", glm::value_ptr(m_Material.Color));

        ImGui::Text("Ambient:"); ImGui::SameLine();
        ImGui::DragFloat("## Ambient", &m_Material.Ambient, 0.005f, 0.0f, 1.0f);

        ImGui::Text("Diffuse:"); ImGui::SameLine();
        ImGui::DragFloat("## Diffuse", &m_Material.Diffuse, 0.005f, 0.0f, 1.0f);

        ImGui::Text("Specular:"); ImGui::SameLine();
        ImGui::DragFloat("## Specular", &m_Material.Specular, 0.005f, 0.0f, 1.0f);

        ImGui::Text("Shininess:"); ImGui::SameLine();
        ImGui::DragInt("## Shininess", &m_Material.Shininess, 1, 0, 100);

        ImGui::Text("Reflectivity:"); ImGui::SameLine();
        ImGui::DragFloat("## Reflectivity", &m_Material.Reflectivity, 0.005f, 0.0f, 1.0f);
    }
}