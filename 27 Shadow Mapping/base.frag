#version 330 core

in VERT_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
} vert_in;

out vec4 fragColor;

uniform sampler2D diffuseTexture;
uniform vec3 viewPos;
uniform vec3 directLightColor;
uniform vec3 directLightDir;

vec3 DirectLight(vec3 lightDir, vec3 normal, vec3 viewDir, vec3 lightColor)
{
	vec3 ambient = 0.02f * lightColor;
	
	float diff = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = diff * lightColor;

	float spec = 0.0f;
	vec3 halfwayDir = normalize(lightDir + viewDir);
	spec = pow(max(dot(normal, halfwayDir), 0.0f), 16.0f);

	vec3 specular = spec * lightColor;

	return ambient+diffuse+specular;
}


void main()
{
	vec3 color = texture(diffuseTexture ,vert_in.TexCoords).rgb;
	vec3 lighting = vec3(0.0f, 0.0f, 0.0f);

	vec3 normal = normalize(vert_in.Normal);
	vec3 viewDir = normalize(viewPos - vert_in.FragPos);

	lighting += DirectLight(normalize(directLightDir), normal, viewDir, directLightColor);
	color *= lighting;

	color = pow(color, vec3(1.0f/2.2f));
    fragColor = vec4(color, 1.0f);
}
