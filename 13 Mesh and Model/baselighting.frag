#version 330 core
#define NR_POINT_LIGHT 2

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
}; 

struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight
{
	vec3 position;	
	vec3 spotDir;

	float cutOff;
	float outerCutOff;
	
	vec3 ambient;
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
	vec3 result = vec3(0.0f, 0.0f, 0.0f);// = CalcDirLight(dirLight, norm, viewDir);

	for(int i = 0; i < NR_POINT_LIGHT; ++i)
		result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);

	//result += CalcSpotLight(spotLight, norm, FragPos, viewDir);

	color = vec4(result, 1.0f);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - FragPos);
	// 计算漫反射强度
	float diff = max(dot(normal, lightDir), 0.0f);
	// 计算镜面反射强度
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// 计算衰减
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	// 将各个分量合并
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}
