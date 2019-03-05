#include "display/DisplayController.h"

#include "display/Window.h"

DisplayController::DisplayController() {
	this->bufferRenderer = new TextureRenderer("renderHemicube.frag");
	this->textureRenderer = new TextureRenderer("renderScene.frag");
	Camera* userCamera = Window::get()->getCamera();
	this->textureRenderer->setCamera(userCamera);
	this->renderer = this->textureRenderer;
	this->mode = DISPLAY;
}

void DisplayController::render() {
	this->renderer->render();
}

void DisplayController::setMode(RunMode mode) {
	this->mode = mode;
	if (mode == PREPROCESS)
		this->renderer = this->bufferRenderer;
	else
		this->renderer = this->textureRenderer;
}


DisplayController::~DisplayController() {
	delete this->bufferRenderer;
}
