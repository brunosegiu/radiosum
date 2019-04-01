#version 450 core

out vec4 outColor;

in vec2 uv;

layout(binding=5) uniform sampler2D screenSampler;

void main() {
	vec3 color = texture(screenSampler,uv).rgb;
	outColor = vec4(color, 1.0f);
}