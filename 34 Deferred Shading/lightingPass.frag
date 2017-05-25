#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColorSpec;

struct Light
{
	vec3 Position;
	vec3 Color;

	float linear;
	float quadratic;
};

const int NR_LIGHTS = 16;
uniform Light lights[NR_LIGHTS];
uniform vec3 viewPos;

void main()
{
	vec3 FragPos = texture(gPosition, TexCoords).rgb;
	vec3 Normal = texture(gNormal, TexCoords).rgb;
	vec3 Color = texture(gColorSpec, TexCoords).rgb;
	float Specular = texture(gColorSpec, TexCoords).a;

	vec3 lighting = Color * 0.1f;
	vec3 viewDir = normalize(viewPos - FragPos);
	for(int i = 0; i < NR_LIGHTS; ++i)
	{
		vec3 lightDir = normalize(lights[i].Position - FragPos);
		vec3 diffuse = max(dot(Normal, lightDir), 0.0f) * Color * lights[i].Color;
		
		vec3 halfwayDir = normalize(lightDir + viewDir);
		vec3 specular = pow(max(dot(Normal, halfwayDir), 0.0f), 16.0f) * Specular * lights[i].Color;

		float distance    = length(lights[i].Position - FragPos);
		float attenuation = 1.0f / (1.0f + lights[i].linear * distance + lights[i].quadratic * (distance * distance));
		
		lighting += ((diffuse + specular) * attenuation);
	}

	FragColor = vec4(vec3(lighting) , 1.0f);
}
