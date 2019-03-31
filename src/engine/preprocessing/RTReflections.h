#pragma once

#include <chrono>
#include <random>
#include <thread>

#include <embree3/rtcore.h>
#include <embree3/rtcore_geometry.h>
#include <embree3/rtcore_ray.h>
#include "preprocessing/ReflectionsPipeline.h"

class RTReflections : public ReflectionsPipeline {
 public:
  RTReflections(Scene* scene);
  void run();
  ~RTReflections();
  bool done;

 private:
  void runWr(std::vector<Face> faces);
  std::vector<std::tuple<GLuint, GLfloat, GLfloat>> triples;
  std::vector<GLfloat> reflactances;
  GLuint renderRay(RTCRay ray);
  RTCDevice device;
  RTCScene scene;
  Scene* nscene;
  std::vector<std::tuple<GLuint, GLuint, GLfloat>> triplets;
  std::mt19937_64 rng;
  std::uniform_real_distribution<GLfloat> uniformGenerator;
  std::thread main;
};
