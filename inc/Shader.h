#pragma once

/*
*	This class takes a vertex shader and fragment shader,
*	compiles and then links them into a program.
*/
#include <glad/glad.h>
#include <string>
#include <vector>
#include <glm/glm.hpp>

class Shader 
{
	public: 
		Shader() {}
		Shader(std::string vertexPath, std::string fragmentPath);
		~Shader();
		GLuint getID() const;
		/*
			Compiles and links the shaders to a program.
			The program's ID is stored in the ID memeber 
			variable.
		*/
		bool addShader(std::string shaderPath, GLuint type);
		bool link();
		void setUniform1i(const char *uniform, int value);
		void setUniformMatrix4fv(const char *uniform, const glm::mat4 &matrix);
		void setUniform3fv(const char *uniform, const glm::vec3 &vec);
		void setUniform4fv(const char *uniform, const glm::vec4 &vec);
		void setUniform1f(const char *uniform, float value);

		void use() const;
		void unuse() const;
	private:
		GLuint ID;
		std::vector<GLuint> shaders;
		std::string parseShader(std::string shaderPath);
};
