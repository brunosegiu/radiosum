#pragma once

#include "buffers/RowBuffer.h"
#include "preprocessing/Pipeline.h"
#include "preprocessing/opengl/HemicubeCorrector.h"
#include "preprocessing/opengl/IDRenderer.h"
#include "shaders/ComputeShader.h"

class OpenGLPipeline : public Pipeline {
 public:
  OpenGLPipeline(Scene* scene, GLuint resolution);
  void computeFormFactors();
  virtual ~OpenGLPipeline();

  bool ready();
  void setStage(PipelineStage stage);

 private:
  // Internal
  IDRenderer* renderer;
  ComputeShader* reducer;
  RowBuffer* row;
  HemicubeCorrector* corrector;
  GLuint resolution;

  // Storage

  // FF
  std::vector<std::thread> formFactorWorkers;
  std::mutex ffLock;
  GLuint threadsReady;

  // FF
  void setUpRenderer();
  std::vector<GLfloat> getMatrixRowGPU();
  std::vector<GLfloat> getMatrixRowCPU();
  void processRow(std::vector<GLfloat> faceFactors, GLuint faceIndex);
  void waitForFFWorkers();
};
