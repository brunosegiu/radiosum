#version 450 core

out vec4 outColor;

in vec2 uv;

layout(binding=2) uniform usampler2D idScreenSampler;

void main() {
	vec3 color;
	uint id = texture(idScreenSampler,uv).r;
	float r = float( (id >> 16) & uint(0xFF)) / 255.0f;
	float g = float( (id >> 8)  & uint(0xFF)) / 255.0f;
	float b = float(  id        & uint(0xFF)) / 255.0f;
	color = vec3(r,g,b);
	outColor = vec4(color, 1.0f);
}