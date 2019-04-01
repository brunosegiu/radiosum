#version 450 core

in smooth float emissionToFrag;
in smooth vec3 radiosityToFrag;
in smooth vec3 reflactanceToFrag;
in smooth vec2 uvToFrag;

layout(location = 0) out vec3 radiosity;
layout(location = 1) out uint id;
layout(location = 2) out float emission;
layout(location = 3) out vec3 reflactance;
layout(location = 4) out vec3 albedo;

uniform uint tFaces;
uniform uint offset;
layout(location = 0) uniform sampler2D albedoSampler;

uint correrctID(uint vertexID){
	if (vertexID < tFaces)
		return vertexID;
	return tFaces + (vertexID - tFaces) / 2;
}

void main() {
	id = correrctID(gl_PrimitiveID) + offset;
	radiosity = max(radiosityToFrag, 0.0f);
	emission = max(emissionToFrag, 0.0f);
	reflactance = max(reflactanceToFrag, 0.0f);
	albedo = texture(albedoSampler,uvToFrag).rgb;
}