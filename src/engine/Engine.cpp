#include "Engine.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include "EngineStore.h"
#include "display/Picker.h"

Engine::Engine() {
  this->mode = FACES;
  this->camera =
      new Camera(1.0f, 90.0f, 0.5f, 5000.0f, glm::vec3(0.0f, 0.0f, 0.0f),
                 glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  this->scene = new Scene();

  this->sceneRenderer = new SceneRenderer(this->camera);
  this->displayer = new DisplayController(this->mode);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
}

// Main camera

Camera* Engine::getCamera() { return this->camera; }

// Engine output

void Engine::setMode(RenderMode mode) {
  if (EngineStore::pipelineStage != FF_LOADING) {
    this->mode = mode;
    this->displayer->setMode(mode);
  }
}

RenderMode Engine::getMode() { return this->mode; }

void Engine::preprocess(bool withOutput, RendererType renderer) {
  if (this->preprocessor) delete this->preprocessor;
  this->preprocessor = new PreprocessingController(this->scene, renderer);
  this->setMode(PREPROCESS);
  if (!withOutput) this->preprocessor->runUnsafe(false);
}

void Engine::computeRadiosity(std::vector<Channel> channels,
                              EigenSolverType solver, bool smooth) {
  std::set<Channel> sChannels;
  for (auto& channel : channels) {
    sChannels.insert(channel);
  }
  this->preprocessor->computeRadiosity(sChannels, solver, smooth);
}

void Engine::setRadiosity(bool smooth) {
  auto rads = this->scene->getRadiosity();
  this->scene->setRadiosity(rads, smooth);
}

// Main scene
void Engine::importGeometry(std::string path) { this->scene->addMesh(path); }

Scene* Engine::getScene() { return this->scene; }

void Engine::step() {
  if (this->preprocessor != nullptr) {
    this->preprocessor->pollState();
  }
  if (this->mode == PREPROCESS && this->preprocessor != nullptr) {
    this->preprocessor->runStep();
  } else {
    this->sceneRenderer->setScene(scene);
    this->sceneRenderer->render();
    this->sceneRenderer->read();
  }
  this->displayer->render();
  this->outputTexture = this->displayer->read();
}

GLuint Engine::pick(GLint x, GLint y) {
  EngineStore::logger.log("Getting face id at: " + std::to_string(x) + "/" +
                          std::to_string(y));
  Picker picker(this->scene->size(),
                dynamic_cast<PickingBuffer*>(this->sceneRenderer->getBuffer()));
  return picker.getSelected(x, y);
}

void Engine::resetScene() {
  Scene* old = this->scene;
  this->scene = new Scene();
  delete old;
}

void Engine::exportGeometry(std::string path) {}

void Engine::exportFFMatrix(std::string path) {
  if (this->preprocessor) {
    auto triplets = this->preprocessor->getTriplets();
    std::ofstream file;
    file.open(path.data());
    for (GLuint index = 0; index < triplets.size(); index++) {
      file << std::get<0>(triplets[index]) << " "
           << std::get<1>(triplets[index]) << " "
           << std::get<2>(triplets[index]) << std::endl;
    }
    file.close();
  } else {
    EngineStore::logger.log("Unable to export form factors, none available.");
  }
}

void Engine::exportEmission(std::string path) {
  std::ofstream file;
  file.open(path.data());
  auto emission = this->scene->getEmission();
  for (GLuint index = 0; index < emission.size(); index++) {
    file << index << " " << emission[index] << std::endl;
  }
  file.close();
}

void Engine::exportReflactance(std::string path) {
  std::ofstream file;
  file.open(path.data());
  auto reflactance = this->scene->getReflactance();
  for (GLuint index = 0; index < reflactance.size(); index++) {
    file << index << " " << reflactance[index].x << " " << reflactance[index].y
         << " " << reflactance[index].z << std::endl;
  }
  file.close();
}

void Engine::exportRadiosity(std::string path) {
  std::ofstream file;
  file.open(path.data());
  auto radiosity = this->scene->getRadiosity();
  for (GLuint index = 0; index < radiosity.size(); index++) {
    file << index << " " << radiosity[index].x << " " << radiosity[index].y
         << " " << radiosity[index].z << std::endl;
  }
  file.close();
}

void Engine::importFFMatrix(std::string path) {
  if (!this->preprocessor) {
    this->preprocessor = new PreprocessingController(this->scene);
    this->setMode(PREPROCESS);
  }
  std::ifstream input(path);
  std::string line;
  std::vector<std::tuple<GLuint, GLuint, GLfloat>> triplets;
  while (getline(input, line)) {
    GLuint indexR = 0;
    GLuint indexC = 0;
    GLfloat value = .0f;
    sscanf_s(line.c_str(), "%d %d %f ", &indexR, &indexC, &value);
    if (indexR < this->scene->size() && indexC < this->scene->size())
      triplets.push_back(
          std::tuple<GLuint, GLuint, GLfloat>(indexR, indexC, value));
  }
  this->preprocessor->setTriplets(triplets);
}

void Engine::importEmission(std::string path) {
  std::ifstream input(path);
  std::string line;
  std::vector<GLfloat> emission(this->scene->size(), .0f);
  while (getline(input, line)) {
    GLuint index = 0;
    GLfloat value = .0f;
    sscanf_s(line.c_str(), "%d %f ", &index, &value);
    if (index < this->scene->size())
      emission[index] = value;
    else
      EngineStore::logger.log("Unable to add emission " +
                              std::to_string(index) + ", out of range.");
  }
  this->scene->setEmission(emission);
}

void Engine::importReflactance(std::string path) {
  std::ifstream input(path);
  std::string line;
  std::vector<glm::vec3> reflactance(this->scene->size(), glm::vec3(.0f));
  while (getline(input, line)) {
    GLuint index = 0;
    glm::vec3 value(.0f);
    sscanf_s(line.c_str(), "%d %f %f %f ", &index, &value.x, &value.y,
             &value.z);
    if (index < this->scene->size())
      reflactance[index] = value;
    else
      EngineStore::logger.log("Unable to add reflactance " +
                              std::to_string(index) + ", out of range.");
  }
  this->scene->setReflactance(reflactance);
}

void Engine::importRadiosity(std::string path) {
  std::ifstream input(path);
  std::string line;
  std::vector<glm::vec3> radiosity(this->scene->size(), glm::vec3(.0f));
  while (getline(input, line)) {
    GLuint index = 0;
    glm::vec3 value(.0f);
    sscanf_s(line.c_str(), "%d %f %f %f ", &index, &value.x, &value.y,
             &value.z);
    if (index < this->scene->size())
      radiosity[index] = value;
    else
      EngineStore::logger.log("Unable to add reflactance " +
                              std::to_string(index) + ", out of range.");
  }
  this->scene->setRadiosity(radiosity);
}

Engine::~Engine() {
  delete this->scene;
  delete this->camera;
  if (this->preprocessor) delete this->preprocessor;
  delete this->sceneRenderer;
  delete this->displayer;
}