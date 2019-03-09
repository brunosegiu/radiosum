#version 450 core

out vec4 outColor;

in vec2 uv;

layout(binding=4) uniform sampler2D screenSampler;

void main() {
	float reflactance = texture(screenSampler,uv).r;
	vec3 color;
	color = vec3(reflactance);
	outColor = vec4(color, 1.0f);
}