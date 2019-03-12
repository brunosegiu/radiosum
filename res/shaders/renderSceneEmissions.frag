#version 450 core

out vec4 outColor;

in vec2 uv;

layout(binding=3) uniform sampler2D screenSampler;
uniform float maxEmission = 100.0f;

void main() {
	float emission = texture(screenSampler,uv).r / maxEmission;
	vec3 color;
	color = vec3(emission);
	outColor = vec4(color, 1.0f);
}