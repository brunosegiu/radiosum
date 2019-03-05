#include "common/Renderer.h"



Renderer::Renderer() {

}


void Renderer::setCamera(Camera* camera) {
	this->camera = camera;
}

void Renderer::setScene(Scene* scene) {
	this->scene = scene;
}

void Renderer::read() {
	this->buffer->read();
}


Renderer::~Renderer() {
}
