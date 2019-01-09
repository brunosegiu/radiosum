#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices=15) out;

#define FACES 5

flat in uint idToGeom[];

flat out uint idToFrag;

uniform mat4 worldTransform[FACES];


void main(){
	for (int face = 0; face < FACES; face++){
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