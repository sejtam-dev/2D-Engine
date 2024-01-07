#pragma once

#include "Static.h"

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

#include "glm/mat2x2.hpp"
#include "glm/mat3x3.hpp"
#include "glm/mat4x4.hpp"

#include <map>

enum class ShaderType
{
    VERTEX = 0, FRAGMENT = 1
};

class Shader
{
private:
	GLuint m_Program;
	GLuint m_VertexShader;
	GLuint m_FragmentShader;

	std::map<std::string, int> m_UniformLocations;
	GLint getUniform(const std::string& name);

    static Shader* linkedShader;

public:
    Shader() = default;
	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	~Shader();

	GLuint CreateShader(const std::string& shaderString, ShaderType type);
    GLuint CreateProgram();

    void DeleteShader() const;
	void DeleteProgram() const;

    static Shader* GetLinkedShader() {
        return linkedShader;
    }

	static void LinkShader(Shader* shader);

	void SetUniform1f(const std::string& name, float value1);
	void SetUniform2f(const std::string& name, const glm::vec2& value);
	void SetUniform3f(const std::string& name, const glm::vec3& value);
	void SetUniform4f(const std::string& name, const glm::vec4& value);

	void SetUniformMatrix2fv(const std::string& name, const glm::mat2& value);
	void SetUniformMatrix3fv(const std::string& name, const glm::mat3& value);
	void SetUniformMatrix4fv(const std::string& name, const glm::mat4& value);

private:
    static std::string LoadFromFile(const std::string& path);

};