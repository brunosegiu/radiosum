#version 450 core

out vec4 outColor;

in vec2 uv;

layout(binding=3) uniform sampler2D screenSampler;

void main() {
	float emission = texture(screenSampler,uv).r;
	vec3 color;
	if (emission == .0f)
		color = vec3(.0f,.0f,.1f);
	else
		color = vec3(emission);
	outColor = vec4(color, 1.0f);
}