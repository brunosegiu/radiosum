#version 450 core

in flat float radiosityToFrag;
in flat float emissionToFrag;
in flat uint idToFrag;

layout(location = 0) out float radiosity;
layout(location = 1) out uint id;
layout(location = 2) out float emission;

void main() {
	id = idToFrag;
	radiosity = max(radiosityToFrag * 1000.0f, 0.0f);
	emission = max(emissionToFrag, 0.01f);
}