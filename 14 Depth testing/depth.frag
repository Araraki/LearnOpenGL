#version 330 core

in vec2 TexCoords;

out vec4 color;

uniform float near;
uniform float far;
uniform sampler2D texture1;

float LinearizeDepth(float depth)
{
	float z = depth * 2.0f - 1.0f;	// back to NDC
	return (2.0f * near * far) / (far + near - z * (far - near));
}

void main()
{
	float depth = (LinearizeDepth(gl_FragCoord.z) - near) / (far - near);
	//color = vec4(vec3(depth), 1.0f);
	color = texture(texture1, TexCoords);
	// color = vec4(vec3(gl_FragCoord.z), 1.0f);
}
