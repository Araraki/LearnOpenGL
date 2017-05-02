#version 330 core

in VERT_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
} vert_in;

out vec4 color;

uniform sampler2D groundTexture;
uniform vec3 viewPos;
uniform vec3 pointLightPos;
uniform float shininess;
uniform bool blinn;

void main()
{
	vec3 texColor = texture(groundTexture ,vert_in.TexCoords).rgb;
	
	// ambient
	vec3 ambient = 0.05f * texColor;
	
	// diffuse
	vec3 lightDir = normalize(pointLightPos - vert_in.FragPos);
	vec3 normal = normalize(vert_in.Normal);
	float diff = max(dot(lightDir, normal), 0.0f);
	vec3 diffuse = diff * texColor;
	
	// specular
	vec3 viewDir = normalize(viewPos - vert_in.FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = 0.0f;
	if(blinn)
	{
		vec3 halfwayDir = normalize(lightDir + viewDir);
		spec = pow(max(dot(normal, halfwayDir), 0.0f), 16.0f);
	}
	else
	{
		vec3 reflectDir = reflect(-lightDir, normal);
		spec = pow(max(dot(viewDir, reflectDir), 0.0f), 8.0f);
	}
	vec3 specular = vec3(0.3f) * spec;

	// º∆À„À•ºı
    float distance    = length(pointLightPos - vert_in.FragPos);
    float attenuation = 1.0f / (1.0f + 0.09f * distance + 0.032f * (distance * distance));

	color = vec4(ambient*attenuation + diffuse*attenuation + specular*attenuation, 1.0f);
}
