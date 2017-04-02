#version 330 core

in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;
uniform float blend;

void main()
{
	//color = texture(ourTexture, TexCoord);
	color = mix(texture(ourTexture1, TexCoord),
				texture(ourTexture2, vec2(TexCoord.x*2, TexCoord.y*2)),
				blend);
}
