module;
#include "Utils.h"
#include "glm/mat2x2.hpp"
#include "glm/mat3x3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

#include <map>
#include <memory>
#include <string>
#include <sstream>

module Engine.Graphics;

Shader::Shader(std::string name, const std::string& vertexPath, const std::string& fragmentPath)
    : m_Name(std::move(name)) {
    const std::string vertexShaderString = Shader::LoadFromFile(vertexPath);
    const std::string fragmentShaderString = Shader::LoadFromFile(fragmentPath);

    CreateShader(vertexShaderString, ShaderType::VERTEX);
    CreateShader(fragmentShaderString, ShaderType::FRAGMENT);
    LOG("Shader '{}' created! Vertex Shader ID: {} | Fragment Shader ID: {}", m_Name, m_VertexShader,
        m_FragmentShader);

    CreateProgram();
    LOG("Shader '{}' program created! Program ID: {}", m_Name, m_Program);
}

Shader::~Shader() {
    DeleteProgram();
    DeleteShader();
}

GLuint Shader::CreateShader(const std::string& shaderString, const ShaderType type) {
    GLuint id = 0;

    switch (type) {
        case ShaderType::VERTEX:
            m_VertexShader = id = GLCallReturn(glCreateShader(GL_VERTEX_SHADER));
            break;
        case ShaderType::FRAGMENT:
            m_FragmentShader = id = GLCallReturn(glCreateShader(GL_FRAGMENT_SHADER));
            break;
    }

    const char* shader = shaderString.c_str();
    GLCall(glShaderSource(id, 1, &shader, nullptr));
    GLCall(glCompileShader(id));

    GLint result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        GLint length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        auto* message = static_cast<GLchar*>(alloca(length * sizeof(GLchar)));
        GLCall(glGetShaderInfoLog(id, length, &length, message));

        ERROR("Failed to compile {} shader!", (type == ShaderType::VERTEX ? "vertex" : "fragment"));
        ERROR("{}", message);
    }

    return id;
}

GLuint Shader::CreateProgram() {
    m_Program = GLCallReturn(glCreateProgram());

    if (m_VertexShader != -1) {
        GLCall(glAttachShader(m_Program, m_VertexShader));
    }

    if (m_FragmentShader != -1) {
        GLCall(glAttachShader(m_Program, m_FragmentShader));
    }

    return m_Program;
}

void Shader::DeleteShader() {
    if (m_VertexShader != -1) {
        GLCall(glDeleteShader(m_VertexShader));

        m_VertexShader = -1;
    }

    if (m_FragmentShader != -1) {
        GLCall(glDeleteShader(m_FragmentShader));

        m_FragmentShader = -1;
    }
}

void Shader::DeleteProgram() {
    if (m_Program != -1) {
        if (m_VertexShader != -1) {
            GLCall(glDetachShader(m_Program, m_VertexShader));
        }

        if (m_FragmentShader != -1) {
            GLCall(glDetachShader(m_Program, m_FragmentShader));
        }

        GLCall(glDeleteProgram(m_Program));
    }
}

void Shader::SetUniform1f(const std::string& name, const float value1) {
    const GLint uniform = getUniform(name);
    if (uniform == -1)
        return;

    glUniform1f(uniform, value1);
}

void Shader::SetUniform2f(const std::string& name, const glm::vec2& value) {
    const GLint uniform = getUniform(name);
    if (uniform == -1)
        return;

    glUniform2f(uniform, value.x, value.y);
}

void Shader::SetUniform3f(const std::string& name, const glm::vec3& value) {
    const GLint uniform = getUniform(name);
    if (uniform == -1)
        return;

    glUniform3f(uniform, value.x, value.y, value.z);
}

void Shader::SetUniform4f(const std::string& name, const glm::vec4& value) {
    const GLint uniform = getUniform(name);
    if (uniform == -1)
        return;

    glUniform4f(uniform, value.x, value.y, value.z, value.w);
}

void Shader::SetUniformMatrix2fv(const std::string& name, const glm::mat2& value) {
    const GLint uniform = getUniform(name);
    if (uniform == -1)
        return;

    glUniformMatrix2fv(uniform, 1, false, &value[0][0]);
}

void Shader::SetUniformMatrix3fv(const std::string& name, const glm::mat3& value) {
    const GLint uniform = getUniform(name);
    if (uniform == -1)
        return;

    glUniformMatrix3fv(uniform, 1, false, &value[0][0]);
}

void Shader::SetUniformMatrix4fv(const std::string& name, const glm::mat4& value) {
    const GLint uniform = getUniform(name);
    if (uniform == -1)
        return;

    glUniformMatrix4fv(uniform, 1, false, &value[0][0]);
}

std::string Shader::LoadFromFile(const std::string& path) {
    std::ifstream stream;
    stream.open(path);

    std::stringstream stringStream;
    if (stream.is_open()) {
        std::string line;

        while (getline(stream, line)) {
            stringStream << line << "\n";
        }
        stream.close();
    } else return {};

    return stringStream.str();
}

GLint Shader::getUniform(const std::string& name) {
    if (m_UniformLocations.find(name) != m_UniformLocations.end())
        return m_UniformLocations[name];

    const GLint uniformLocation = GLCallReturn(glGetUniformLocation(m_Program, name.c_str()));
    m_UniformLocations[name] = uniformLocation;
    return uniformLocation;
}

std::map<std::string, std::unique_ptr<Shader>> Shader::m_Shaders;
Shader* Shader::linkedShader = nullptr;
