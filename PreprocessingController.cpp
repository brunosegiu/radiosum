#include "PreprocessingController.h"

#include "Window.h"
#include "ConfigurationManager.h"
#include "CameraController.h"

PreprocessingController::PreprocessingController(std::vector<Mesh*> meshes) {
	this->renderer = new IDRenderer(meshes);
	this->meshStep = this->faceStep = 0;
	this->meshes = meshes;

	this->bufferRenderer = new BufferRenderer();
}


void PreprocessingController::reset() {
	this->meshStep = this->faceStep = 0;
}

void PreprocessingController::runStep() {
	if (!this->end()) {
		if (ConfigurationManager::get("DEBUG") == "true") {
			this->renderer->setCamera(Window::get()->getCamera());
			this->renderer->render();
			this->renderer->read();
			this->bufferRenderer->render();
		}
		else {
			this->renderer->render();
		}

		// TODO: save results for face

		if (this->meshes[this->meshStep]->size() <= this->faceStep) {
			this->meshStep++;
			this->faceStep = 0;
		}
		else {
			this->faceStep++;
		}
	}
}


bool PreprocessingController::end() {
	return this->meshStep >= this->meshes.size();
}

PreprocessingController::~PreprocessingController() {
}
