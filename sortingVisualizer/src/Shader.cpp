#include "Shader.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "Renderer.h"


Shader::Shader() : m_RendererID(0)
{
    string vertexShaderSource = ParseShader("res/shaders/vertex.shader").str();
    string fragmentShaderSource = ParseShader("res/shaders/fragment.shader").str();

    m_RendererID = CreateShader(vertexShaderSource, fragmentShaderSource);
}


Shader::~Shader()
{
    glDeleteProgram(m_RendererID);
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::UnBind() const
{
    GLCall(glUseProgram(0));
}

int Shader::GetUniformLocation(const string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
    {
        return m_UniformLocationCache[name];
    }
    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
    {
        cout << "Warning: uniform " << name << "doesn't exist" << endl;
    }
    m_UniformLocationCache[name] = location;
    return location;
}

void Shader::SetUniform4f(const string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const string& name, const glm::mat4 matrix)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

void Shader::SetUniform1i(const string& name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

stringstream Shader::ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);
    std::string line;
    std::stringstream ss;
    while (getline(stream, line))
    {
        ss << line << "\n";
    }
    return ss;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    GLCall(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr);)
    GLCall(glCompileShader(id);)

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result);)
    if (result == GL_FALSE)
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);)
        char* message = (char*)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message);)
        std::cout << "Failed to compile " << std::endl;
        std::cout << message << std::endl;
        GLCall(glDeleteShader(id);)
        return 0;
    }
    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    GLCall(unsigned int program = glCreateProgram();)
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs);)
    GLCall(glAttachShader(program, fs);)
    GLCall(glLinkProgram(program);)
    GLCall(glValidateProgram(program);)

    GLCall(glDeleteShader(vs);)
    GLCall(glDeleteShader(fs);)

    return program;
}

