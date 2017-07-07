#version 330 core

in vec3 vsColor;
out vec4 color;

void main()
{
	color = vec4(vsColor, 1.0f);
}