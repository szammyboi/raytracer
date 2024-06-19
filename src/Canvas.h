#pragma once

#include "Shader.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <vector>

struct RGBColor
{
    union
    {
        unsigned char data[3] = {255, 255, 255};
        struct
        {
            unsigned char r;
            unsigned char g;
            unsigned char b;
        };
    };
};

const float CANVAS_VERTICES [] = {
    // positions          // colors           // texture coords
     1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
     1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
    -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
    -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
};

const unsigned int CANVAS_INDICES [] = {  
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

class Canvas
{
public:
    Canvas() = delete;
    Canvas(int width, int height);
    ~Canvas() = default;

    void Resize(int width, int height);
    void UpdateTexture();
    void Render(GLFWwindow* window);
    std::pair<int, int> GetDimensions();
    std::vector<RGBColor>& GetPixels();

    void WriteToFile(const std::string& filename);
private:
    unsigned m_VBO, m_VAO, m_EBO, m_Texture;    
    int m_Width;
    int m_Height;
    std::vector<RGBColor> m_Pixels;
    const Shader m_Shader;
};