#version 330 core

out uint id;

flat in uint idToFrag;

void main() {
	id = uint(100);// idToFrag;
}