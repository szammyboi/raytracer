#include "Shapes.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_stdlib.h>

#include <glm/gtc/type_ptr.hpp>

Pyramid::Pyramid()
{
    Triangle* left = new Triangle();
    m_Primitives.push_back(left);

    left->m_RelativePoints[0] = {-1.0f, 0.0f, -1.0f};
    left->m_RelativePoints[1] = {-1.0f, 0.0f, 1.0f};
    left->m_RelativePoints[2] = {0.0f, 1.0f, 0.0f};

    Triangle* front = new Triangle();
    m_Primitives.push_back(front);

    front->m_RelativePoints[0] = {1.0f, 0.0f, -1.0f};
    front->m_RelativePoints[1] = {-1.0f, 0.0f, -1.0f};
    front->m_RelativePoints[2] = {0.0f, 1.0f, 0.0f};

    Triangle* right = new Triangle();
    m_Primitives.push_back(right);

    right->m_RelativePoints[0] = {1.0f, 0.0f, 1.0f};
    right->m_RelativePoints[1] = {1.0f, 0.0f, -1.0f};
    right->m_RelativePoints[2] = {0.0f, 1.0f, 0.0f};

    Triangle* back = new Triangle();
    m_Primitives.push_back(back);

    back->m_RelativePoints[0] = {-1.0f, 0.0f, 1.0f};
    back->m_RelativePoints[1] = {1.0f, 0.0f, 1.0f};
    back->m_RelativePoints[2] = {0.0f, 1.0f, 0.0f};

    Triangle* bottom1 = new Triangle();
    m_Primitives.push_back(bottom1);

    bottom1->m_RelativePoints[1] = {-1.0f, 0.0f, -1.0f};
    bottom1->m_RelativePoints[0] = {-1.0f, 0.0f, 1.0f};
    bottom1->m_RelativePoints[2] = {1.0f, 0.0f, -1.0f};

    Triangle* bottom2 = new Triangle();
    m_Primitives.push_back(bottom2);

    bottom2->m_RelativePoints[0] = {1.0f, 0.0f, -1.0f};
    bottom2->m_RelativePoints[1] = {1.0f, 0.0f, 1.0f};
    bottom2->m_RelativePoints[2] = {-1.0f, 0.0f, 1.0f};
}


void Pyramid::RenderEditUI()
{
    std::string title = "Pyramid";
    if (ImGui::CollapsingHeader(title.c_str()))
    {
        ImGui::Text("Position:");
        if (ImGui::DragFloat3("## pos", glm::value_ptr(m_Position), 0.005f, -10.0f, 10.0f))
            for (Primitive* primitive : m_Primitives)
                primitive->m_Position = m_Position;

        ImGui::Text("Scale:");
        if (ImGui::DragFloat("## scale", &m_Scale, 0.005f, -10.0f, 10.0f))
            for (Primitive* primitive : m_Primitives)
                primitive->m_Scale = m_Scale;

        ImGui::Text("Color:");
        if (ImGui::ColorEdit3("## color picker", glm::value_ptr(m_Material.Color)))
            for (Primitive* primitive : m_Primitives)
                primitive->m_Material.Color = m_Material.Color;

        ImGui::Text("Ambient:"); ImGui::SameLine();
        if (ImGui::DragFloat("## ambient", &m_Material.Ambient, 0.005f, 0.0f, 1.0f))
            for (Primitive* primitive : m_Primitives)
                primitive->m_Material.Ambient = m_Material.Ambient;

        ImGui::Text("Diffuse:"); ImGui::SameLine();
        if (ImGui::DragFloat("## diffuse", &m_Material.Diffuse, 0.005f, 0.0f, 1.0f))
            for (Primitive* primitive : m_Primitives)
                primitive->m_Material.Diffuse = m_Material.Diffuse;

        ImGui::Text("Specular:"); ImGui::SameLine();
        if (ImGui::DragFloat("## specular", &m_Material.Specular, 0.005f, 0.0f, 1.0f))
            for (Primitive* primitive : m_Primitives)
                primitive->m_Material.Specular = m_Material.Specular;

        ImGui::Text("Shininess:"); ImGui::SameLine();
        if (ImGui::DragInt("## shininess", &m_Material.Shininess, 1, 0, 100))
            for (Primitive* primitive : m_Primitives)
                primitive->m_Material.Shininess = m_Material.Shininess;

        ImGui::Text("Reflectivity:"); ImGui::SameLine();
        if (ImGui::DragFloat("## reflectivity", &m_Material.Reflectivity, 0.005f, 0.0f, 1.0f))
            for (Primitive* primitive : m_Primitives)
                primitive->m_Material.Reflectivity = m_Material.Reflectivity;
    }
}

