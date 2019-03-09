#include "Renderer.h"


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

void Renderer::start() {
	this->shader->bind();
	this->buffer->bind();
	this->buffer->clean();
}

Camera* Renderer::getCamera() {
	return this->camera;
}


Renderer::~Renderer() {
}
