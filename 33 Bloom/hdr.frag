#version 330 core

out vec4 color;

in vec2 TexCoords;

uniform sampler2D hdrBuffer;
uniform float exposure;

void main()
{
    const float gamma = 2.2;
    vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;

	//vec3 result = hdrColor / (hdrColor + vec3(1.0));
	vec3 result = vec3(1.0f) - exp(-hdrColor * exposure);

	result = pow(result, vec3(1.0f / gamma));

	color = vec4(result, 1.0f);
}