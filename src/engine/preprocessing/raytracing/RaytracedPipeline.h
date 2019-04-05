#pragma once

#include <thread>

#include <embree3/rtcore.h>
#include <embree3/rtcore_geometry.h>
#include <embree3/rtcore_ray.h>

#include "preprocessing/Pipeline.h"
#include "preprocessing/raytracing/RayGenerator.h"

class RaytracedPipeline : public Pipeline {
 public:
  RaytracedPipeline(Scene* scene, GLuint resolution);
  void computeFormFactors();
  virtual ~RaytracedPipeline();

  bool ready();
  void setStage(PipelineStage stage);

  bool done;

 private:
  void runWr(std::vector<Face> faces);
  std::pair<GLuint, GLfloat> renderRay(RTCRay ray);

  std::vector<GLfloat> reflactances;

  RTCDevice device;
  RTCScene eScene;

  std::thread mainThread;
  std::mutex ffLock;

  GLuint nRays;
  RayGenerator generator;
};
