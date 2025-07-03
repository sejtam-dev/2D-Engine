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

export module Engine.Graphics:Shader;

import Engine.Utils;

export class Shader {
public:
    enum class ShaderType {
        VERTEX   = 0,
        FRAGMENT = 1
    };

    Shader() = default;

    Shader(std::string name, const std::string& vertexPath, const std::string& fragmentPath);

    ~Shader();

    Shader(const Shader&) = delete;

    Shader& operator=(const Shader&) = delete;

    Shader(Shader&&) = default;

    Shader& operator=(Shader&&) = default;

    GLuint CreateShader(const std::string& shaderString, const ShaderType type);

    GLuint CreateProgram();

    void DeleteShader();

    void DeleteProgram();

    static void CreateShader(const std::string& shaderName, const std::string& vertexPath,
                             const std::string& fragmentPath) {
        m_Shaders.emplace(shaderName, std::move(std::make_unique<Shader>(shaderName, vertexPath, fragmentPath)));
    }

    static void AddShader(std::unique_ptr<Shader>& shader) {
        m_Shaders.emplace(shader->GetName(), std::move(shader));
    }

    static void LinkShader(const std::string& shaderName) {
        return LinkShader(m_Shaders[shaderName].get());
    }

    static void LinkShader(Shader* shader) {
        if (shader->m_Program != -1) {
            GLCall(glLinkProgram(shader->m_Program));
            GLCall(glValidateProgram(shader->m_Program));

            GLCall(glUseProgram(shader->m_Program));

            Shader::linkedShader = shader;
        }
    }

    static std::map<std::string, std::unique_ptr<Shader>>& GetShaders() {
        return m_Shaders;
    }

    static Shader* GetLinkedShader() {
        return linkedShader;
    }

    static void ClearShaders() {
        m_Shaders.clear();
    }

    void SetUniform1f(const std::string& name, const float value1);

    void SetUniform2f(const std::string& name, const glm::vec2& value);

    void SetUniform3f(const std::string& name, const glm::vec3& value);

    void SetUniform4f(const std::string& name, const glm::vec4& value);

    void SetUniformMatrix2fv(const std::string& name, const glm::mat2& value);

    void SetUniformMatrix3fv(const std::string& name, const glm::mat3& value);

    void SetUniformMatrix4fv(const std::string& name, const glm::mat4& value);

    std::string& GetName() {
        return m_Name;
    }

private:
    static std::string LoadFromFile(const std::string& path);

    GLint getUniform(const std::string& name);

    std::string m_Name;

    GLuint m_Program = -1;
    GLuint m_VertexShader = -1;
    GLuint m_FragmentShader = -1;

    std::map<std::string, int> m_UniformLocations = {};

    static std::map<std::string, std::unique_ptr<Shader>> m_Shaders;
    static Shader* linkedShader;
};
