#version 330 core

out vec4 color;

float LinearizeDepth(float depth)
{
	float near = 0.1f;
	float far = 100.0;
	float z = depth * 2.0f - 1.0f;
	return (2.0f * near) / (far + near - z * (far - near));
}

void main()
{
	float depth = LinearizeDepth(gl_FragCoord.z);
	color = vec4(vec3(depth), 1.0f);
}
