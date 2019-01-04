#include "DisplayController.h"



DisplayController::DisplayController() {
	this->bufferRenderer = new BufferRenderer();

	// TODO: allow setup of several renderers at runtime
	this->renderer = this->bufferRenderer;
}

void DisplayController::render() {
	this->renderer->render();
}


DisplayController::~DisplayController() {
	delete this->bufferRenderer;
}
