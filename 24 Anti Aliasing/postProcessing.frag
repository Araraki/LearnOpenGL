#version 330 core

in ivec2 TexCoords;

out vec4 color;

uniform sampler2DMS screenTextureMS;

void main()
{
	vec4 sum = vec4(0,0,0,0);
	for (int i = 0; i < 4; ++i)
	{
		vec4 sample = texelFetch(screenTextureMS, TexCoords, i);
		sum += sample;
	}
	color = sum / float(4);	
}