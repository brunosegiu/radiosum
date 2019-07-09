#include "preprocessing/PreprocessingController.h"

#include "ConfigurationManager.h"
#include "EngineStore.h"
#include "geometry/Face.h"
#include "preprocessing/opengl/OpenGLPipeline.h"
#include "preprocessing/raytracing/RaytracedPipeline.h"

PreprocessingController::PreprocessingController(Scene* scene,
                                                 RendererType renderer) {
  GLuint nSamples = std::stoi(ConfigurationManager::get("INTERNAL_WIDTH"));

  this->pipeline = renderer == RAYTRACING
                       ? (Pipeline*)new RaytracedPipeline(scene, nSamples)
                       : (Pipeline*)new OpenGLPipeline(scene, nSamples);
}

void PreprocessingController::computeFormFactors(Reflectors reflections) {
  this->pipeline->configureReflections(reflections);
  this->pipeline->computeFormFactors();
}

void PreprocessingController::pollState() {
  EngineStore::pipelineStage = this->pipeline->pollState();
}

void PreprocessingController::computeRadiosity(std::set<Channel> channels,
                                               SolverType solver,
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

PreprocessingController::~PreprocessingController() { delete this->pipeline; }
