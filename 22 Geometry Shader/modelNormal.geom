#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT
{
	vec3 normal;
} gs_in[];

const float MAGNITUDE = 0.05f;

void GenerateLine(int index)
{
	gl_Position = gl_in[index].gl_Position;
	EmitVertex();
	gl_Position = gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0f) * MAGNITUDE;
	EmitVertex();
	EndPrimitive();
}

void main()
{
	for(int i = 0; i<3; ++i)
		GenerateLine(i);
	//GenerateLine(0);
	//GenerateLine(1);
	//GenerateLine(2);
}