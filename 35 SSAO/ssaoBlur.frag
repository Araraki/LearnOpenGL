#version 330 core
out float fragColor;
in vec2 TexCoords;

uniform sampler2D ssaoInput;
const int blurSize = 4;

void main()
{
	vec2 texelSize = 1.0f / vec2(textureSize(ssaoInput, 0));
	float result = 0.0f;

	for(int x = 0; x < blurSize; ++x)
	{
		for(int y = 0; y < blurSize; ++y)
		{
			vec2 offset = (vec2(-2.0f) + vec2(float(x), float(y))) * texelSize;
			result += texture(ssaoInput, TexCoords + offset).r;
		}
	}
	fragColor = result / float(blurSize * blurSize);
}