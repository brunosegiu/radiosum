#include "preprocessing/PreprocessingController.h"

#include "ConfigurationManager.h"
#include "geometry/Face.h"
#include "EngineStore.h"

PreprocessingController::PreprocessingController(Scene* scene) {
	this->iterator = new SceneIterator(scene);
	this->pipeline = new Pipeline(scene, std::stoi(ConfigurationManager::get("INTERNAL_WIDTH")));
}

GLuint PreprocessingController::runStep() {
	GLuint index = this->iterator->faceIndex();
	if (EngineStore::pipelineStage == FF_LOADING || EngineStore::pipelineStage == INIT) {
		Face* face = new Face(this->iterator->get());
		this->pipeline->configureFaceIndex(index);
		this->pipeline->configureFace(face);
		this->pipeline->computeFormFactors();
		iterator->nextFace();
	}
	return index;
}

void PreprocessingController::runUnsafe(bool full) {
	while (!this->iterator->end()) {
		GLuint index = this->iterator->faceIndex();
		Face* face = new Face(this->iterator->get());
		this->pipeline->configureFaceIndex(index);
		this->pipeline->configureFace(face);
		this->pipeline->computeFormFactors();
		iterator->nextFace();
	}
	if (full)
		this->pipeline->computeRadiosity();
}

void PreprocessingController::checkFlags() {
	EngineStore::pipelineStage = this->pipeline->checkPipelineStage();
}

void PreprocessingController::computeRadiosity(std::set<Channel> channels, bool smooth) {
	this->pipeline->configureInterpolation(smooth);
	this->pipeline->configureChannels(std::set<Channel>(channels));
	this->pipeline->computeRadiosity();
}

PreprocessingController::~PreprocessingController() {
	delete this->iterator;
	delete this->pipeline;
}
