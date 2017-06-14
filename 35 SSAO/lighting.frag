#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPositionDepth;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D ssao;

struct Light
{
	vec3 Position;
	vec3 Color;

	float linear;
	float quadratic;
};

const float constant = 1.0f;
uniform Light light;
//uniform vec3 viewPos;

void main()
{
	vec3 FragPos = texture(gPositionDepth, TexCoords).rgb;
	vec3 Normal = texture(gNormal, TexCoords).rgb;
	vec3 Diffuse = texture(gAlbedo, TexCoords).rgb;
	float AmbientOcclusion = texture(ssao, TexCoords).r;

	vec3 ambient = vec3(0.3 * AmbientOcclusion);
	vec3 lighting = ambient;
	vec3 viewDir = normalize(-FragPos);

	vec3 lightDir = normalize(light.Position - FragPos);
	vec3 diffuse = max(dot(Normal, lightDir), 0.0f) * Diffuse * light.Color;
	
	vec3 halfwayDir = normalize(lightDir + viewDir);
	vec3 specular = pow(max(dot(Normal, halfwayDir), 0.0f), 16.0f) * light.Color;

	float distance    = length(light.Position - FragPos);
	float attenuation = 1.0f / (1.0f + light.linear * distance + light.quadratic * (distance * distance));
	
	diffuse  *= attenuation;
    specular *= attenuation;
    lighting += diffuse + specular;

	FragColor = vec4(lighting , 1.0f);
}
