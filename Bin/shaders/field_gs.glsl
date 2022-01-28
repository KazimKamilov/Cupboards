#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

out vec2 texcoords;

void main()
{
	vec2 position0 = gl_in[0].gl_Position.xy;	
	const vec2 size = vec2(0.15);
	
    gl_Position.xy = position0 + vec2(-size.x, -size.y);
	texcoords = vec2(0.0, 1.0);	
    EmitVertex(); 
	
    gl_Position.xy = position0 + vec2(size.x, -size.y);
	texcoords = vec2(1.0, 1.0);
    EmitVertex();
	
    gl_Position.xy = position0 + vec2(-size.x, size.y);
	texcoords = vec2(0.0, 0.0);
    EmitVertex();
	
    gl_Position.xy = position0 + vec2(size.x, size.y);
	texcoords = vec2(1.0, 0.0);
    EmitVertex();

    EndPrimitive();
}