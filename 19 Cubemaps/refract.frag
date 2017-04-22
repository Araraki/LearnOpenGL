#version 330 core

in vec3 Normal;
in vec3 Position;

out vec4 color;

uniform vec3 cameraPos;
uniform samplerCube skybox;

float refractionRatio = 1.0f / 1.52f;
void main()
{
	vec3 I = normalize(Position - cameraPos);
	vec3 R = refract(I, normalize(Normal), refractionRatio);
	color = texture(skybox, vec3(-R.x, -R.y, R.z));
}
