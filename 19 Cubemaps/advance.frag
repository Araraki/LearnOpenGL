#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

uniform vec3 cameraPos;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_reflection1;
uniform samplerCube skybox;

void main()
{
	vec4 diffuse = texture(texture_diffuse1, TexCoords);

	vec3 I = normalize(FragPos - cameraPos);
	vec3 R = reflect(I, normalize(Normal));

	float spec = texture(texture_reflection1, TexCoords).r;
	vec4 skyrefl = texture(skybox, vec3(-R.x, -R.y, R.z)) * (spec>0.1f ? spec : 0.0f);
	
	color = diffuse + skyrefl;
}
