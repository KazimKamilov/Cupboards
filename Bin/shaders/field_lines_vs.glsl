#version 330 core

in vec2 vertex;
out vec2 point;

void main()
{
	point = vertex;
	gl_Position = vec4(vertex, 0.0, 1.0);
}