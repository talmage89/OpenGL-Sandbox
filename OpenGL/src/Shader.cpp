#include <iostream>
#include <GL/glew.h>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "Shader.h"

#include <glm/glm.hpp>

Shader::Shader(const std::string& filepath)
	: m_Filepath(filepath), m_RendererID(0), m_Uniforms()
{
	ShaderProgramSource source = ParseShader(filepath);
	m_RendererID = CreateProgram(source.VertexSource, source.FragmentSource);
}

Shader::~Shader() {
	GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const {
	GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const {
	GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int v0) {
	int location = GetUniformLocation(name);
	GLCall(glUniform1i(location, v0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
	int location = GetUniformLocation(name);
	GLCall(glUniform4f(location, v0, v1, v2, v3));
}

void Shader::SetUniformMat4(const std::string& name, glm::mat4 matrix) {
	int location = GetUniformLocation(name);
	GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]));
}

int Shader::GetUniformLocation(const std::string& name) {

	if (m_Uniforms.find(name) != m_Uniforms.end())
		return m_Uniforms[name];

	int location = glGetUniformLocation(m_RendererID, name.c_str());
	m_Uniforms[name] = location;

	if (location == -1)
		std::cout << name << " location is -1 at " << m_Filepath << std::endl;

	return location;
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath) {
	enum ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };

	std::ifstream stream(filepath);
	std::string line;
	std::stringstream ss[2];

	ShaderType type = ShaderType::NONE;

	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else if (type > -1)
			ss[(int)type] << line << '\n';
	}

	return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int compiled;
	glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);
	if (compiled != GL_TRUE)
	{
		int log_length;
		char message[1024];
		glGetShaderInfoLog(id, 1024, &log_length, message);
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

unsigned int Shader::CreateProgram(const std::string& vertexShader, const std::string& fragmentShader) {
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	if (vs == 0 || fs == 0)
	{
		std::cout << "Shader compilation unsuccessful." << std::endl;
		return 0;
	}

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	int linked;
	int valid;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	glGetProgramiv(program, GL_VALIDATE_STATUS, &valid);
	if (linked != GL_TRUE || valid != GL_TRUE) {
		int log_length;
		char message[1024];
		glGetProgramInfoLog(program, 1024, &log_length, message);
		std::cout << message << std::endl;
		return 0;
	}

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}