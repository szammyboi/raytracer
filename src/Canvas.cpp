#include "Canvas.h"

#include <stdexcept>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

Canvas::Canvas(int width, int height)
    : m_Shader("shaders/raytracing.vert", "shaders/raytracing.frag"), m_Width(width), m_Height(height)
{
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
    glGenVertexArrays(1, &m_VAO);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CANVAS_VERTICES), CANVAS_VERTICES, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(CANVAS_INDICES), CANVAS_INDICES, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glGenTextures(1, &m_Texture);
    glBindTexture(GL_TEXTURE_2D, m_Texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    m_Pixels.resize(width*height);
}

void Canvas::Resize(int width, int height)
{
    // Keep width and height in powers of two
    // https://stackoverflow.com/questions/466204/rounding-up-to-next-power-of-2
    width--;
    width |= width >> 1;
    width |= width >> 2;
    width |= width >> 4;
    width |= width >> 8;
    width |= width >> 16;
    width++;

    height--;
    height |= height >> 1;
    height |= height >> 2;
    height |= height >> 4;
    height |= height >> 8;
    height |= height >> 16;
    height++;

    m_Pixels.clear();
    m_Pixels.resize(width*height);

    m_Width = width;
    m_Height = height;

    
}

void Canvas::UpdateTexture()
{
    if (&m_Pixels[0])
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_Pixels.data());
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else throw std::runtime_error("Failed to load texture!");
}

void Canvas::Render(GLFWwindow* window)
{
    glBindTexture(GL_TEXTURE_2D, m_Texture);
    m_Shader.Use();

    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

std::vector<RGBColor>& Canvas::GetPixels()
{
    return m_Pixels;
}

std::pair<int, int> Canvas::GetDimensions()
{
    return {m_Width, m_Height};
}

#include <iostream>
void Canvas::WriteToFile(const std::string& filename)
{
    int channels = 3;
    RGBColor* const data = m_Pixels.data();

    int result = 0;

    stbi_flip_vertically_on_write(true);
    result = stbi_write_png(filename.c_str(), m_Width, m_Height, channels, data, m_Width*channels); 
}