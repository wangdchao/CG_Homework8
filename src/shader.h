#ifndef SHADER_H
#define SHADER_H

#include "allheader.h"
#include <string>
#include <iostream>

class shader
{
private:

	unsigned int ID;

	// utility function for checking shader compilation/linking errors.
	// ------------------------------------------------------------------------
	bool checkCompileErrors(GLuint shader, std::string type) const
	{
		GLint success;
		GLchar infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
				return false;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
				return false;
			}
		}
		return true;
	}

public:
	
	// constructor generates the shade
	// ------------------------------------------------------------------------
	shader()=default;

	bool createShader(const char* vShaderCode, const char* fShaderCode)
	{
		
		unsigned int vertex, fragment;
		// vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		if (!checkCompileErrors(vertex, "VERTEX")) return false;
		// fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		if (!checkCompileErrors(fragment, "FRAGMENT")) return false;

		// shader progream
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);

		glLinkProgram(ID);
		if (!checkCompileErrors(ID, "PROGRAM")) return false;
		// delete the shaders as they're linked into our program now and no longer necessery
		glDeleteShader(vertex);
		glDeleteShader(fragment);

		return true;
	}
 
	// ------------------------------------------------------------------------
	void use()
	{
		glUseProgram(ID);
	}
	void setUniformFloat(const ImVec4 &color) {
		int vertexColorLocation = glGetUniformLocation(ID, "color");
		glUniform4f(vertexColorLocation, color.x, color.y, color.z, color.w);
	}
	void setVertexUniformModel(const glm::mat4 &model) {
		int vertexColorLocation = glGetUniformLocation(ID, "model");
		glUniformMatrix4fv(vertexColorLocation, 1, GL_FALSE, &model[0][0]);
	}
};
#endif