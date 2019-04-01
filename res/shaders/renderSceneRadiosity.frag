#version 450 core

out vec4 outColor;

in vec2 uv;

layout(binding=1) uniform sampler2D screenSampler;
layout(binding=5) uniform sampler2D albedoSampler;

void main() {
	vec3 color;
	vec3 albedo = texture(albedoSampler,uv).rgb;
	albedo = length(albedo) == 0 ? vec3(1.0f) : albedo;
	color = albedo * texture(screenSampler,uv).rgb;
	outColor = vec4(color, 1.0f);
}