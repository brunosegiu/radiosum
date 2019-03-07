#include "common/Renderer.h"



Renderer::Renderer() {

}


void Renderer::setCamera(Camera* camera) {
	this->camera = camera;
}

void Renderer::setScene(Scene* scene) {
	this->scene = scene;
}

GLuint Renderer::read() {
	return this->buffer->read();
}

Buffer* Renderer::getBuffer() {
	return this->buffer;
}


Renderer::~Renderer() {
}
