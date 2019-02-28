#version 450 core

layout (location = 0) in vec3 position;
layout (location = 3) in uint id;

flat out uint idToGeom;

void main(){
	gl_Position = vec4(position,1.0f);
	idToGeom = id;
}