#include <glad/glad.h>
#include "Shader.h"

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <glm/gtc/type_ptr.hpp>	// used for value_ptr
using namespace std;

Shader::Shader(string vertexPath, string fragmentPath)
{
	ID = glCreateProgram();
	
	addShader(vertexPath, GL_VERTEX_SHADER);
	addShader(fragmentPath, GL_FRAGMENT_SHADER);	
}

Shader::~Shader()
{
	// cout << "DELETEING SHADER" << ID<< endl;
	for (auto const &shader : shaders)
	{
		glDeleteShader(shader);
	}
}

bool Shader::addShader(string shaderPath, GLenum type)
{
	GLuint shader;
	shader = glCreateShader(type);
	string shaderSource = parseShader(shaderPath);
	const GLchar *sSource = shaderSource.c_str();
	glShaderSource(shader, 1, &sSource, NULL);
	glCompileShader(shader);

	// error checking
	char infoLog[1024];
	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		string shaderType;
		switch (type)
		{
			case GL_VERTEX_SHADER:
				shaderType = "VERTEX"; break;
			case GL_FRAGMENT_SHADER:
				shaderType = "FRAGMENT"; break;
			case GL_TESS_CONTROL_SHADER:
				shaderType = "TESS CONTROL"; break;
			case GL_TESS_EVALUATION_SHADER:
				shaderType = "TESS EVALUATION"; break;
		}
		glGetShaderInfoLog(shader, 1024, NULL, infoLog);
		cerr << shaderType << " SHADER COMPILATION FAILED\n" << 
			shaderPath << endl << infoLog << endl;
		return 0;
	}
	glAttachShader(ID, shader);
	return success;

}

bool Shader::link()
{
	glLinkProgram(ID);
	
	char infoLog[1024];
	int success;
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 1024, NULL, infoLog);
		cerr << "PROGRAM LINKAGE FAILED\n" << infoLog << endl;
		
	}
	return success;
	//glDeleteShader(vertexShader);	// no longer needed after linkage
	//glDeleteShader(fragmentShader);
}

string Shader::parseShader(string path)
{
	ifstream in(path);
	string buffer = [&in] {
		std::ostringstream ss{};
		ss << in.rdbuf();
		return ss.str();
	}();
	in.close();
	return buffer;
}

void Shader::use() const
{
	glUseProgram(ID);
}

void Shader::unuse() const
{
	glUseProgram(0);
}

void Shader::setUniform1i(const char *uniform, int value)
{
	GLint uniformLocation = glGetUniformLocation(ID, uniform);
	glUniform1i(uniformLocation, value);
}
void Shader::setUniform1f(const char *uniform, float value)
{
	GLint uniformLocation = glGetUniformLocation(ID, uniform);
	glUniform1f(uniformLocation, value);
}
void Shader::setUniformMatrix4fv(const char *uniform, const glm::mat4 &matrix)
{
	GLint uniformLocation = glGetUniformLocation(ID, uniform);
	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(matrix));
}
void Shader::setUniform3fv(const char *uniform, const glm::vec3 &vec)
{
	GLint uniformLocation = glGetUniformLocation(ID, uniform);
	glUniform3fv(uniformLocation, 1, glm::value_ptr(vec));
}
void Shader::setUniform4fv(const char *uniform, const glm::vec4 &vec)
{
	GLint uniformLocation = glGetUniformLocation(ID, uniform);
	glUniform4fv(uniformLocation, 1, glm::value_ptr(vec));
}
GLuint Shader::getID() const { return ID; }