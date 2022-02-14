#pragma once

#include "Static.h"

enum class ENGINE_API ShaderType
{
	NONE = -1, VERTEX = 0, FRAGMENT = 1
};

class ENGINE_API Shader
{
private:
	static unsigned int program;
	
	std::string m_shaderString;

public:
	unsigned int id = 0;
	ShaderType type = ShaderType::NONE;

	Shader(ShaderType type);
	~Shader();

	bool loadFromFile(const std::string& path);
	void loadFromString(const std::string& string);

	unsigned int createShader();
	static unsigned int linkShaders(const Shader* vertex, const Shader* fragment);
};