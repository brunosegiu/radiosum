#version 450 core

in vec3 normalToFrag;
in flat uint idToFrag;

layout(location = 0) out vec4 color; 
layout(location = 1) out uint id; 

void main() {
	color = max(dot(normalize(normalToFrag), normalize(vec3(.2f,.4f,0.6f))), 0.05f) * vec4(1.0f);
	id = idToFrag;
}