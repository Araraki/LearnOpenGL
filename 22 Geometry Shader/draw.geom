#version 330 core
layout (point) in;
layout (line_strip, max_vertices = 2) out;

void main()
{
	gl_Position = gl_in[0].gl_Position + vec4(-0.1f, 0.0f, 0.0f, 0.0f);
	EmitVertex();

	gl_Position = gl_in[0].gl_Position + vec4(0.1f, 0.0f, 0.0f, 0.0f);
	EmitVertex();

	EndPrimitive();
}