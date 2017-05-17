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
uniform bool inverse_normals;

out VERT_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
} vert_out;

void main()
{
	vert_out.FragPos = vec3(model * vec4(position, 1.0f));
	vert_out.TexCoords = texCoords;

	vec3 n = inverse_normals ? -normal : normal;
	mat3 normalMatrix = transpose(inverse(mat3(model)));
	vert_out.Normal = normalize(normalMatrix * n);

    gl_Position = proj * view * model * vec4(position, 1.0);
}