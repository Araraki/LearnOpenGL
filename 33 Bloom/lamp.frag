#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

uniform vec3 lampColor;

in VERT_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
} fs_in;

void main()
{
	FragColor = vec4(lampColor, 1.0f);

	float brightness = dot(FragColor.rgb, vec3(0.2126f, 0.7152f, 0.0722f));
	if(brightness > 1.0f)
		BrightColor = vec4(FragColor.rgb, 1.0f);
	else
		BrightColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}