#version 330 core

layout (location = 0) in vec2 vertex;
layout (location = 1) in vec3 point_color;

out vec3 color;

void main()
{
	color = point_color;
	gl_Position = vec4(vertex, 0.0, 1.0);
}