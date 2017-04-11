#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <gl/glew.h>

class Shader
{
public:
	GLuint Program;
	Shader();
	Shader(const std::string vertexPath, const std::string fragmentPath);
	void Use();
};

