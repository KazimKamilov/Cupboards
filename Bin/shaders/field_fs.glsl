#version 330 core

in vec2 texcoords;
out vec4 FragColor;

void main()
{
	const float border = 0.04;
	const float circle_radius = 0.3;
	const vec2 circle_center = vec2(0.5, 0.5);

	vec2 uv = texcoords;
	uv -= circle_center;

	float dist = sqrt(dot(uv, uv));
	float alpha = smoothstep(circle_radius + border, circle_radius - border, dist);
	float alpha0 = 1.0 + smoothstep(circle_radius, circle_radius + (border + 0.02), dist) - smoothstep(circle_radius - (border + 0.02), circle_radius, dist);

	FragColor = vec4(mix(vec3(0.01), vec3(3.0), (1.0 - alpha0)), alpha);
}