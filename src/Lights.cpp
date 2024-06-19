#include "Lights.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_stdlib.h>

#include <glm/gtc/type_ptr.hpp>

void PointLight::RenderEditUI()
{
    std::string title = "Light";
    if (ImGui::CollapsingHeader(title.c_str()))
    {
        ImGui::Text("Intensity: ");
        ImGui::SliderFloat("## intensity", &m_Intensity, 0.0f, 1.0f);

        ImGui::Text("Position");
        ImGui::DragFloat3("## lpos", glm::value_ptr(m_Position), 0.005f, -10.0f, 10.0f);

        ImGui::Text("Color:");
        ImGui::ColorEdit3("## color picker", glm::value_ptr(m_Color));
    }
}