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

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;

void main()
{
	vec4 viewPos = view * model * vec4(position, 1.0f);
	gl_Position = proj * viewPos;
	FragPos =  viewPos.xyz;
	TexCoords = texCoords;

	mat3 normalMatrix = transpose(inverse(mat3(view * model)));
	Normal = normalMatrix * normal;
}