#pragma once

#include <mutex>
#include <thread>
#include <vector>

#include <set>

#include "EngineTypes.h"
#include "Timer.h"
#include "buffers/RowBuffer.h"
#include "geometry/Scene.h"
#include "preprocessing/HemicubeCorrector.h"
#include "preprocessing/IDRenderer.h"
#include "preprocessing/Solver.h"
#include "shaders/ComputeShader.h"

class DiffusePipeline {
 public:
  DiffusePipeline(Scene* scene, GLuint resolution);

  void computeFormFactors();
  void computeRadiosity();

  void configureFace(Face* face);
  void configureFaceIndex(GLuint index);
  void configureReflections(bool enable);
  void configureChannels(std::set<Channel> channels);
  void configureInterpolation(bool enable);
  void configureSolver(Solver* solver);

  std::vector<std::tuple<GLuint, GLuint, GLfloat>> getTriplets();
  void setTriplets(std::vector<std::tuple<GLuint, GLuint, GLfloat>> triplets);

  PipelineStage checkPipelineStage();

  virtual ~DiffusePipeline();

 private:
  // Config

  // Flags
  bool enableReflections;
  std::set<Channel> channels;
  bool enableInterpolation;

  // Geometry, math...
  Scene* scene;
  Face* face;
  GLuint index;

  // Internal

  PipelineStage currentStage;

  IDRenderer* renderer;

  ComputeShader* reducer;
  RowBuffer* row;
  HemicubeCorrector* corrector;
  GLuint instances;

  Timer t;

  // Storage

  // FF
  std::vector<std::tuple<GLuint, GLuint, GLfloat>> triplets;
  std::vector<std::thread> formFactorWorkers;
  std::mutex ffLock;
  GLuint threadsReady;

  // Radiosity
  std::vector<std::thread> radiosityWorkers;
  std::vector<GLfloat> radiosity[N_CHANNELS];
  bool radiosityReady[N_CHANNELS];
  Solver* solver;

  // Internal functions for steps

  // FF
  void setUpRenderer();
  std::vector<GLfloat> getMatrixRow(GLuint face);
  void processRow(std::vector<GLfloat> faceFactors, GLuint faceIndex);

  // Radiosity
  void waitForWorkers();
  void crWrapped(Channel channel);
};
