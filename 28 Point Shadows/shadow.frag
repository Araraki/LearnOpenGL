#version 330 core

out vec4 FragColor;

in VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
} fs_in;

uniform sampler2D diffuseTexture;
uniform samplerCube depthMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float far_plane;

int samples = 20;
vec3 sampleOffsetDirections[20] = vec3[]
(
	vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1),
	vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
	vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
	vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
	vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);

float closestDepth = 0.0f;
float currentDepth = 0.0f;
float bias = 0.05f;

float ShadowCalculation(vec3 fragPos)
{
	float shadow = 0.0f;
//	float diskRadius = 0.05f;
	float diskRadius = (1.0 + (length(viewPos - fragPos) / far_plane)) / 25.0f;

	vec3 fragToLight = fragPos - lightPos;
	closestDepth = texture(depthMap, fragToLight).r;
	closestDepth *= far_plane;

	currentDepth = length(fragToLight);
	shadow = currentDepth - bias > closestDepth ? 1.0f : 0.0f;
	// Percentage-closer filtering
	for(int i = 0; i < samples; ++i)
	{
		closestDepth = texture(depthMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
		closestDepth *= far_plane;	// Undo mapping [0, 1]
		if(currentDepth - bias > closestDepth)
			shadow += 1.0f;
	}
	shadow /= float(samples);

	return shadow;
}

void main()
{
	vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
	vec3 normal = normalize(fs_in.Normal);
	vec3 lightColor = vec3(0.3f);

	vec3 ambient = 0.3f * color;

	vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	float diff = max(dot(lightDir, normal), 0.0f);
	vec3 diffuse = diff * lightColor;

	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	float spec = 0.0f;
	vec3 halfwayDir = normalize(lightDir + viewDir);
	spec = pow(max(dot(normal, halfwayDir), 0.0f), 64.0f);
	vec3 specular = spec * lightColor;

	float shadow = ShadowCalculation(fs_in.FragPos);
	vec3 lighting = (ambient + (1.0f-shadow)*(diffuse+specular)) * color;

	FragColor = vec4(lighting, 1.0f);
	// debug
	// FragColor = vec4(vec3(closestDepth / far_plane), 1.0f);
}
