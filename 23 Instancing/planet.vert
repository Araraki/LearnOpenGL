#version 330 core
layout (location = 0) in vec3 position;
layout (location = 2) in vec2 texCoords;

layout (std140) uniform Matrices
{
	mat4 proj;
	mat4 view;
};
uniform mat4 model;

out VERT_OUT
{
	vec2 TexCoords;
} vert_out;

void main()
{
    gl_Position = proj * view * model * vec4(position, 1.0f);
	vert_out.TexCoords = texCoords;
}