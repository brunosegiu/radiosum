#include "PreprocessingController.h"

#include "Window.h"
#include "ConfigurationManager.h"
#include "CameraController.h"

PreprocessingController::PreprocessingController(Scene* scene) {
	this->renderer = new IDRenderer(scene);
	this->meshStep = this->faceStep = 0;
	this->meshes = meshes;
}


void PreprocessingController::reset() {
	this->meshStep = this->faceStep = 0;
}

void PreprocessingController::runStep() {
	if (!this->end()) {
		// TODO: setup camera correctly per face
		this->renderer->setCamera(ConfigurationManager::get("DEBUG") == "true" ? Window::get()->getCamera() : Window::get()->getCamera());

		this->renderer->render();
		this->renderer->read();

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
