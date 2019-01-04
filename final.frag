#version 330 core

in vec3 normalToFrag;

out vec4 color;

void main() {
	color = max(dot(normalize(normalToFrag), normalize(vec3(.2f,.4f,0.6f))), 0.05f) * vec4(1.0f);
}