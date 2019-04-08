#include "preprocessing/opengl/OpenGLPipeline.h"

#include "EngineStore.h"
#include "preprocessing/EigenSolver.h"

OpenGLPipeline::OpenGLPipeline(Scene* scene, GLuint resolution)
    : Pipeline(scene, resolution) {
  this->renderer = new IDRenderer(scene);
  this->reducer = new ComputeShader("computeRow.comp");
  this->row = new RowBuffer(scene->size() + 1);  // Padding for 0 (void)
  this->instances = resolution;
  this->corrector = new HemicubeCorrector(this->instances);

  // Thread flags
  threadsReady = 0;
}

// Form factor
void OpenGLPipeline::setUpRenderer() {
  // Get camera configuration and prepare for render
  {
    Face face = *this->face;
    glm::vec3 origin, normal, up;
    glm::vec4 plane = face.getPlane();
    origin = face.getBarycenter();
    normal = face.getNormal();
    up = face.getUp();
    origin += normal * 0.01f;
    delete this->renderer->getCamera();
    Camera* faceCamera =
        new Camera(1.0f, 90.0f, 0.2f, 500.0f, origin, normal, up);
    this->renderer->setCamera(faceCamera);
    this->renderer->setClipPlane(plane);
  }
}

std::vector<GLfloat> OpenGLPipeline::getMatrixRow(GLuint face) {
  this->reducer->bind();
  this->row->bind();
  this->row->clean();
  this->corrector->read();
  this->reducer->run(this->instances, this->instances, 1);
  return this->row->getBuffer();
}

void OpenGLPipeline::processRow(std::vector<GLfloat> faceFactors,
                                GLuint faceIndex) {
  GLuint iIndex = faceIndex;
  for (GLuint jIndex = 0; jIndex < faceFactors.size() - 1; jIndex++) {
    GLfloat ff = GLfloat(faceFactors[jIndex + 1]);
    if (iIndex == jIndex) {
      ffLock.lock();
      this->triplets.push_back(
          std::tuple<GLuint, GLuint, GLfloat>(iIndex, jIndex, 1.0f));
      ffLock.unlock();
    } else if (ff > 0.0f) {
      ffLock.lock();
      this->triplets.push_back(
          std::tuple<GLuint, GLuint, GLfloat>(iIndex, jIndex, ff));
      ffLock.unlock();
    }
  }
  ffLock.lock();
  threadsReady++;
  ffLock.unlock();
}

void OpenGLPipeline::computeFormFactors() {
  if (this->index < this->scene->size()) {
    this->setStage(FF_LOADING);
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
          std::thread(&OpenGLPipeline::processRow, this, faceFactors, index));
    }
  }
}

bool OpenGLPipeline::ready() {
  return this->threadsReady == this->scene->size();
}

void OpenGLPipeline::setStage(PipelineStage stage) {
  this->currentStage = stage;
}

void OpenGLPipeline::waitForFFWorkers() {
  for (auto& worker : formFactorWorkers) {
    if (worker.joinable()) worker.join();
  }
  this->formFactorWorkers.clear();
}

OpenGLPipeline::~OpenGLPipeline() { this->waitForFFWorkers(); }
