#include "preprocessing/PreprocessingController.h"

#include "ConfigurationManager.h"
#include "EngineStore.h"
#include "geometry/Face.h"

PreprocessingController::PreprocessingController(Scene* scene) {
  this->iterator = new SceneIterator(scene);
  this->dPipeline = new DiffusePipeline(
      scene, std::stoi(ConfigurationManager::get("INTERNAL_WIDTH")));
  //	this->rPipeline = new RTReflections(this->scene);
}

GLuint PreprocessingController::runStep() {
  GLuint index = this->iterator->faceIndex();
  if (EngineStore::pipelineStage == FF_LOADING ||
      EngineStore::pipelineStage == INIT) {
    Face* face = new Face(this->iterator->get());
    this->dPipeline->configureFaceIndex(index);
    this->dPipeline->configureFace(face);
    this->dPipeline->computeFormFactors();
    iterator->nextFace();
  }
  return index;
}

void PreprocessingController::runUnsafe(bool full) {
  while (!this->iterator->end()) {
    GLuint index = this->iterator->faceIndex();
    Face* face = new Face(this->iterator->get());
    this->dPipeline->configureFaceIndex(index);
    this->dPipeline->configureFace(face);
    this->dPipeline->computeFormFactors();
    iterator->nextFace();
  }
  if (full) this->dPipeline->computeRadiosity();
}

void PreprocessingController::checkFlags() {
  EngineStore::pipelineStage = this->dPipeline->checkPipelineStage();
}

void PreprocessingController::computeRadiosity(std::set<Channel> channels,
                                               bool smooth) {
  this->dPipeline->configureInterpolation(smooth);
  this->dPipeline->configureChannels(std::set<Channel>(channels));
  this->dPipeline->computeRadiosity();
}

std::vector<std::tuple<GLuint, GLuint, GLfloat>>
PreprocessingController::getTriplets() {
  return this->dPipeline->getTriplets();
}

void PreprocessingController::setTriplets(
    std::vector<std::tuple<GLuint, GLuint, GLfloat>> triplets) {
  this->dPipeline->setTriplets(triplets);
}

PreprocessingController::~PreprocessingController() {
  delete this->iterator;
  delete this->dPipeline;
}
