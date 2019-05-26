#version 450 core
layout(location = 0) out uint id;

uniform uint tFaces;
uniform uint offset;

uint correrctID(uint vertexID){
	if (vertexID < tFaces)
		return vertexID;
	return tFaces + (vertexID - tFaces) / 2;
}

void main() {
	id = correrctID(gl_PrimitiveID) + offset;
}