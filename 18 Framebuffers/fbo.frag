#version 330 core

in vec2 TexCoords;

out vec4 color;

uniform sampler2D screenTexture;

const  float offset = 1.0f / 300.0f;
void main()
{
	color = texture(screenTexture, TexCoords);
	
	// Inversion
	//color = vec4(vec3(1.0f - texture(screenTexture, TexCoords)), 1.0f);
	
/*	// Grayscale
	color = texture(screenTexture, TexCoords);
	float average = (color.r + color.g + color.b) / 3.0f;
	color = vec4(average, average, average, 1.0f);
*/
/*	// Grayscale
	//color = texture(screenTexture, TexCoords);
	//float average = 0.2126f * color.r + 0.7152f * color.g + 0.0722f * color.b;
	//color = vec4(average, average, average, 1.0f);
*/
/*	// Kernel effects
	vec2 offsets[9] = vec2[](
		vec2(-offset, offset),	vec2(0.0f, offset),	vec2(offset, offset),
		vec2(-offset, 0.0f),	vec2(0.0f, 0.0f),	vec2(offset, 0.0f),
		vec2(-offset, -offset),	vec2(0.0f, -offset),vec2(offset, -offset)
	);

	float kernel[9] = float[](
		-1, -1, -1,
		-1,  9, -1,
		-1, -1, -1
	);

	vec3 sampleTex[9];

	for(int i = 0; i < 9; ++i)
		sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
	
	vec3 col;
	for(int i = 0; i < 9; ++i)
		col += sampleTex[i] * kernel[i];

	color = vec4(col, 1.0f);
*/
/*	// Blur
		vec2 offsets[9] = vec2[](
		vec2(-offset, offset),	vec2(0.0f, offset),	vec2(offset, offset),
		vec2(-offset, 0.0f),	vec2(0.0f, 0.0f),	vec2(offset, 0.0f),
		vec2(-offset, -offset),	vec2(0.0f, -offset),vec2(offset, -offset)
	);

	float kernel[9] = float[](
		1.0f / 16, 2.0f / 16, 1.0f / 16,
		2.0f / 16, 4.0f / 16, 2.0f / 16,
		1.0f / 16, 2.0f / 16, 1.0f / 16  
	);

	vec3 sampleTex[9];

	for(int i = 0; i < 9; ++i)
		sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
	
	vec3 col;
	for(int i = 0; i < 9; ++i)
		col += sampleTex[i] * kernel[i];

	color = vec4(col, 1.0f);
*/
/*	// Edge-detection
	vec2 offsets[9] = vec2[](
		vec2(-offset, offset),	vec2(0.0f, offset),	vec2(offset, offset),
		vec2(-offset, 0.0f),	vec2(0.0f, 0.0f),	vec2(offset, 0.0f),
		vec2(-offset, -offset),	vec2(0.0f, -offset),vec2(offset, -offset)
	);

	float kernel[9] = float[](
		1, 1, 1,
		1,-8, 1,
		1, 1, 1  
	);

	vec3 sampleTex[9];

	for(int i = 0; i < 9; ++i)
		sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
	
	vec3 col;
	for(int i = 0; i < 9; ++i)
		col += sampleTex[i] * kernel[i];

	color = vec4(col, 1.0f);
*/
}
