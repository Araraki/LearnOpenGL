#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

layout (std140) uniform Matrices
{
	mat4 proj;
	mat4 view;
};

out vec2 TexCoords;
out vec3 fragPosition;
out vec3 Normal;

uniform mat4 model;

void main()
{
	gl_Position = proj * view * model * vec4(position, 1.0f);
	fragPosition = vec3(model * vec4(position, 1.0f));
	Normal = mat3(transpose(inverse(model))) * normal;
	TexCoords = texCoords;
}