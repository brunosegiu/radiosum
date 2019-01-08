#include "PreprocessingController.h"

#include "Window.h"
#include "ConfigurationManager.h"
#include "CameraController.h"

PreprocessingController::PreprocessingController(Scene* scene) {
	this->renderer = new IDRenderer(scene);
	this->faceStep = 0;
	this->scene = scene;
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

		if (this->scene->size() < this->faceStep) {
			this->faceStep++;
		}
	}
}


bool PreprocessingController::end() {
	return this->faceStep >= this->scene->size();
}

PreprocessingController::~PreprocessingController() {
}
