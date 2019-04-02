#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in float emission;
layout (location = 2) in vec3 reflactance;
layout (location = 3) in vec3 radiosity;
layout (location = 4) in vec2 uv;
layout (location = 5) in float phongFactor;

out smooth float emissionToFrag;
out smooth vec3 radiosityToFrag;
out smooth vec3 reflactanceToFrag;
out smooth vec2 uvToFrag;
out smooth float phongFactorToFrag;


uniform mat4 worldTransform;

void main(){
	gl_Position =  worldTransform * vec4(position,1.0f);
	radiosityToFrag = radiosity;
	reflactanceToFrag = reflactance;
    emissionToFrag = emission;
	uvToFrag = uv;
	phongFactorToFrag = phongFactor == .0f ? 1.0f : phongFactor;
}