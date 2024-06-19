#pragma once

// This shader class is overkill for this project but it is just code I had already written
// for a different OpenGL project

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

class Shader
{
public:
    Shader(const std::string& vertex_path, const std::string& fragment_path);
    ~Shader() = default;

    void Use() const;
    void SetUniform1f(const std::string& uniform, float x);
    void SetUniform3f(const std::string& uniform, float x, float y, float z);
    void SetUniform4f(const std::string& uniform, float x, float y, float z, float a);

    void SetUniform1i(const std::string& uniform, int x);

    void SetUniformMatrix4f(const std::string& uniform, glm::mat4 matrix);
private:
    std::string ReadFromFile(const std::string& path);
private:
    unsigned m_Program;
    std::string m_VertexSource;
    std::string m_FragmentSource;
};