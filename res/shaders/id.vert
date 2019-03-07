#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in uint id;

flat out uint idToFrag;

uniform mat4 worldTransform;

void main(){
	gl_Position =  worldTransform * vec4(position,1.0f);
	idToFrag = id;
}