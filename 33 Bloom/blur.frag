#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D image;

uniform bool horizontal;

uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{
	vec2 tex_offset = 1.0f / textureSize(image, 0);	// get size of single textel
	vec3 result = texture(image, TexCoords).rgb * weight[0];	// curent fragment's contribution

	if(horizontal)
	{
		for(int i = 1; i < 5; ++i)
		{
			result += texture(image, TexCoords + vec2(tex_offset.x * i, 0.0f)).rgb * weight[i];
			result += texture(image, TexCoords - vec2(tex_offset.x * i, 0.0f)).rgb * weight[i];
		}
	}
	else
	{
		for(int i = 1; i < 5; ++i)
		{
			result += texture(image, TexCoords + vec2(0.0f, tex_offset.y * i)).rgb * weight[i];
			result += texture(image, TexCoords - vec2(0.0f, tex_offset.y * i)).rgb * weight[i];
		}
	}
	FragColor = vec4(result, 1.0f);
}