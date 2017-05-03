#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;

out vec2 TexCoords;

in float near_plane;
in float far_plane;

uniform mat4 model;

void main()
{
    gl_Position = vec4(position, 1.0);
	TexCoords = texCoords;
}