#include "preprocessing/DiffusePipeline.h"

#include "EngineStore.h"
#include "preprocessing/EigenSolver.h"

DiffusePipeline::DiffusePipeline(Scene* scene, GLuint resolution) {
  this->enableReflections = false;
  this->enableInterpolation = false;

  this->scene = scene;
  this->renderer = new IDRenderer(scene);
  this->reducer = new ComputeShader("computeRow.comp");
  this->row = new RowBuffer(scene->size() + 1);  // Padding for 0 (void)
  this->instances = resolution;
  this->corrector = new HemicubeCorrector(this->instances);

  this->currentStage = INIT;

  // Thread flags
  radiosityReady[RED] = false;
  radiosityReady[GREEN] = false;
  radiosityReady[BLUE] = false;
  threadsReady = 0;

  solverType = SLU;
}

void DiffusePipeline::configureFace(Face* face) { this->face = face; }

void DiffusePipeline::configureFaceIndex(GLuint index) { this->index = index; }

void DiffusePipeline::configureReflections(bool enable) {
  this->enableReflections = enable;
}

void DiffusePipeline::configureChannels(std::set<Channel> channels) {
  this->channels = channels;
}

void DiffusePipeline::configureInterpolation(bool enable) {
  this->enableInterpolation = enable;
}

void DiffusePipeline::configureSolver(EigenOpt solver) {
  this->solverType = solver;
}

PipelineStage DiffusePipeline::checkPipelineStage() {
  if (this->currentStage == INIT) {
  } else if (this->currentStage == FF_LOADING) {
    ffLock.lock();
    if (threadsReady == this->scene->size()) {
      this->threadsReady = 0;
      this->currentStage = FF_READY;
      EngineStore::logger.log("Computed Form Factors in " +
                              std::to_string(t.get()) + " seconds");
    }
    ffLock.unlock();
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

// Form factor
void DiffusePipeline::setUpRenderer() {
  // Get camera configuration and prepare for render
  {
    Face face = *this->face;
    glm::vec3 origin, normal, up;
    glm::vec4 plane = face.getPlane();
    origin = face.getBarycenter();
    normal = face.getNormal();
    up = glm::normalize(face.v1 - face.v0);
    origin += normal * 0.01f;
    delete this->renderer->getCamera();
    Camera* faceCamera =
        new Camera(1.0f, 90.0f, 0.2f, 500.0f, origin, normal, up);
    this->renderer->setCamera(faceCamera);
    this->renderer->setClipPlane(plane);
  }
}

std::vector<GLfloat> DiffusePipeline::getMatrixRow(GLuint face) {
  this->reducer->bind();
  this->row->bind();
  this->row->clean();
  this->corrector->read();
  this->reducer->run(this->instances, this->instances, 1);
  return this->row->getBuffer();
}

void DiffusePipeline::processRow(std::vector<GLfloat> faceFactors,
                                 GLuint faceIndex) {
  GLuint iIndex = faceIndex;
  for (GLuint jIndex = 0; jIndex < faceFactors.size() - 1; jIndex++) {
    GLfloat ff = GLfloat(faceFactors[jIndex + 1]);
    if (iIndex == jIndex) {
      ffLock.lock();
      triplets.push_back(
          std::tuple<GLuint, GLuint, GLfloat>(iIndex, jIndex, 1.0f));
      ffLock.unlock();
    } else if (ff > 0.0f) {
      ffLock.lock();
      triplets.push_back(
          std::tuple<GLuint, GLuint, GLfloat>(iIndex, jIndex, ff));
      ffLock.unlock();
    }
  }
  ffLock.lock();
  threadsReady++;
  ffLock.unlock();
}

std::vector<std::tuple<GLuint, GLuint, GLfloat>>
DiffusePipeline::getTriplets() {
  return this->triplets;
}

void DiffusePipeline::setTriplets(
    std::vector<std::tuple<GLuint, GLuint, GLfloat>>) {
  this->triplets = triplets;
  this->threadsReady = 0;
  this->currentStage = FF_READY;
}

void DiffusePipeline::computeFormFactors() {
  if (this->index == 0) {
    t = Timer();
  }
  if (this->index < this->scene->size()) {
    this->currentStage = FF_LOADING;
    EngineStore::ffProgress = this->index / GLfloat(this->scene->size());
    this->renderer->start();
    this->setUpRenderer();
    // Render hemicube and read results
    {
      this->renderer->render();
      this->renderer->read();
    }

    // Process hemicube and compute row
    {
      std::vector<GLfloat> faceFactors = this->getMatrixRow(index);
      this->formFactorWorkers.push_back(
          std::thread(&DiffusePipeline::processRow, this, faceFactors, index));
    }
  }
}

void DiffusePipeline::crWrapped(Channel channel) {
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

void DiffusePipeline::computeRadiosity() {
  this->waitForWorkers();
  this->currentStage = RADIOSITY_LOADING;
  t = Timer();
  radiosityReady[RED] = false;
  radiosityReady[GREEN] = false;
  radiosityReady[BLUE] = false;
  for (Channel channel : channels) {
    this->radiosityWorkers.push_back(
        std::thread(&DiffusePipeline::crWrapped, this, channel));
  }
}

void DiffusePipeline::waitForWorkers() {
  for (auto& worker : formFactorWorkers) {
    if (worker.joinable()) worker.join();
  }
  for (auto& radiosityWorkers : radiosityWorkers) {
    if (radiosityWorkers.joinable()) radiosityWorkers.join();
  }
  this->formFactorWorkers.clear();
  this->radiosityWorkers.clear();
}

DiffusePipeline::~DiffusePipeline() { this->waitForWorkers(); }
