#include "App.h"

#include "Shapes.h"

#include <vector>
#include <stdexcept>
#include <thread>
#include <sstream>
#include <iomanip>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_stdlib.h>

const int DEFAULT_WIDTH = 256;
const int DEFAULT_HEIGHT = 128;

void ResizeCallback(GLFWwindow* window, int width, int height)
{
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));

    glViewport(0, 0, width, height);
}

void KeyboardCallback(GLFWwindow* window)
{
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
}

Canvas* Application::GetCanvas()
{
    return m_Canvas;
}

void Application::Init()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    CreateWindow();
    glfwMakeContextCurrent(m_Window);

    glfwSetWindowAspectRatio(m_Window, 2, 1);
    m_Camera.m_AspectRatio = static_cast<float>(2)/static_cast<float>(1);
    glfwSetWindowUserPointer(m_Window, this);
    glfwSetFramebufferSizeCallback(m_Window, ResizeCallback);

    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0)
        throw std::runtime_error("Failed to initialize OpenGL context!");

    InitImGui();

    m_Canvas = new Canvas(DEFAULT_WIDTH, DEFAULT_HEIGHT);   
}

void Application::InitImGui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    const char* glsl_version = "#version 330";
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

void Application::CreateWindow()
{
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    GLFWvidmode videoMode = *glfwGetVideoMode(monitor); 

    m_Window = glfwCreateWindow(2048, 1024, "Raytracing", NULL, NULL);
    if (m_Window == NULL)
    {
        throw std::runtime_error("Failed to create GLFW window!");
        glfwTerminate();
    }
}

void Application::Run()
{
    while (!glfwWindowShouldClose(m_Window))
    {
        StartOpenGLRender();
            if(m_LiveEditor)
                RenderFrame();
            m_Canvas->Render(m_Window);
            RenderUI();
        EndOpenGLRender();
    }
}

void Application::StartOpenGLRender()
{
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Application::EndOpenGLRender()
{
    glfwSwapBuffers(m_Window);
    glfwPollEvents();
}

void Application::RenderUI()
{
    int w, h;
    glfwGetFramebufferSize(m_Window, &w, &h);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();
    ImGui::SetNextWindowSize(ImVec2(0, h));
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    
    ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::SetWindowCollapsed(true, ImGuiCond_Once);

        ImGui::Text("Live Editor:"); ImGui::SameLine();
        if(ImGui::Checkbox("## live ", &m_LiveEditor))
        {
            if (m_LiveEditor) 
            {
                m_Canvas->Resize(256, 128);
                m_Camera.m_AspectRatio = 256.0f/128.0f;
            }
            else 
            {
                m_Canvas->Resize(2048, 1024);
                m_Camera.m_AspectRatio = 2048.0f/1024.0f;
                RenderFrame();
            }
        }

        if (!m_LiveEditor)
        {   
            ImGui::Text("FPS:");
            if(ImGui::DragInt("## fps", &m_Fps));

            ImGui::Text("Video Length:");
            if(ImGui::DragInt("## length", &m_VideoLength));
            
            if(ImGui::Button("Render Video"))
            {
                RenderVideo();
            }

            if(ImGui::Button("Render Frame"))
            {
                RenderFrame();
            }
        }
        else
        {
            ImGui::Text("Animate:"); ImGui::SameLine();
            if(ImGui::Checkbox("## animating ", &m_Animating))
            {
                if (m_Animating)
                {
                    m_PreviousCamera = m_Camera;
                } else 
                {
                    m_Camera = m_PreviousCamera;
                }
            }
        }

        ImGui::Text("Color:");
        ImGui::ColorEdit3("## bg picker", glm::value_ptr(m_Background));

        if (ImGui::CollapsingHeader("Camera"))
        {
            const char* camera_options[] = { "Perspective", "Orthographic"};
            static int current_type = 0;
            ImGui::Text("Camera Type:");
            if(ImGui::Combo("## Camera Type", &current_type, camera_options, IM_ARRAYSIZE(camera_options)))
            {
                if (current_type == 0)
                    m_Camera.m_Type = CameraType::Perspective;
                if (current_type == 1)
                    m_Camera.m_Type = CameraType::Orthographic;
            }

            ImGui::Text("Camera D:");
            if(ImGui::DragFloat("## D", &m_Camera.m_Distance, 0.005f));

            ImGui::Text("View Scale:");
            if(ImGui::DragFloat("## View Scale", &m_Camera.m_ViewportScale, 0.005f));

            ImGui::Text("Camera X:");
            if(ImGui::DragFloat("## X", &m_Camera.m_Position.x, 0.005f));

            ImGui::Text("Camera Y:");
            if(ImGui::DragFloat("## Y", &m_Camera.m_Position.y, 0.005f));

            ImGui::Text("Camera Z:");
            if(ImGui::DragFloat("## Z", &m_Camera.m_Position.z, 0.005f));
        }

        ImGui::Separator();

        if(ImGui::Button("Add Sphere"))
        {
            Sphere* newSphere = new Sphere();
            m_Objects.push_back(newSphere);
        }

        if(ImGui::Button("Add Triangle"))
        {
            Triangle* newTriangle = new Triangle();
            m_Objects.push_back(newTriangle);
        }

        if(ImGui::Button("Add Pyramid"))
        {
            Pyramid* newPyramid = new Pyramid();
            m_Objects.push_back(newPyramid);
        }

        int id = 0;
        for (Object* object : m_Objects)
        {
            ImGui::PushID(id);
            object->RenderEditUI();
            ImGui::PopID();
            id++;
        }
        
        ImGui::Separator();

        if(ImGui::Button("Add Light"))
        {
            PointLight* newPointLight = new PointLight();
            m_Lights.push_back(newPointLight);
        }

        int j = id-1;
        for (Light* light : m_Lights)
        {
            ImGui::PushID(id);
            light->RenderEditUI();
            ImGui::PopID();
            id++;
        }

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

std::pair<float, Primitive*> ClosestHelper(Primitive* current, Ray ray, float smallestT, Primitive* closestEntity)
{
    float t = current->Hit(ray);

    if (t > 0.0f && (t < smallestT || smallestT == 0))
        return {t, current};
    return {smallestT, closestEntity};
}

std::pair<float, Primitive*> Application::ClosestEntity(Ray ray)
{
    float smallestT = 0;
    Primitive* closestEntity;

    for (Object* object : m_Objects)
    {   
        Primitive* primitive = dynamic_cast<Primitive*>(object);
        if (primitive == nullptr) 
        {
            Shape* shape = dynamic_cast<Shape*>(object);
            if (shape == nullptr) continue;

            for (Primitive* shapePrimitive : shape->m_Primitives)
                std::tie(smallestT, closestEntity) = ClosestHelper(shapePrimitive, ray, smallestT, closestEntity);

            continue;
        }

        std::tie(smallestT, closestEntity) = ClosestHelper(primitive, ray, smallestT, closestEntity);
    }

    return {smallestT, closestEntity};
}

glm::vec3 Application::CalculateLight(Ray ray, glm::vec3 point, glm::vec3 normal, Material material)
{
    glm::vec3 finalLight(0.0f);

    for (Light* light : m_Lights)
    {
        glm::vec3 contribution(0.0f);

        glm::vec3 lightDir = -1.0f * glm::normalize(point - light->m_Position);

        Ray lightPath;
        lightPath.origin = point;
        lightPath.direction = lightDir;

        auto [t, closestEntity] = ClosestEntity(lightPath);
        
        if (t > 0.001f) continue;


        // Add diffuse shading to this light's contribution
        float nl =  glm::dot(lightDir, normal);
        if (nl > 0)
            contribution += light->m_Intensity * glm::vec3(1.0f) * material.Diffuse * nl;


        // full blinn phong
        glm::vec3 r = glm::normalize(2.0f * normal * glm::dot(lightDir, normal) - lightDir);
        float rdv = glm::dot(r, glm::normalize(-ray.direction));

        // Add specular shading to this light's contribution
        if (rdv > 0)
            contribution += light->m_Intensity * glm::vec3(1.0f) * material.Specular * static_cast<float>(pow(rdv, material.Shininess));
            
        // Use this instead for the blinn-phong approximation
        // glm::vec3 bisector = glm::normalize(glm::normalize(-ray.direction) + lightDir);    
        // contribution += details.Intensity * glm::vec3(1.0f) * material.Specular * (float)pow(std::max(0.0f, glm::dot(normal, bisector)), material.Shininess);

        // Bound light to 0-1
        contribution.r = std::max(0.0f, std::min(1.0f, contribution.r));
        contribution.g = std::max(0.0f, std::min(1.0f, contribution.g));
        contribution.b = std::max(0.0f, std::min(1.0f, contribution.b));

        // Account for the light's color
        contribution *= light->m_Color;

        // Add this light's contribution to the total light at this point
        finalLight += contribution;
    }
    

    // Bound final light to ambient-1.0
    finalLight.r = std::max(material.Ambient, std::min(1.0f, finalLight.r));
    finalLight.g = std::max(material.Ambient, std::min(1.0f, finalLight.g));
    finalLight.b = std::max(material.Ambient, std::min(1.0f, finalLight.b));
    
    return finalLight;
}


// Casts a ray through the scene and calculates its color
// If it is reflective, bounce the ray off the object and go find that new color
// All the reflected colors are added up as the recursive calls end
glm::vec3 Application::CastRay(Ray ray, int depth)
{
    auto [t, closestEntity] = ClosestEntity(ray);

    // Set this to vec(0.0f) if the background shouldn't be reflected
    if (t <= 0.0f)
        return m_Background;

    glm::vec3 point = ray.At(t);
    glm::vec3 normal = closestEntity->NormalAt(point);

    glm::vec3 base_color = closestEntity->m_Material.Color * CalculateLight(ray, point, normal, closestEntity->m_Material);

    if (depth <= 0 || closestEntity->m_Material.Reflectivity <= 0.0f)
        return base_color;

    Ray reflected;
    reflected.origin = point;
    reflected.direction = 2.0f * normal * glm::dot(-ray.direction, normal) + ray.direction;

    return base_color * (1.0f - closestEntity->m_Material.Reflectivity) + CastRay(reflected, depth-1) * closestEntity->m_Material.Reflectivity;
}

void Application::RenderVideo()
{
    m_Animating = false;
    m_PreviousCamera = m_Camera;
    

    for (int i = 0; i <= m_Fps*m_VideoLength; i++)
    {
        AnimateCamera(i/static_cast<float>(m_Fps));

        std::stringstream ss;
        ss << std::setw(4) << std::setfill('0') << i;
        std::string s = ss.str();

        RenderFrame();
        std::string name = "video/frame-" + s + ".png";
        m_Canvas->WriteToFile(name);
    }

    m_Camera = m_PreviousCamera;
    RenderFrame();
}

void Application::AnimateCamera(float time)
{
    m_Camera.m_Position.x = sin(time)*m_PreviousCamera.m_Position.z;
    m_Camera.m_Position.z = cos(time)*m_PreviousCamera.m_Position.z;
    m_Camera.PointTo(glm::vec3(0.0f));
}

void Application::RenderFrame()
{
    if (m_Animating) AnimateCamera(glfwGetTime());
    auto& pixels = m_Canvas->GetPixels();
    auto [width, height] = m_Canvas->GetDimensions();


    // Split portions of the image rendering across different threads
    std::vector<std::thread> threads;

    int split = std::max(2, (int)std::thread::hardware_concurrency());
    for (int t = 1; t <= split; t++)
    {
        threads.push_back(std::thread([&, this, t]() mutable {
            int start = (t-1)*(height/split);
            int end = (t == split) ? height : t*(height/split);
            for (int i = start; i < end; i++)
            {
                for (int j = 0; j < width; j++)
                {

                    int idx = (i * width + j) ;
                    float u = (j + 0.5f) / static_cast<float>(width);
                    float v = (i + 0.5f) / static_cast<float>(height);

                    
                    pixels[idx].r = m_Background.r*255;
                    pixels[idx].g = m_Background.g*255;
                    pixels[idx].b = m_Background.b*255;

                    Ray ray = m_Camera.GenerateRay(u, v);
                    glm::vec3 color = CastRay(ray, 3);

                    if (color == glm::vec3(0.0f))
                        continue;

                    pixels[idx].r = color.r*255;
                    pixels[idx].g = color.g*255;
                    pixels[idx].b = color.b*255;
                    
                }
            }
        }));
    }

    for (auto& thread : threads)
        thread.join();

    m_Canvas->UpdateTexture();
}