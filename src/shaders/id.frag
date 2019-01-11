#version 330 core

out uint id;

flat in uint idToFrag;

void main() {
	id = idToFrag;
}