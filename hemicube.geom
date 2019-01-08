#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

#define NUM_FACES 6

flat in uint idToGeom[];

flat out uint idToFrag;

uniform mat4 worldTransform[NUM_FACES];


void main(){
	for (int face = 0; face < NUM_FACES; face++){
		mat4 transform = worldTransform[face];
		for (int vertex = 0; vertex < 3; vertex++){
			gl_Layer = face;
			gl_Position = transform * gl_in[vertex].gl_Position;
			idToFrag = idToGeom[vertex];
			EmitVertex();
		}
		EndPrimitive();
	}
}