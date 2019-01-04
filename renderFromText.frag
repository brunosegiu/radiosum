#version 330 core

out vec4 outColor;

in vec2 uv;

uniform usampler2D buffer;

void main() {
	uint id = texture(buffer, uv).r;
	float r = float( (id >> 16) & uint(0xFF)) / 255.0f;
	float g = float( (id >> 8)  & uint(0xFF)) / 255.0f;
	float b = float(  id        & uint(0xFF)) / 255.0f;
	vec3 color = vec3(r,g,b);
	outColor = vec4(color, 1.0f);
}