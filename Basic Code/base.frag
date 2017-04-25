#version 330 core
#define NR_POINT_LIGHT 4

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

in VERT_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
} vert_in;

out vec4 color;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHT];
uniform SpotLight spotLight;
uniform Material material;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	vec3 norm = normalize(vert_in.Normal);
	vec3 viewDir = normalize(viewPos - vert_in.FragPos);

	vec3 result = CalcDirLight(dirLight, norm, viewDir);

	for(int i = 0; i < NR_POINT_LIGHT; ++i)
		result += CalcPointLight(pointLights[i], norm, vert_in.FragPos, viewDir);

	result += CalcSpotLight(spotLight, norm, vert_in.FragPos, viewDir);

	color = vec4(result, 1.0f);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	// ���㷴��ǿ��
	float diff = max(dot(normal, lightDir), 0.0f);
	// ���㾵�淴��ǿ��
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// �ϲ��������շ���
	vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, vert_in.TexCoords));
	vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, vert_in.TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, vert_in.TexCoords));

	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	// ����������ǿ��
	float diff = max(dot(normal, lightDir), 0.0f);
	// ���㾵�淴��ǿ��
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// ����˥��
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	// �����������ϲ�
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, vert_in.TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, vert_in.TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, vert_in.TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	
	float diff = max(dot(normal, lightDir), 0.0f);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, vert_in.TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, vert_in.TexCoords));	
	vec3 specular = light.specular * spec * vec3(texture(material.specular, vert_in.TexCoords));
	
	float theta = dot(lightDir, normalize(-light.spotDir)); // ���� ��Դ��Ƭ������ �� ��Դ��ǰ������ �γɼнǵ�cosֵ
	float epsilon = light.cutOff - light.outerCutOff;
	float spotIntensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);

	ambient *= spotIntensity;
	diffuse *= spotIntensity;
	specular *= spotIntensity;

	float distance = length(light.position - fragPos);
	float attenuation = 1.0f / (light.constant + light.linear*distance + light.quadratic*(distance*distance));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

    return (ambient + diffuse + specular);
}