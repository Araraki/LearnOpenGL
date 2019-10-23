#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>

class Shader
{
public:
	GLuint Program;
	Shader(const std::string vertexPath, const std::string fragmentPath);
	void Use();
};

