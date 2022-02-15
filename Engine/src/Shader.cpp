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
	id = 0;

	switch(type)
	{
	case ShaderType::NONE:
		return 0;
	case ShaderType::VERTEX:
		id = GLCallReturn(glCreateShader(GL_VERTEX_SHADER));
		break;
	case ShaderType::FRAGMENT:
		id = GLCallReturn(glCreateShader(GL_FRAGMENT_SHADER));
		break;
	}

	const GLchar* shader = m_shaderString.c_str();
	GLCall(glShaderSource(id, 1, &shader, nullptr));
	GLCall(glCompileShader(id));

	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if(result == GL_FALSE)
	{
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = static_cast<char*>(alloca(length * sizeof(char)));
		GLCall(glGetShaderInfoLog(id, length, &length, message));

		std::cerr << "Failed to compile " << (type == ShaderType::VERTEX ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cerr << message << std::endl;
	}

	return id;
}

void Shader::DeleteProgram() {
	GLCall(glDeleteProgram(program));
}

GLuint Shader::LinkShaders(const Shader* vertex, const Shader* fragment)
{
	if (vertex == nullptr)
		return 0;

	if (fragment == nullptr)
		return 0;

	if (program == 0)
		program = GLCallReturn(glCreateProgram());

	if (linkedVertexShader != vertex->id) {
		const GLuint vertexShader = vertex->id;
		GLCall(glAttachShader(program, vertexShader));
	}

	if (linkedFragmentShader != fragment->id) {
		const GLuint fragmentShader = fragment->id;
		GLCall(glAttachShader(program, fragmentShader));
	}

	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glUseProgram(program));

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
			GLCall(glDetachShader(program, linkedVertexShader));
			linkedVertexShader = 0;
		}

		return cachedShader;

	case ShaderType::FRAGMENT:
		cachedShader = linkedFragmentShader;

		if (linkedFragmentShader != 0)
		{
			GLCall(glDetachShader(program, linkedFragmentShader));
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
	const GLint uniformLocation = GLCallReturn(glGetUniformLocation(program, key));
	glUniform4f(uniformLocation, value1, value2, value3, value4);
}

GLuint Shader::program = 0;
GLuint Shader::linkedVertexShader = 0;
GLuint Shader::linkedFragmentShader = 0;