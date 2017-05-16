#version 330 core
#define NR_POINT_LIGHT 4

struct Material
{
	sampler2D diffuse;
	float shininess;
}; 

struct PointLight
{
	vec3 position;
	vec3 color;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

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

	vec3 result = vec3(0.0f);

	for(int i = 0; i < NR_POINT_LIGHT; ++i)
		result += CalcPointLight(pointLights[i], norm, vert_in.FragPos, viewDir);

	color = vec4(result, 1.0f);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	// 计算漫反射强度
	float diff = max(dot(normal, lightDir), 0.0f);
	// 计算镜面反射强度
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// 计算衰减
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	// 将各个分量合并
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, vert_in.TexCoords)) * light.color;
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, vert_in.TexCoords)) * light.color;
    vec3 specular = vec3(0.0f);//light.specular * spec * vec3(0.3f) * light.color;
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}
