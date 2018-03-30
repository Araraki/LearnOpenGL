#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
//#include <gl\glew.h>
//#include <glad/glad.h>

class Shader
{
public:
	GLuint Program;

	Shader(const std::string vertexPath, const std::string fragmentPath)
	{
		std::string vertCode, fragCode;
		std::ifstream vertFile, fragFile;

		vertFile.exceptions(std::ifstream::badbit);
		fragFile.exceptions(std::ifstream::badbit);

		try
		{
			vertFile.open(vertexPath);
			fragFile.open(fragmentPath);

			std::stringstream vertStream, fragStream;
			vertStream << vertFile.rdbuf();
			fragStream << fragFile.rdbuf();

			vertFile.close();
			fragFile.close();

			vertCode = vertStream.str();
			fragCode = fragStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "Error::Shader::File_Not_Succesfully_Read" << std::endl;
		}

		const GLchar* vertCode_cstr = vertCode.c_str();
		const GLchar* fragCode_cstr = fragCode.c_str();

		GLuint vertex, fragment;
		GLint success;
		GLchar infoLog[512];

		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertCode_cstr, nullptr);
		glCompileShader(vertex);
		
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (! success)
		{
			glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
			std::cout << "Error::Shader::Vertex::Compilation_Failed" << std::endl
					  << "File:\t" << vertexPath << std::endl
					  << "Details:" << std::endl << infoLog << std::endl;
		}

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragCode_cstr, nullptr);
		glCompileShader(fragment);

		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
			std::cout << "Error::Shader::Fragment::Compilation_Failed\n" << std::endl
				<< "File:\t" << fragmentPath << std::endl
				<< "Details:" << std::endl << infoLog << std::endl;
		}
		
		this->Program = glCreateProgram();
		glAttachShader(this->Program, vertex);
		glAttachShader(this->Program, fragment);
		glLinkProgram(this->Program);
		
		glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(this->Program, 512, nullptr, infoLog);
			std::cout << "Error::Shader::Program::Linking_Failed" << std::endl;
		}

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	void Use()
	{
		glUseProgram(this->Program);
	}
};
