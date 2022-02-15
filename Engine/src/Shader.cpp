#include "Shader.h"

Shader::Shader(ShaderType type) : type(type) { }
Shader::~Shader()
{
	glDeleteShader(id);
}

bool Shader::LoadFromFile(const std::string& path)
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

void Shader::LoadFromString(const std::string& string)
{
	m_shaderString = string;
}

GLuint Shader::CreateShader()
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

	const GLchar* shader = m_shaderString.c_str();
	glShaderSource(id, 1, &shader, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if(result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = static_cast<char*>(alloca(length * sizeof(char)));
		glGetShaderInfoLog(id, length, &length, message);

		std::cerr << "Failed to compile " << (type == ShaderType::VERTEX ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cerr << message << std::endl;
	}

	return id;
}

void Shader::DeleteProgram() {
	glDeleteProgram(program);
}

GLuint Shader::LinkShaders(const Shader* vertex, const Shader* fragment)
{
	if (vertex == nullptr)
		return 0;

	if (fragment == nullptr)
		return 0;

	if (program == 0)
		program = glCreateProgram();

	if (linkedVertexShader != vertex->id) {
		if (linkedVertexShader != 0)
			glDetachShader(program, linkedVertexShader);

		const GLuint vertexShader = vertex->id;
		glAttachShader(program, vertexShader);
	}

	if (linkedFragmentShader != fragment->id) {
		if (linkedFragmentShader != 0)
			glDetachShader(program, linkedFragmentShader);

		const GLuint fragmentShader = fragment->id;
		glAttachShader(program, fragmentShader);
	}

	glLinkProgram(program);
	glValidateProgram(program);

	glUseProgram(program);

	return program;
}

GLuint Shader::UnlinkShader(ShaderType type)
{
	GLuint cachedShader;

	switch(type)
	{
	case ShaderType::VERTEX:
		cachedShader = linkedVertexShader;

		if(linkedVertexShader != 0)
		{
			glDetachShader(program, linkedVertexShader);
			linkedVertexShader = 0;
		}

		return cachedShader;

	case ShaderType::FRAGMENT:
		cachedShader = linkedFragmentShader;

		if (linkedFragmentShader != 0)
		{
			glDetachShader(program, linkedFragmentShader);
			linkedFragmentShader = 0;
		}

		return cachedShader;

	case ShaderType::NONE:
		return 0;
	}
}

void Shader::UnlinkShaders()
{
	UnlinkShader(ShaderType::VERTEX);
	UnlinkShader(ShaderType::FRAGMENT);
}

void Shader::SetUniform4f(const GLchar* key, const GLfloat value1, const GLfloat value2, const GLfloat value3, const GLfloat value4)
{
	const GLint uniformLocation = glGetUniformLocation(program, key);
	glUniform4f(uniformLocation, value1, value2, value3, value4);
}

GLuint Shader::program = 0;
GLuint Shader::linkedVertexShader = 0;
GLuint Shader::linkedFragmentShader = 0;