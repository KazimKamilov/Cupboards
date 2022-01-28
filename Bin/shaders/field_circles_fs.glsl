#version 330 core

uniform vec3 color;
uniform bool selected;

in vec2 texcoords;
out vec4 FragColor;

void main()
{
	const vec2 checker_center = vec2(0.5, 0.5);
	const float selected_border = 0.05;
	const float selected_radius = 0.4;
	
	vec2 uv = texcoords;
	uv -= checker_center;
	float distance = sqrt(dot(uv, uv));

	float alpha = 1.0 + smoothstep(selected_radius, selected_radius + selected_border, distance) 
				  - smoothstep(selected_radius - selected_border, selected_radius, distance);
	vec4 final_color = vec4(vec3(0.1, 0.1, 1.0), (1.0 - alpha));


	FragColor = final_color;
}