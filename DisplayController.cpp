#include "DisplayController.h"

#include "Window.h"

DisplayController::DisplayController(Scene* scene) {
	this->bufferRenderer = new BufferRenderer();
	this->sceneRenderer = new SceneRenderer(scene);
	Camera* userCamera = Window::get()->getCamera();
	this->sceneRenderer->setCamera(userCamera);
	// TODO: allow setup of several renderers at runtime
	this->renderer = this->bufferRenderer;
}

void DisplayController::render() {
	this->renderer->render();
}


DisplayController::~DisplayController() {
	delete this->bufferRenderer;
}
