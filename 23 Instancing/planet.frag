#version 330 core
in VERT_OUT
{
	vec2 TexCoords;
} frag_in;

out vec4 color;

uniform sampler2D texture_diffuse1;

void main()
{
	color = texture(texture_diffuse1, frag_in.TexCoords);
}