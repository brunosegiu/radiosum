#version 450 core

out uint id;

flat in uint idToFrag;

void main() {
	id = idToFrag;
}