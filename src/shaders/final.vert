#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 normalToFrag;

uniform mat4 worldTransform;

void main(){
	gl_Position =  worldTransform * vec4(position,1.0f);
	normalToFrag = normal;
}