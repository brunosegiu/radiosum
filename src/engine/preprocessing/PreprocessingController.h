#pragma once

#include <vector>

#include "EngineTypes.h"
#include "geometry/Scene.h"
#include "preprocessing/Pipeline.h"

enum RendererType { OPENGL, RAYTRACING };

class PreprocessingController {
 public:
  PreprocessingController(Scene* scene, RendererType = OPENGL);
  void computeFormFactors(Reflectors reflectors = DISABLED);
  void computeRadiosity(std::set<Channel> channels,
                        EigenSolverType solver = SLU, bool smooth = false);
  void pollState();
  std::vector<std::tuple<GLuint, GLuint, GLfloat>> getTriplets();
  void setTriplets(std::vector<std::tuple<GLuint, GLuint, GLfloat>>);
  virtual ~PreprocessingController();

 private:
  Pipeline* pipeline;
};
