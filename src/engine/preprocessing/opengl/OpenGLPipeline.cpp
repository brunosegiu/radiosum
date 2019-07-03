#include "preprocessing/opengl/OpenGLPipeline.h"

#include <set>

#include "EngineStore.h"
#include "buffers/HemicubeBuffer.h"
#include "preprocessing/EigenSolver.h"

#define MAX_REFLECTION_DEPTH 5

OpenGLPipeline::OpenGLPipeline(Scene* scene, GLuint resolution)
    : Pipeline(scene, resolution) {
  this->renderer = new IDRenderer(scene);
  this->reflectionsRenderer = new ReflectionsRenderer(scene);
  this->reflectionsRendererRT = new ReflectionsRendererRT(scene);
  this->reducer = new ComputeShader("computeRow.comp");
  this->row = new RowBuffer(scene->size() + 1);  // Padding for 0 (void)
  this->resolution = resolution;
  this->corrector = new HemicubeCorrector(this->resolution);

  // Thread flags
  threadsReady = 0;
}

// Form factor
void OpenGLPipeline::computeFormFactors() {
  this->setStage(FF_LOADING);
  SceneIterator iterator(this->scene);
  this->renderer->start();
  while (!iterator.end()) {
    Face face = iterator.get();
    GLuint index = iterator.faceIndex();
    if (this->reflector != DISABLED) {
      this->renderer->start();
    }
    this->setUpRenderer(face);
    this->renderer->render();

    // Process hemicube and compute row
    {
      std::vector<GLfloat> faceFactors = this->getMatrixRowCPU(face, index);
      this->formFactorWorkers.push_back(
          std::thread(&OpenGLPipeline::processRow, this, faceFactors, index));
    }
    iterator.nextFace();
  }
  EngineStore::ffProgress = 1.0f;
}

void OpenGLPipeline::setUpRenderer(Face& face) {
  // Get camera configuration and prepare for render
  {
    glm::vec3 origin, normal, up;
    glm::vec4 plane = face.getPlane();
    origin = face.getBarycenter();
    normal = face.getNormal();
    up = face.getUp();
    origin += normal * 1e-5f;
    delete this->renderer->getCamera();
    Camera* faceCamera =
        new Camera(1.0f, 90.0f, 0.2f, 500.0f, origin, normal, up);
    this->renderer->setCamera(faceCamera);
    this->renderer->setClipPlane(plane);
  }
}

std::vector<GLfloat> OpenGLPipeline::getMatrixRowGPU() {
  this->reducer->bind();
  this->row->bind();
  this->row->clean();
  this->corrector->read();
  this->reducer->run(this->resolution, this->resolution, 1);
  return this->row->getBuffer();
}

std::vector<GLfloat> OpenGLPipeline::getMatrixRowCPU(Face& face, GLuint index) {
  HemicubeBuffer* buffer =
      dynamic_cast<HemicubeBuffer*>(this->renderer->getBuffer());
  std::vector<std::vector<GLuint>> data = buffer->getData();
  GLuint rowSize = scene->size();

  std::vector<GLfloat> topCorrector = this->corrector->topData;
  std::vector<GLfloat> rowTopFace = std::vector<GLfloat>(rowSize, 0.0f);

  std::vector<GLfloat> reflactances = this->scene->getSpecularReflactance();

  std::vector<std::map<GLuint, GLfloat>> reflectionsSteps =
      std::vector<std::map<GLuint, GLfloat>>(MAX_REFLECTION_DEPTH);

  for (GLuint i = 0; i < resolution; i++) {
    for (GLuint j = 0; j < resolution; j++) {
      GLuint index = i * resolution + j;
      GLuint seenFace = data[0][index];
      if (seenFace > 0) {
        seenFace = seenFace - 1;
        GLfloat addition = topCorrector[index];
        GLfloat reflactance =
            this->reflector != DISABLED ? reflactances[seenFace] : 0;
        rowTopFace[seenFace] += (1.0f - reflactance) * addition;
        if (reflactance > 0) {
          if (reflectionsSteps[0].count(seenFace) > 0) {
            reflectionsSteps[0][seenFace] += reflactance * addition;
          } else {
            reflectionsSteps[0][seenFace] = reflactance * addition;
          }
        }
      }
    }
  }

  std::vector<GLfloat> sideCorrector = this->corrector->sideData;
  std::vector<std::vector<GLfloat>> rowPerSideFace =
      std::vector<std::vector<GLfloat>>(HEMICUBE_FACES - 1,
                                        std::vector<GLfloat>(rowSize, 0.0f));
  GLuint readOffset = resolution * resolution / 2;
#pragma omp parallel for
  for (GLint face = 1; face < HEMICUBE_FACES - 1; face++) {
    for (GLuint i = 0; i < resolution; i++) {
      for (GLuint j = 0; j < resolution / 2; j++) {
        GLuint index = i * resolution / 2 + j;
        GLuint seenFace = data[face][readOffset + index];
        GLfloat addition = sideCorrector[index];
        if (seenFace > 0) {
          seenFace = seenFace - 1;
          GLfloat reflactance =
              this->reflector != DISABLED ? reflactances[seenFace] : 0;
          rowPerSideFace[face][seenFace] += (1.0f - reflactance) * addition;
          if (reflactance > 0) {
            if (reflectionsSteps[0].count(seenFace) > 0) {
              reflectionsSteps[0][seenFace] += reflactance * addition;
            } else {
              reflectionsSteps[0][seenFace] = reflactance * addition;
            }
          }
        }
      }
    }
  }

  if (this->reflector != DISABLED) {
    for (GLuint step = 0; step < reflectionsSteps.size(); step++) {
      std::map<GLuint, GLfloat> reflectionSet = reflectionsSteps[step];
      for (std::pair<GLuint, GLfloat> pair : reflectionSet) {
        GLuint reflectiveIndex = pair.first;
        GLfloat addition = pair.second;
        Face reflectiveFace = this->scene->getFace(reflectiveIndex);
        std::vector<GLuint> seenFaces;
        if (this->reflector == REMBREE) {
          this->reflectionsRendererRT->render(face, index, reflectiveFace,
                                              reflectiveIndex);
          seenFaces = this->reflectionsRenderer->getData();
        } else {
          this->reflectionsRenderer->render(face, index, reflectiveFace,
                                            reflectiveIndex);
          seenFaces = this->reflectionsRenderer->getData();
        }
        GLfloat normalizer = GLfloat(seenFaces.size());
        for (auto& seenFace : seenFaces) {
          if (seenFace > 0 && seenFace < reflactances.size() &&
              step + 1 < reflectionsSteps.size()) {
            seenFace = seenFace - 1;
            GLfloat reflactance = reflactances[seenFace];
            rowTopFace[seenFace] +=
                (1.0f - reflactance) * addition / normalizer;
            if (reflactance > 0) {
              if (reflectionsSteps[step].count(seenFace) > 0) {
                reflectionsSteps[step][seenFace] +=
                    reflactance * addition / normalizer;
              } else {
                reflectionsSteps[step][seenFace] =
                    reflactance * addition / normalizer;
              }
            }
          }
        }
      }
    }
  }

  for (GLint face = 1; face < FACES - 1; face++) {
    for (GLuint i = 0; i < rowSize; i++) {
      rowTopFace[i] += rowPerSideFace[face][i];
    }
  }
  return rowTopFace;
}

void OpenGLPipeline::processRow(std::vector<GLfloat> faceFactors,
                                GLuint faceIndex) {
  GLuint iIndex = faceIndex;
  for (GLuint jIndex = 0; jIndex < faceFactors.size(); jIndex++) {
    GLfloat ff = GLfloat(faceFactors[jIndex]);
    if (ff > 0.0f) {
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

OpenGLPipeline::~OpenGLPipeline() {
  this->waitForFFWorkers();
  delete this->reflectionsRenderer;
  delete this->reflectionsRendererRT;
}
