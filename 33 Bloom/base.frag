#version 330 core
#define NR_POINT_LIGHT 4

struct Material
{
	sampler2D diffuseTex;
}; 

struct PointLight
{
	vec3 position;
	vec3 color;
};

in VERT_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
} fs_in;

out vec4 color;

uniform vec3 viewPos;
uniform PointLight lights[NR_POINT_LIGHT];
uniform Material material;

void main()
{
	vec3 baseColor = texture(material.diffuseTex, fs_in.TexCoords).rgb;
	vec3 normal = normalize(fs_in.Normal);
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);

	vec3 ambient = 0.1f * baseColor;
	
	vec3 lighting = vec3(0.0f);
	for	(int i = 0; i < NR_POINT_LIGHT; ++i)
	{
		vec3 lightDir = normalize(lights[i].position - fs_in.FragPos);
		float diff = max(dot(lightDir, normal), 0.0f);
		vec3 diffuse  = lights[i].color * diff * baseColor;

		vec3 halfwayDir = normalize(viewDir + lightDir);
		float spec = pow(max(dot(normal, halfwayDir), 0.0f), 16.0f);
		vec3 specular = vec3(0.3f) * lights[i].color * spec;

		vec3 result = diffuse + specular;
		float dist    = length(fs_in.FragPos - lights[i].position);
		result *= 1.0f / (dist * dist);

		lighting += result;
	}

	color = vec4(ambient + lighting, 1.0f);
}