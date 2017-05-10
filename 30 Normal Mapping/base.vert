
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

uniform vec3 lightPos;
uniform vec3 viewPos;

out VERT_OUT
{
	vec3 FragPos;
	vec2 TexCoords;
	vec3 TangentLightPos;
	vec3 TangentViewPos;
	vec3 TangentFragPos;
} vert_out;

void main()
{
    gl_Position = proj * view * model * vec4(position, 1.0);
	vert_out.FragPos = vec3(model * vec4(position, 1.0f));
	vert_out.TexCoords = texCoords;
	
	// method 2: transform all relevant world-space vectors then pass out
	mat3 normalMatrix = transpose(inverse(mat3(model)));
	vec3 T = normalize(normalMatrix * tangent);
	vec3 B = normalize(normalMatrix * bitangent);
	vec3 N = normalize(normalMatrix * normal);
	
	mat3 TBN = transpose(mat3(T, B, N));
	vert_out.TangentLightPos = TBN * lightPos;
	vert_out.TangentViewPos = TBN * viewPos;
	vert_out.TangentFragPos = TBN * vert_out.FragPos;


}
