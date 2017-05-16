#version 330 core

out vec4 color;

in vec2 TexCoords;

uniform sampler2D hdrBuffer;

void main()
{
	const float gamma = 2.2f;
	vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;

	vec3 mapped = hdrColor / (hdrColor + vec3(1.0f));

	mapped = pow(mapped, vec3(1.0f / gamma));

	color = vec4(hdrColor, 1.0f);
}