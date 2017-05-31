#version 330 core
layout (location = 0) out vec4 color;

uniform vec3 lampColor;

void main()
{
	color = vec4(lampColor, 1.0f);
}
