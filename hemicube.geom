#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

flat in uint idToGeom[];

flat out uint idToFrag;

uniform mat4 worldTransform;

void main(){
	for (int face = 0; face < 6; face++){
		for (int vertex = 0; vertex < 3; vertex++){
			gl_Layer = face;
			gl_Position = worldTransform * gl_in[vertex].gl_Position;
			idToFrag = idToGeom[vertex];
			EmitVertex();
		}
		EndPrimitive();
	}
}