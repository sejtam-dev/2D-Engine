#include "Shader.h"

Shader::Shader(ShaderType type = ShaderType::NONE) : type(type) { }
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

		ERROR("Failed to compile " << (type == ShaderType::VERTEX ? "vertex" : "fragment") << " shader!");
		ERROR(message);
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
		linkedVertexShader = vertex->id;

		const GLuint vertexShader = vertex->id;
		GLCall(glAttachShader(program, vertexShader));
	}

	if (linkedFragmentShader != fragment->id) {
		linkedFragmentShader = fragment->id;

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

GLint Shader::getUniform(const std::string& name) {
	if (m_UniformLocations.find(name) != m_UniformLocations.end())
		return m_UniformLocations[name];

	const GLint uniformLocation = GLCallReturn(glGetUniformLocation(program, name.c_str()));
	m_UniformLocations[name] = uniformLocation;
	return uniformLocation;
}

void Shader::SetUniform1f(const std::string& name, const float value1)
{
	const GLint uniform = getUniform(name);
	if (uniform == -1)
		return;

	glUniform1f(uniform, value1);
}
void Shader::SetUniform2f(const std::string& name, glm::vec2& value)
{
	const GLint uniform = getUniform(name);
	if (uniform == -1)
		return;

	glUniform2f(uniform, value.x, value.y);
}
void Shader::SetUniform3f(const std::string& name, glm::vec3& value)
{
	const GLint uniform = getUniform(name);
	if (uniform == -1)
		return;

	glUniform3f(uniform, value.x, value.y, value.z);
}
void Shader::SetUniform4f(const std::string& name, glm::vec4& value)
{
	const GLint uniform = getUniform(name);
	if (uniform == -1)
		return;

	glUniform4f(uniform, value.x, value.y, value.z, value.w);
}

void Shader::SetUniformMatrix2fv(const std::string& name, glm::mat2& value) {
	const GLint uniform = getUniform(name);
	if (uniform == -1)
		return;

	glUniformMatrix2fv(uniform, 1, false, &value[0][0]);
}
void Shader::SetUniformMatrix3fv(const std::string& name, glm::mat3& value) {
	const GLint uniform = getUniform(name);
	if (uniform == -1)
		return;

	glUniformMatrix3fv(uniform, 1, false, &value[0][0]);
}
void Shader::SetUniformMatrix4fv(const std::string& name, glm::mat4& value) {
	const GLint uniform = getUniform(name);
	if (uniform == -1)
		return;

	glUniformMatrix4fv(uniform, 1, false, &value[0][0]);
}

GLuint Shader::program = 0;
GLuint Shader::linkedVertexShader = 0;
GLuint Shader::linkedFragmentShader = 0;