#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

in VS_OUT{
	vec3 color;
} gs_in[];

out vec3 fColor;

void build_house(vec4 position)
{
	fColor = gs_in[0].color;
	gl_Position = position + vec4(-0.2f, -0.2f, 0.0f, 0.0f);    // 1:bottom-left  
	EmitVertex();

	//fColor = vec3(0.0f, 0.0f, 0.0f);
	gl_Position = position + vec4( 0.2f, -0.2f, 0.0f, 0.0f);    // 2:bottom-right
	EmitVertex();

	//fColor = gs_in[0].color;
	gl_Position = position + vec4(-0.2f,  0.2f, 0.0f, 0.0f);    // 3:top-left
	EmitVertex();

	gl_Position = position + vec4( 0.2f,  0.2f, 0.0f, 0.0f);    // 4:top-right
	EmitVertex();
	
	fColor = vec3(1.0f, 1.0f, 1.0f);
	gl_Position = position + vec4( 0.0f,  0.4f, 0.0f, 0.0f);    // 5:top
	EmitVertex();

	EndPrimitive();
}

void main()
{
	build_house(gl_in[0].gl_Position);
}