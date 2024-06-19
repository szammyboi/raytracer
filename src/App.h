#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "Canvas.h"
#include "Camera.h"
#include "Primitives.h"
#include "Lights.h"

void ResizeCallback(GLFWwindow* window, int width, int height);

class Application
{
public:
    void Run();
    void Init();
    void InitImGui();
    void CreateWindow();

    Canvas* GetCanvas();

    std::pair<float, Primitive*> ClosestEntity(Ray ray);
    glm::vec3 CalculateLight(Ray ray, glm::vec3 point, glm::vec3 normal, Material material);

    glm::vec3 CastRay(Ray ray, int depth);

    void StartOpenGLRender();
    void EndOpenGLRender();
    void RenderUI();
    void RenderFrame();

    void RenderVideo();
    void AnimateCamera(float time);
private:
    GLFWwindow* m_Window;
    Canvas* m_Canvas;

    Camera m_Camera;
    Camera m_PreviousCamera;

    glm::vec3 m_Background = {0, 0, 0};
    bool m_LiveEditor = true;
    
    bool m_Animating = false;
    int m_Fps = 24;
    int m_VideoLength = 10;

    std::vector<Object*> m_Objects; 
    std::vector<Light*> m_Lights; 

    int m_MaxThreads = 4;
};