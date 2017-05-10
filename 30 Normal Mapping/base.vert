
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

layout (std140) uniform Matrices
{
	mat4 proj;
	mat4 view;
};
uniform mat4 model;

out VERT_OUT
{
	vec3 FragPos;
	mat3 TBN;
	vec2 TexCoords;
} vert_out;

void main()
{
    gl_Position = proj * view * model * vec4(position, 1.0);
	vert_out.FragPos = vec3(model * vec4(position, 1.0f));
	vert_out.TexCoords = texCoords;
	vec3 T = normalize(vec3(model * vec4(tangent, 0.0f)));
	vec3 B = normalize(vec3(model * vec4(bitangent, 0.0f)));
	vec3 N = normalize(vec3(model * vec4(normal, 0.0f)));
	// method 1: pass TBN to fragShader
	vert_out.TBN = mat3(T, B, N);
}
