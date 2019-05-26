#version 450 core

layout (location = 0) in vec3 position;

uniform mat4 worldTransform;
uniform vec4 clipPlane;

void main(){
	gl_Position =  worldTransform * vec4(position,1.0f);
	gl_ClipDistance[0] = dot(gl_Position, clipPlane);
}