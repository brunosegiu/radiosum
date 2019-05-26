#version 450 core

layout (location = 0) in vec3 position;

uniform mat4 worldTransform;

void main(){
	gl_Position =  worldTransform * vec4(position,1.0f);
}