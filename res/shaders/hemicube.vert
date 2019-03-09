#version 450 core

layout (location = 0) in vec3 position;

flat out uint idToGeom;

void main(){
	gl_Position = vec4(position,1.0f);
}