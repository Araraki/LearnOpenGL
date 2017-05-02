#version 330 core

out vec4 color;

uniform vec3 lampColor;

void main()
{
	color = vec4(lampColor, 1.0f);
//	float gamma = 2.2;
//    color.rgb = pow(color.rgb, vec3(1.0/gamma));
}