#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in uint id;
layout (location = 2) in float emission;
layout (location = 3) in float radiosity;
layout (location = 4) in float reflactance;

out smooth float radiosityToFrag;
out smooth float emissionToFrag;
out smooth float reflactanceToFrag;
out flat uint idToFrag;

uniform mat4 worldTransform;

void main(){
	gl_Position =  worldTransform * vec4(position,1.0f);
	radiosityToFrag = radiosity;
	reflactanceToFrag = reflactance;
    emissionToFrag = emission;
	idToFrag = id;
}