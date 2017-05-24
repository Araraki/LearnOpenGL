#version 330 core
#define NR_POINT_LIGHT 2

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
}; 

struct PointLight
{
	vec3 position;

	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

uniform vec3 viewPos;
uniform PointLight pointLights[NR_POINT_LIGHT];
uniform Material material;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 result = 0.12f * texture(material.diffuse, TexCoords).rgb;

	for(int i = 0; i < NR_POINT_LIGHT; ++i)
		result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);

	color = vec4(result, 1.0f);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - FragPos);

	// 计算漫反射强度
	float diff = max(dot(normal, lightDir), 0.0f);

	// 计算镜面反射强度
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0f), 16.0f);

	// 将各个分量合并
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
	
	// 计算衰减
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    return (diffuse + specular) * attenuation;
}
