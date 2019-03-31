#pragma once

#include "Renderer.h"

class IDRenderer : public Renderer {
 public:
  IDRenderer(Scene* scene);
  void render();
  void read();
  void setClipPlane(glm::vec4 plane);
  virtual ~IDRenderer();

 private:
  glm::vec4 clipPlane;
};
