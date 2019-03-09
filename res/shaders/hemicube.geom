#version 450 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 15) out;

#define FACES 5

uniform mat4 worldTransform[FACES];
uniform vec4 clipPlane;

void main() {
	for (int face = 0; face < FACES; face++) {
		mat4 transform = worldTransform[face];
		for (int vertex = 0; vertex < 3; vertex++) {
			vec4 position = gl_in[vertex].gl_Position;
			gl_Layer = face;
			gl_Position = transform * position;
			gl_ClipDistance[0] = dot(position, clipPlane);
			gl_PrimitiveID = gl_PrimitiveIDIn;
			EmitVertex();
		}
		EndPrimitive();
	}
}