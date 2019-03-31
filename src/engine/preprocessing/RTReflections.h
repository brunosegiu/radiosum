#pragma once

#include <chrono>
#include <random>

#include <embree3/rtcore.h>
#include <embree3/rtcore_geometry.h>
#include <embree3/rtcore_ray.h>
#include "preprocessing/ReflectionsPipeline.h"

class RTReflections : public ReflectionsPipeline {
 public:
  RTReflections(std::vector<IndexedBuffers> geometry,
                std::vector<GLfloat> reflactances);
  std::vector<std::tuple<GLuint, GLfloat>> compute(Face* face);
  ~RTReflections();

 private:
  std::vector<GLfloat> reflactances;
  GLuint renderRay(RTCRay ray);
  RTCDevice device;
  RTCScene scene;
  std::mt19937_64 rng;
  std::uniform_real_distribution<GLfloat> uniformGenerator;
};
