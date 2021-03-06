#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

layout (std140) uniform Matrices
{
	mat4 proj;
	mat4 view;
};
uniform mat4 model;

out VERT_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
} vert_out;

void main()
{
    gl_Position = proj * view * model * vec4(position, 1.0);
	vert_out.FragPos = vec3(model * vec4(position, 1.0f));
	vert_out.Normal = mat3(transpose(inverse(model))) * normal;
	vert_out.TexCoords = texCoords;
}