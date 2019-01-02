#include "Renderer.h"



Renderer::Renderer(std::vector<Mesh*> meshes, Shader* shader) {
	this->meshes = meshes;
	this->shader = shader;
}


Renderer::~Renderer() {
}
