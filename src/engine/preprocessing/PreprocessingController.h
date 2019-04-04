#pragma once

#include <vector>

#include "EngineTypes.h"
#include "geometry/Scene.h"
#include "preprocessing/DiffusePipeline.h"
#include "preprocessing/RTReflections.h"

class PreprocessingController {
 public:
  PreprocessingController(Scene* scene);
  GLuint runStep();
  void runUnsafe(bool full = false);
  void computeRadiosity(std::set<Channel> channels, EigenOpt solver = SLU,
                        bool smooth = false);
  void checkFlags();
  std::vector<std::tuple<GLuint, GLuint, GLfloat>> getTriplets();
  void setTriplets(std::vector<std::tuple<GLuint, GLuint, GLfloat>>);
  virtual ~PreprocessingController();

 private:
  SceneIterator* iterator;
  DiffusePipeline* dPipeline;
  RTReflections* rPipeline;
};
