#include "preprocessing/Pipeline.h"

#include "EngineStore.h"
#include "preprocessing/EigenSolver.h"

Pipeline::Pipeline(Scene* scene, GLuint resolution) {
  this->enableReflections = false;
  this->enableInterpolation = false;

  this->scene = scene;

  this->currentStage = INIT;

  // Thread flags
  radiosityReady[RED] = false;
  radiosityReady[GREEN] = false;
  radiosityReady[BLUE] = false;

  solverType = SLU;
}

void Pipeline::configureFace(Face* face) { this->face = face; }

void Pipeline::configureFaceIndex(GLuint index) { this->index = index; }

void Pipeline::configureReflections(bool enable) {
  this->enableReflections = enable;
}

void Pipeline::configureChannels(std::set<Channel> channels) {
  this->channels = channels;
}

void Pipeline::configureInterpolation(bool enable) {
  this->enableInterpolation = enable;
}

void Pipeline::configureSolver(EigenSolverType solver) {
  this->solverType = solver;
}

PipelineStage Pipeline::pollState() {
  if (this->currentStage == INIT) {
  } else if (this->currentStage == FF_LOADING) {
    if (this->ready()) {
      this->currentStage = FF_READY;
      EngineStore::logger.log("Computed Form Factors in " +
                              std::to_string(t.get()) + " seconds");
    }

  } else if (this->currentStage == FF_READY) {
  } else if (this->currentStage == RADIOSITY_LOADING) {
    bool done = true;
    EngineStore::radiosityProgress = 0;
    for (auto& channel : this->channels) {
      done = done && radiosityReady[channel];
      if (radiosityReady[channel]) {
        EngineStore::radiosityProgress += 1.0f / GLfloat(channels.size());
      }
    }
    if (done) {
      for (auto& channel : this->channels) {
        radiosityReady[channel] = false;
      }
      this->currentStage = RADIOSITY_READY;
      EngineStore::logger.log("Computed radiosity in " +
                              std::to_string(t.get()) + " seconds");
      std::vector<glm::vec3> radiosityVec3;
      radiosityVec3.reserve(this->radiosity[RED].size());
      for (GLuint i = 0; i < this->radiosity[RED].size(); i++) {
        glm::vec3 rad(.0f);
        for (GLuint channel = 0; channel < this->channels.size(); channel++) {
          rad[channel] = this->radiosity[channel][i];
        }
        radiosityVec3.push_back(rad);
      }
      this->scene->setRadiosity(radiosityVec3, this->enableInterpolation);
    }
  } else if (this->currentStage == RADIOSITY_READY) {
  }
  return this->currentStage;
}

std::vector<std::tuple<GLuint, GLuint, GLfloat>> Pipeline::getTriplets() {
  return this->triplets;
}

void Pipeline::setTriplets(std::vector<std::tuple<GLuint, GLuint, GLfloat>>) {
  this->triplets = triplets;
  this->setStage(FF_READY);
}

void Pipeline::crWrapped(Channel channel) {
  EigenSolver* solver;

  switch (this->solverType) {
    case SLU:
      solver = new EigenSolverSparseLU();
      break;
    case SLDLT:
      solver = new EigenSolverSimplicialLDLT();
      break;
    case CG:
      solver = new EigenSolverConjugateGradient();
      break;
    case BICGSTAB:
      solver = new EigenSolverBiCGSTAB();
      break;
    default:
      solver = new EigenSolverSparseLU();
      break;
  }

  solver->init(this->scene->size(), this->triplets);

  std::vector<glm::vec3> reflactanceRGB = this->scene->getReflactance();
  std::vector<GLfloat> channelReflactance;
  channelReflactance.reserve(reflactanceRGB.size());
  for (glm::vec3& reflactance : reflactanceRGB) {
    channelReflactance.push_back(reflactance[channel]);
  }
  solver->multiplyReflactance(channelReflactance);

  std::vector<GLfloat> emissions = this->scene->getEmission();
  this->radiosity[channel] = solver->solve(emissions);
  this->radiosityReady[channel] = true;
  delete solver;
}

void Pipeline::computeRadiosity() {
  this->waitForRadiosityWorkers();
  this->currentStage = RADIOSITY_LOADING;
  t = Timer();
  radiosityReady[RED] = false;
  radiosityReady[GREEN] = false;
  radiosityReady[BLUE] = false;
  for (Channel channel : channels) {
    this->radiosityWorkers.push_back(
        std::thread(&Pipeline::crWrapped, this, channel));
  }
}

void Pipeline::waitForRadiosityWorkers() {
  for (auto& radiosityWorkers : radiosityWorkers) {
    if (radiosityWorkers.joinable()) radiosityWorkers.join();
  }
  this->radiosityWorkers.clear();
}

Pipeline::~Pipeline() { this->waitForRadiosityWorkers(); }
