#version 330 core

uniform vec3 color;
uniform bool selected;
uniform bool win;

in vec2 texcoords;
out vec4 FragColor;

void main()
{
	const float checker_border = 0.05;
	const float checker_radius = 0.24;
	const vec2 checker_center = vec2(0.5, 0.5);
	const float selected_border = 0.05;
	const float selected_radius = 0.4;
	
	vec2 uv = texcoords;
	uv -= checker_center;
	float distance = sqrt(dot(uv, uv));
	
	vec4 final_color = vec4(0.0);
	
	float alpha = smoothstep(checker_radius + checker_border, checker_radius - checker_border, distance);
	final_color = vec4(color, alpha);
	
	if(win)
	{
		float a = 1.0 + smoothstep(selected_radius, selected_radius + selected_border, distance) 
					  - smoothstep(selected_radius - selected_border, selected_radius, distance);
		final_color = vec4(a < 1.0 ? vec3(0.1, 1.0, 0.1) : final_color.rgb, (1.0 - a) + final_color.a);
	}
	
	if(selected)
	{
		float a = 1.0 + smoothstep(selected_radius, selected_radius + selected_border, distance) 
					  - smoothstep(selected_radius - selected_border, selected_radius, distance);
		final_color = vec4(a < 1.0 ? vec3(1.0, 0.1, 0.1) : final_color.rgb, (1.0 - a) + final_color.a);
	}
	

	
	FragColor = final_color;
}