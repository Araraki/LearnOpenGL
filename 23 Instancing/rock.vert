#version 330 core
layout (location = 0) in vec3 position;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in mat4 instanceMatrix;

layout (std140) uniform Matrices
{
	mat4 proj;
	mat4 view;
};

out VERT_OUT
{
	vec2 TexCoords;
} vert_out;

void main()
{
    gl_Position = proj * view * instanceMatrix * vec4(position, 1.0f);
	vert_out.TexCoords = texCoords;
}