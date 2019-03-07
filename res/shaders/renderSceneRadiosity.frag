#version 450 core

out vec4 outColor;

in vec2 uv;

layout(binding=1) uniform sampler2D screenSampler;

void main() {
	vec3 color;
	color = vec3(texture(screenSampler,uv).r);
	outColor = vec4(color, 1.0f);
}