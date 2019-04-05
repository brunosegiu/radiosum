#include "preprocessing/PreprocessingController.h"

#include "ConfigurationManager.h"
#include "EngineStore.h"
#include "geometry/Face.h"
#include "preprocessing/opengl/OpenGLPipeline.h"
#include "preprocessing/raytracing/RaytracedPipeline.h"

PreprocessingController::PreprocessingController(Scene* scene,
                                                 RendererType renderer) {
  this->iterator = new SceneIterator(scene);
  GLuint nSamples = std::stoi(ConfigurationManager::get("INTERNAL_WIDTH"));

  this->pipeline = renderer == RAYTRACING
                       ? (Pipeline*)new RaytracedPipeline(scene, nSamples)
                       : (Pipeline*)new OpenGLPipeline(scene, nSamples);
}

GLuint PreprocessingController::runStep() {
  GLuint index = this->iterator->faceIndex();
  if (!iterator->end()) {
    if (EngineStore::pipelineStage == FF_LOADING ||
        EngineStore::pipelineStage == INIT) {
      Face* face = new Face(this->iterator->get());
      this->pipeline->configureFaceIndex(index);
      this->pipeline->configureFace(face);
      this->pipeline->computeFormFactors();
      iterator->nextFace();
    }
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
  if (full) this->pipeline->computeRadiosity();
}

void PreprocessingController::pollState() {
  EngineStore::pipelineStage = this->pipeline->pollState();
}

void PreprocessingController::computeRadiosity(std::set<Channel> channels,
                                               EigenSolverType solver,
                                               bool smooth) {
  this->pipeline->configureInterpolation(smooth);
  this->pipeline->configureChannels(std::set<Channel>(channels));
  this->pipeline->configureSolver(solver);
  this->pipeline->computeRadiosity();
}

std::vector<std::tuple<GLuint, GLuint, GLfloat>>
PreprocessingController::getTriplets() {
  return this->pipeline->getTriplets();
}

void PreprocessingController::setTriplets(
    std::vector<std::tuple<GLuint, GLuint, GLfloat>> triplets) {
  this->pipeline->setTriplets(triplets);
}

PreprocessingController::~PreprocessingController() {
  delete this->iterator;
  delete this->pipeline;
}
