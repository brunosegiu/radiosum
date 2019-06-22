#pragma once

#include "Renderer.h"

#include <embree3/rtcore.h>
#include <embree3/rtcore_geometry.h>
#include <embree3/rtcore_ray.h>

class ReflectionsRendererRT : public Renderer {
 public:
  ReflectionsRendererRT(Scene *scene);
  void render();
  void render(Face &face, GLuint faceIndex, Face &reflectiveFace,
              GLuint reflectiveFaceIndex);
  std::vector<GLuint> getData();
  virtual ~ReflectionsRendererRT();

 private:
  GLuint renderRayOnce(RTCRay &ray, RTCRayHit &query);
  RTCDevice device;
  RTCScene eScene;
  std::vector<GLuint> buffer;
  std::vector<GLuint> offsetMap;
};
