#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoords;

out	ivec2 TexCoords;

uniform uint screenWidth;
uniform uint screenHeight;

void main()
{
    gl_Position = vec4(position.x, position.y, 0.0f, 1.0f);
	vec2 tmp = vec2((texCoords.x + 1.0f)/2, (texCoords.y + 1.0f)/2);
	TexCoords = ivec2(tmp.x*screenWidth, tmp.y*screenHeight);
}