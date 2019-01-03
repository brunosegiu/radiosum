#include "PreprocessingController.h"

#include "ConfigurationManager.h"

PreprocessingController::PreprocessingController(std::vector<Mesh*> meshes) {
	this->renderer = new IDRenderer(meshes);
	this->meshStep = this->faceStep = 0;
	this->meshes = meshes;
	this->buffer = new UIntBuffer(std::stoi(ConfigurationManager::get("INTERNAL_WIDTH")), std::stoi(ConfigurationManager::get("INTERNAL_HEIGHT")));
	this->bufferRenderer = new BufferRenderer();
}


void PreprocessingController::reset() {
	this->meshStep = this->faceStep = 0;
}

void PreprocessingController::runStep() {
	if (!this->end()) {
		this->buffer->bind();
		this->renderer->render();
		if (ConfigurationManager::get("DEBUG") == "true") {
			this->buffer->read();
			this->bufferRenderer->render();
		}

		// TODO: save results for face


		if (this->meshes[this->meshStep]->size() >= this->faceStep) {
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
