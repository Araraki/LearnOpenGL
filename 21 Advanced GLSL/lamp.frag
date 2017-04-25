#version 330 core

out vec4 color;

uniform vec3 lampColor;

void main()
{
//if(gl_FragCoord.x < 400)
//	color = vec4(lampColor, 1.0f);
//else
//	color = vec4(0.0f,1.0f,0.0f,1.0f);

if(gl_FrontFacing)
	color = vec4(lampColor, 1.0f);
else
	color = vec4(0.0f,1.0f,0.0f,1.0f);
}