#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 text;
layout (location = 2) in vec3 normal;
layout (location = 3) in uint id;

out vec3 normalToFrag;
out flat uint idToFrag;

uniform mat4 worldTransform;

void main(){
	gl_Position =  worldTransform * vec4(position,1.0f);
	normalToFrag = normal;
	idToFrag = id;
}