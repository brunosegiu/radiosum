#version 330 core

#define PI 3.1415926535897932384626433832795

out vec4 outColor;

in vec2 uv;

uniform usamplerCube buffer;

float toRadians(float angle){
	return (angle/180.0f) * PI;
}

vec3 getSamplerVec(vec2 uv){
	return vec3(uv,-1.0f);
	vec2 map = 2.0f * uv - 1.0f;
	return  vec3(cos(map.x * PI), map.y * tan(toRadians(120.0f) / 2.0f), sin(map.x * PI));
}

void main() {
	uint id = texture(buffer,getSamplerVec(uv)).r;
	float r = float( (id >> 16) & uint(0xFF)) / 255.0f;
	float g = float( (id >> 8)  & uint(0xFF)) / 255.0f;
	float b = float(  id        & uint(0xFF)) / 255.0f;
	vec3 color = vec3(r,g,b);
	outColor = vec4(color, 1.0f);
}