#version 450 core

in smooth float radiosityToFrag;
in smooth float emissionToFrag;
in smooth float reflactanceToFrag;

layout(location = 0) out float radiosity;
layout(location = 1) out uint id;
layout(location = 2) out float emission;
layout(location = 3) out float reflactance;

uniform uint tFaces;
uniform uint offset;

uint correrctID(uint vertexID){
	if (vertexID < tFaces)
		return vertexID;
	return tFaces + (vertexID - tFaces) / 2;
}

void main() {
	id = correrctID(gl_PrimitiveID) + offset;
	radiosity = max(radiosityToFrag, 0.0f);
	emission = max(emissionToFrag, 0.01f);
	reflactance = reflactanceToFrag;
}