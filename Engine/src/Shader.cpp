#include "Shader.h"

Shader::Shader(ShaderType type) : type(type) { }
Shader::~Shader()
{
	glDeleteShader(id);
}

bool Shader::loadFromFile(const std::string& path)
{
	std::ifstream stream;
	stream.open(path);

	std::stringstream stringStream;
	if (stream.is_open()) {
		std::string line;

		while (getline(stream, line)) {
			if(line.find("#shader") != std::string::npos)
			{
				if (line.find("vertex") != std::string::npos)
					type = ShaderType::VERTEX;
				else if(line.find("fragment") != std::string::npos)
					type = ShaderType::FRAGMENT;
			}
			else
			{
				stringStream << line << "\n";
			}
		}
		stream.close();
	}
	else
	{
		return false;
	}

	m_shaderString = stringStream.str();
	return true;
}

void Shader::loadFromString(const std::string& string)
{
	m_shaderString = string;
}

unsigned int Shader::createShader()
{
	switch(type)
	{
	case ShaderType::NONE:
		return 0;
	case ShaderType::VERTEX:
		id = glCreateShader(GL_VERTEX_SHADER);
		break;
	case ShaderType::FRAGMENT:
		id = glCreateShader(GL_FRAGMENT_SHADER);
		break;
	}

	const char* shader = m_shaderString.c_str();
	glShaderSource(id, 1, &shader, nullptr);
	glCompileShader(id);

	// TODO: Handle errors

	return id;
}

unsigned int Shader::linkShaders(const Shader* vertex, const Shader* fragment)
{
	if (vertex == nullptr)
		return 0;

	if (fragment == nullptr)
		return 0;

	if (program == 0)
		program = glCreateProgram();

	const unsigned int vertexShader = vertex->id;
	glAttachShader(program, vertexShader);

	const unsigned int fragmentShader = fragment->id;
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);
	glValidateProgram(program);

	glUseProgram(program);

	return program;
}

unsigned int Shader::program = 0;