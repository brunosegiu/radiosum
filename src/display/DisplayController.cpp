#include "display/DisplayController.h"

#include "common/Application.h"

DisplayController::DisplayController(RunMode mode) {
	this->bufferRenderer = new TextureRenderer("renderHemicube.frag");
	this->radiosityRenderer = new TextureRenderer("renderSceneRadiosity.frag");
	this->emissionsRenderer = new TextureRenderer("renderSceneEmissions.frag");
	this->facesRenderer = new TextureRenderer("renderSceneFaces.frag");

	Camera* userCamera = Application::getWindow()->getCamera();
	this->radiosityRenderer->setCamera(userCamera);
	this->emissionsRenderer->setCamera(userCamera);
	this->facesRenderer->setCamera(userCamera);

	this->renderer = this->radiosityRenderer;
	this->mode = mode;
	this->setMode(mode);
}

void DisplayController::render() {
	this->renderer->render();
}

GLuint DisplayController::read() {
	return this->renderer->read();
}

void DisplayController::setMode(RunMode mode) {
	this->mode = mode;
	switch (mode) {
	case PREPROCESS:
		this->renderer = this->bufferRenderer;
		break;
	case DISPLAY_RADIOSITY:
		this->renderer = this->radiosityRenderer;
		break;
	case DISPLAY_EMISSION:
		this->renderer = this->emissionsRenderer;
		break;
	case DISPLAY_FACES:
		this->renderer = this->facesRenderer;
		break;
	}
}

DisplayController::~DisplayController() {
	delete this->bufferRenderer;
}
