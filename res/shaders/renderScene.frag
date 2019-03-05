#version 450 core

out vec4 outColor;

in vec2 uv;

layout(binding=1) uniform sampler2D screenSampler;
layout(binding=2) uniform usampler2D idScreenSampler;
uniform bool shouldRenderIds = false;

void main() {
	vec3 color;
	if (shouldRenderIds){
		uint id = texture(idScreenSampler,uv).r;
		float r = float( (id >> 16) & uint(0xFF)) / 255.0f;
		float g = float( (id >> 8)  & uint(0xFF)) / 255.0f;
		float b = float(  id        & uint(0xFF)) / 255.0f;
		color = vec3(r,g,b);
	} else {
		color = texture(screenSampler,uv).xyz;
	}
	outColor = vec4(color, 1.0f);
}