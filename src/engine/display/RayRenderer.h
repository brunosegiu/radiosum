#pragma once

#include "Renderer.h"

class RayRenderer : public Renderer {
 public:
  RayRenderer(Camera* camera);
  void render();
  void setRays(glm::vec3 orig, std::vector<glm::vec3> dirs);
  void setRenderRays(bool value);
  virtual ~RayRenderer();

 private:
  glm::vec3 origs;
  std::vector<glm::vec3> dirs;
};
