#version 330 core

in vec2 texcoords;
in vec3 point_color;
out vec4 FragColor;

void main()
{
	FragColor = vec4(point_color, (1.0 - length(2.0 * texcoords - 1.0)) < 0.275 ? 0.0 : 1.0);
}