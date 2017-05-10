#version 330 core
#define NR_POINT_LIGHT 4

struct Material
{
	sampler2D diffuse;
	sampler2D normalMap;
}; 

in VERT_OUT
{
	vec3 FragPos;
	vec2 TexCoords;
	vec3 TangentLightPos;
	vec3 TangentViewPos;
	vec3 TangentFragPos;
} fs_in;

out vec4 FragColor;

uniform Material material;

void main()
{
	// method 2: transform all relevant world-space vectors then pass out
	vec3 norm = texture(material.normalMap, fs_in.TexCoords).rgb;
	norm = normalize(norm * 2.0f - 1.0f);
	
	vec3 color = texture(material.diffuse, fs_in.TexCoords).rgb;

	vec3 ambient = 0.1f * color;

	vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
	float diff = max(dot(lightDir, norm), 0.0f);
	vec3 diffuse = diff * color;

	vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(norm, halfwayDir), 0.0f), 32.0f);
	vec3 specular = vec3(0.2f) * spec;

	FragColor = vec4(ambient + diffuse + specular, 1.0f);
}
