#version 330 core
#define NR_POINT_LIGHT 4

struct Material
{
	sampler2D diffuse;
	sampler2D normalMap;
	sampler2D depthMap;
}; 

in VERT_OUT
{
	vec3 FragPos;
	vec2 TexCoords;
	vec3 TangentLightPos;
	vec3 TangentViewPos;
	vec3 TangentFragPos;
} fs_in;

out vec4 FragColor;

uniform Material material;
uniform float height_scale;

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{
	const float minLayers = 8;
	const float maxLayers = 32;
	
	float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0f, 0.0f, 1.0f), viewDir)));	

	float layerDepth = 1.0 / numLayers;
	float currentLayerDepth = 0.0f;
	vec2 P = viewDir.xy * height_scale;
	vec2 deltaTexCoords = P / numLayers;

	vec2 currentTexCoords = texCoords;
	float currentDepthMapValue = texture(material.depthMap, currentTexCoords).r;

	while(currentLayerDepth < currentDepthMapValue)
	{
		currentTexCoords -= deltaTexCoords;
		currentDepthMapValue = texture(material.depthMap, currentTexCoords).r;
		currentLayerDepth += layerDepth;
	}

	// parallax occlusion mapping
	vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

	float afterDepth = currentDepthMapValue - currentLayerDepth;
	float beforeDepth = texture(material.depthMap, prevTexCoords).r - currentLayerDepth + layerDepth;

	float weight = afterDepth / (afterDepth - beforeDepth);
	vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0f - weight);

	return finalTexCoords;
}

void main()
{
	// base value
	vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
	vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	// offset texture coordinates with ParallaxMapping
	vec2 texCoords = ParallaxMapping(fs_in.TexCoords, viewDir);
	if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
		discard;
	vec3 norm =  normalize(texture(material.normalMap, texCoords).rgb * 2.0f - 1.0f);
	

	// calculation
	vec3 color = texture(material.diffuse, texCoords).rgb;

	vec3 ambient = 0.1f * color;

	float diff = max(dot(lightDir, norm), 0.0f);
	vec3 diffuse = diff * color;

	float spec = pow(max(dot(norm, halfwayDir), 0.0f), 32.0f);
	vec3 specular = vec3(0.2f) * spec;
	
	// sum
	FragColor = vec4(ambient + diffuse + specular, 1.0f);
}
