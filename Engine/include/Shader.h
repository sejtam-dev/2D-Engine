#pragma once

#include "Static.h"

enum class ShaderType
{
	NONE = -1, VERTEX = 0, FRAGMENT = 1
};

class ENGINE_API Shader
{
private:
	static GLuint program;
	static GLuint linkedVertexShader;
	static GLuint linkedFragmentShader;
	
	std::string m_shaderString;

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

	static void SetUniform4f(const GLchar* key, GLfloat value1, GLfloat value2, GLfloat value3, GLfloat value4);

};