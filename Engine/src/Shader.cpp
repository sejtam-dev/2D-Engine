#include "Shader.h"

#include <utility>

Shader::Shader(std::string name, const std::string& vertexPath, const std::string& fragmentPath): m_Name(std::move(name))
{
    std::string vertexShaderString = Shader::LoadFromFile(vertexPath);
    std::string fragmentShaderString = Shader::LoadFromFile(fragmentPath);

    CreateShader(vertexShaderString, ShaderType::VERTEX);
    CreateShader(fragmentShaderString, ShaderType::FRAGMENT);
    LOG("Shader '" << this->m_Name << "' created! Vertex Shader ID: " << this->m_VertexShader << " | " << " Fragment Shader ID: " << this->m_FragmentShader);

    CreateProgram();
    LOG("Shader '" << this->m_Name << "' program created! Program ID: " << this->m_Program);
}

Shader::~Shader()
{
	DeleteProgram();
    DeleteShader();
}

std::string Shader::LoadFromFile(const std::string& path)
{
	std::ifstream stream;
	stream.open(path);

	std::stringstream stringStream;
	if (stream.is_open()) {
		std::string line;

		while (getline(stream, line)) {
            stringStream << line << "\n";
		}
		stream.close();
	}
	else return {};

	return stringStream.str();
}

GLuint Shader::CreateShader(const std::string& shaderString, ShaderType type)
{
	GLuint id = 0;

	switch(type)
	{
	case ShaderType::VERTEX:
        this->m_VertexShader = id = GLCallReturn(glCreateShader(GL_VERTEX_SHADER));
		break;
	case ShaderType::FRAGMENT:
        this->m_FragmentShader = id = GLCallReturn(glCreateShader(GL_FRAGMENT_SHADER));
		break;
	}

    const char* shader = shaderString.c_str();
	GLCall(glShaderSource(id, 1, &shader, nullptr));
	GLCall(glCompileShader(id));

	GLint result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if(result == GL_FALSE)
	{
        GLint length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        auto* message = static_cast<GLchar*>(alloca(length * sizeof(GLchar)));
		GLCall(glGetShaderInfoLog(id, length, &length, message));

		ERROR("Failed to compile " << (type == ShaderType::VERTEX ? "vertex" : "fragment") << " shader!");
		ERROR(message);
	}

    return id;
}

GLuint Shader::CreateProgram() {
    this->m_Program = GLCallReturn(glCreateProgram());

    GLCall(glAttachShader(this->m_Program, this->m_VertexShader));
    GLCall(glAttachShader(this->m_Program, this->m_FragmentShader));

    return this->m_Program;
}

void Shader::DeleteShader() const {
    GLCall(glDeleteShader(this->m_VertexShader));
    GLCall(glDeleteShader(this->m_FragmentShader));
}

void Shader::DeleteProgram() const {
    GLCall(glDetachShader(this->m_Program, this->m_VertexShader));
    GLCall(glDetachShader(this->m_Program, this->m_FragmentShader));

	GLCall(glDeleteProgram(this->m_Program));
}

void Shader::LinkShader(Shader* shader)
{
	GLCall(glLinkProgram(shader->m_Program));
	GLCall(glValidateProgram(shader->m_Program));

	GLCall(glUseProgram(shader->m_Program));

    Shader::linkedShader = shader;
}

GLint Shader::getUniform(const std::string& name) {
	if (m_UniformLocations.find(name) != m_UniformLocations.end())
		return m_UniformLocations[name];

	const GLint uniformLocation = GLCallReturn(glGetUniformLocation(this->m_Program, name.c_str()));
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
void Shader::SetUniform2f(const std::string& name, const glm::vec2& value)
{
	const GLint uniform = getUniform(name);
	if (uniform == -1)
		return;

	glUniform2f(uniform, value.x, value.y);
}
void Shader::SetUniform3f(const std::string& name, const glm::vec3& value)
{
	const GLint uniform = getUniform(name);
	if (uniform == -1)
		return;

	glUniform3f(uniform, value.x, value.y, value.z);
}
void Shader::SetUniform4f(const std::string& name, const glm::vec4& value)
{
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

Shader* Shader::linkedShader = nullptr;