#version 330 core

in VERT_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
} vert_in;

out vec4 fragColor;

uniform sampler2D groundTexture;
uniform vec3 viewPos;
uniform vec3 lightColors[4];
uniform vec3 lightPositions[4];
uniform bool blinn;
uniform bool gamma;

vec3 BlinnPhong(vec3 normal, vec3 fragPos, vec3 lightPos, vec3 lightColor)
{
	// diffuse
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(lightDir, normal), 0.0f);
	vec3 diffuse = diff * lightColor * 0.5f;
	
	// specular
	vec3 viewDir = normalize(viewPos - fragPos);
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
	vec3 specular = spec * lightColor;

	// attenuation
	float distance = length(lightPos - fragPos);
	float attenuation = 1.0f / (gamma
							    ? (1.0f + 0.09f * distance + 0.032f * (distance * distance))
								: (1.0f + 0.09f * distance));

	diffuse *= attenuation;
	specular *= attenuation;
	
	return diffuse + specular;
}

void main()
{
	vec3 color = texture(groundTexture ,vert_in.TexCoords).rgb;
	vec3 lighting = vec3(0.0f);

	vec3 normal = normalize(vert_in.Normal);

	for	(int i=0; i<4; ++i)
		lighting += BlinnPhong(normal, vert_in.FragPos, lightPositions[i], lightColors[i]);

	color *= lighting;

	if(gamma)
		color = pow(color, vec3(1.0f/2.2f));
    fragColor = vec4(color, 1.0f);
}
