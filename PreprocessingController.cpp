#include "PreprocessingController.h"

#include "Window.h"
#include "ConfigurationManager.h"

PreprocessingController::PreprocessingController(Scene* scene) {
	this->renderer = new IDRenderer(scene);
	this->faceStep = 0;
	this->scene = scene;
	this->reducer = new ComputeShader("computeRow.comp");
	this->row = new RowBuffer(scene->size());
	std::string widthstr = ConfigurationManager::get("INTERNAL_WIDTH");
	this->instances = std::stoi(widthstr);
}


void PreprocessingController::reset() {
	this->faceStep = 0;
}

void PreprocessingController::runStep() {
	if (!this->end()) {
		// TODO: setup camera correctly per face
		this->renderer->setCamera(ConfigurationManager::get("DEBUG") == "true" ? Window::get()->getCamera() : Window::get()->getCamera());

		this->renderer->render();
		this->renderer->read();

		// TODO: save results for face
		std::vector<GLuint> faceFactors = this->getMatrixRow(this->faceStep);

		if (this->scene->size() < this->faceStep) {
			this->faceStep++;
		}
	}
}

std::vector<GLuint> PreprocessingController::getMatrixRow(GLuint face) {
	this->row->bind();
	this->reducer->bind();
	this->reducer->run(this->instances, this->instances, 1);
	return this->row->getBuffer();
}

bool PreprocessingController::end() {
	return this->faceStep >= this->scene->size();
}

PreprocessingController::~PreprocessingController() {
}
