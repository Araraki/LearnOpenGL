#version 330 core
out float FragColor;
in vec2 TexCoords;

uniform sampler2D gPositionDepth;
uniform sampler2D gNormal;
uniform sampler2D texNoise;

uniform mat4 proj;

uniform vec3 samples[64];

const int kernelSize = 64;
const float radius = 1.0f;

const vec2 noiseScale = vec2(800.0f/4.0f, 600.0f/4.0f);

void main()
{
    vec3 fragPos = texture(gPositionDepth, TexCoords).xyz;
    vec3 normal = texture(gNormal, TexCoords).rgb;
    vec3 randomVec = texture(texNoise, TexCoords * noiseScale).xyz;

	vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN = mat3(tangent, bitangent, normal);

	float occlusion = 0.0f;
	for(int i = 0; i < kernelSize; ++i)
	{
		vec3 sample = TBN * samples[i];
		sample = fragPos + sample * radius;
		
		vec4 offset = vec4(sample, 1.0f);
		offset = proj * offset;
		offset.xyz /= offset.w;
		offset.xyz = offset.xyz * 0.5f + 0.5f;

		float sampleDepth = -texture(gPositionDepth, offset.xy).w;

		float rangeCheck = smoothstep(0.0f, 1.0f, radius / abs(fragPos.z - sampleDepth));
		occlusion += (sampleDepth >= sample.z ? 1.0f : 0.0f) * rangeCheck;
	}
	occlusion = 1.0f - (occlusion / kernelSize);
	FragColor = occlusion;
}
