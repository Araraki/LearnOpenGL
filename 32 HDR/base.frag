#version 330 core
#define NR_POINT_LIGHT 4

struct Material
{
	sampler2D diffuseTex;
	float ambient;
	float diffuse;
	float specular;
	float shininess;
}; 

struct PointLight
{
	vec3 position;
	vec3 color;

	float constant;
	float linear;
	float quadratic;
};

in VERT_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
} vert_in;

out vec4 color;

uniform vec3 viewPos;
uniform PointLight pointLights[NR_POINT_LIGHT];
uniform Material material;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	vec3 norm = normalize(vert_in.Normal);
	vec3 viewDir = normalize(viewPos - vert_in.FragPos);

	vec3 result = material.ambient * vec3(texture(material.diffuseTex, vert_in.TexCoords));
	
	for(int i = 0; i < NR_POINT_LIGHT; ++i)
		result += CalcPointLight(pointLights[i], norm, vert_in.FragPos, viewDir);

	color = vec4(result, 1.0f);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);

    float distance    = length(lightDir);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	float diff = max(dot(-normal, lightDir), 0.0f);
    vec3 diffuse  = material.diffuse * diff * vec3(texture(material.diffuseTex, vert_in.TexCoords)) * light.color;
    diffuse *= attenuation;

	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(-normal, halfwayDir), 0.0), material.shininess);
    vec3 specular = material.specular * spec * vec3(texture(material.diffuseTex, vert_in.TexCoords)) * light.color;
	specular *= attenuation;
	specular = vec3(0.0f);

    return (diffuse + specular);
}
