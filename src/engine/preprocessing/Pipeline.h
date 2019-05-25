#pragma once

#include <mutex>
#include <set>
#include <thread>
#include <vector>

#include "EngineTypes.h"
#include "Timer.h"
#include "geometry/Scene.h"
#include "preprocessing/Pipeline.h"

enum EigenSolverType { SLU, SLDLT, CG, BICGSTAB };

class Pipeline {
 public:
  Pipeline(Scene* scene, GLuint resolution);

  virtual void computeFormFactors() = 0;
  void computeRadiosity();

  void configureFace(Face* face);
  void configureFaceIndex(GLuint index);
  void configureReflections(bool enable);
  void configureChannels(std::set<Channel> channels);
  void configureInterpolation(bool enable);
  void configureSolver(EigenSolverType solver);

  std::vector<std::tuple<GLuint, GLuint, GLfloat>> getTriplets();
  void setTriplets(std::vector<std::tuple<GLuint, GLuint, GLfloat>> triplets);

  PipelineStage pollState();
  virtual bool ready() = 0;
  virtual void setStage(PipelineStage stage) = 0;

  virtual ~Pipeline();

 protected:
  std::vector<std::tuple<GLuint, GLuint, GLfloat>> triplets;
  PipelineStage currentStage;

  // Geometry, math...
  Scene* scene;

 private:
  // Config

  // Flags
  bool enableReflections;
  std::set<Channel> channels;
  bool enableInterpolation;

  // Internal
  Timer t;

  // Storage

  // FF

  // Radiosity
  std::vector<std::thread> radiosityWorkers;
  std::vector<GLfloat> radiosity[N_CHANNELS];
  bool radiosityReady[N_CHANNELS];
  EigenSolverType solverType;

  void waitForRadiosityWorkers();
  void crWrapped(Channel channel);
};
