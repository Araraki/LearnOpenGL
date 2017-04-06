#version 330 core

in vec3 LightingColor;

out vec4 color;

uniform vec3 objectColor;

void main()
{
	color = vec4(LightingColor * objectColor, 1.0f);
}
