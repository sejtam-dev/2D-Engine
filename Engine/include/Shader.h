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
	NONE = -1, VERTEX = 0, FRAGMENT = 1
};

class Shader
{
private:
	static GLuint program;
	static GLuint linkedVertexShader;
	static GLuint linkedFragmentShader;
	
	std::string m_shaderString;

	static std::map<std::string, int> m_UniformLocations;
	static GLint getUniform(const std::string& name);

public:
	GLuint id = 0;
	ShaderType type = ShaderType::NONE;

	Shader(ShaderType type);
	~Shader();

	bool LoadFromFile(const std::string& path);
	void LoadFromString(const std::string& string);

	GLuint CreateShader();

	static void DeleteProgram();

	static GLuint LinkShaders(const Shader* vertex, const Shader* fragment);
	static GLuint UnlinkShader(ShaderType type);
	static void UnlinkShaders();

	static void SetUniform1f(const std::string& name, float value1);
	static void SetUniform2f(const std::string& name, const glm::vec2& value);
	static void SetUniform3f(const std::string& name, const glm::vec3& value);
	static void SetUniform4f(const std::string& name, const glm::vec4& value);

	static void SetUniformMatrix2fv(const std::string& name, const glm::mat2& value);
	static void SetUniformMatrix3fv(const std::string& name, const glm::mat3& value);
	static void SetUniformMatrix4fv(const std::string& name, const glm::mat4& value);
};