#include "Renderer.h"



Renderer::Renderer() {

}

Renderer::Renderer(std::vector<Mesh*> meshes, Shader* shader, Camera* camera) {
	this->meshes = meshes;
	this->shader = shader;
	this->camera = camera;
}


Renderer::~Renderer() {
}
